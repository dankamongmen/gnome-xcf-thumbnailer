/* 
 * Copyright (C) 2008 Bastien Nocera <hadess@hadess.net>
 *
 * Authors: Bastien Nocera <hadess@hadess.net>
 * Henning Makholm <henning@makholm.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "config.h"

/* Make sure the xcf2png.c's main doesn't get used */
#define main(x, y) foobar(x, y)
#define opt_usage(f) {}
#include "xcf2png.c"
#undef main

#include <string.h>
#include <gio/gio.h>

static int output_size = 64;
static gboolean g_fatal_warnings = FALSE;
static char **filenames = NULL;

static const GOptionEntry entries[] = {
	{ "size", 's', 0, G_OPTION_ARG_INT, &output_size, "Size of the thumbnail in pixels", NULL },
	{"g-fatal-warnings", 0, 0, G_OPTION_ARG_NONE, &g_fatal_warnings, "Make all warnings fatal", NULL},
 	{ G_OPTION_REMAINING, '\0', 0, G_OPTION_ARG_FILENAME_ARRAY, &filenames, NULL, "[FILE...]" },
	{ NULL }
};

int main (int argc, char **argv)
{
	GError *error = NULL;
	GOptionContext *context;
	char *input, *output;

	/* Options parsing */
	context = g_option_context_new ("Thumbnail Nintendo DS ROMs");
	g_option_context_add_main_entries (context, entries, NULL);
	g_type_init ();

	if (g_option_context_parse (context, &argc, &argv, &error) == FALSE) {
		g_warning ("Couldn't parse command-line options: %s", error->message);
		g_error_free (error);
		return 1;
	}

	/* Set fatal warnings if required */
	if (g_fatal_warnings) {
		GLogLevelFlags fatal_mask;

		fatal_mask = g_log_set_always_fatal (G_LOG_FATAL_MASK);
		fatal_mask |= G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL;
		g_log_set_always_fatal (fatal_mask);
	}

	if (filenames == NULL || g_strv_length (filenames) != 2) {
		g_print ("Expects an input and an output file\n");
		return 1;
	}
	input = filenames[0];
	output = filenames[1];

	init_flatspec(&flatspec) ;
	flatspec.output_filename = output;
	read_or_mmap_xcf(input,NULL);
	getBasicXcfInfo() ;
	initColormap();

	complete_flatspec(&flatspec,guessIndexed);
	if( flatspec.process_in_memory ) {
		rgba **allPixels = flattenAll(&flatspec);

		analyse_colormode(&flatspec,allPixels,guessIndexed);

		/* See if we can do alpha compaction.
		*/
		if( flatspec.partial_transparency_mode != ALLOW_PARTIAL_TRANSPARENCY &&
		    !FULLALPHA(flatspec.default_pixel) &&
		    flatspec.out_color_mode != COLOR_INDEXED ) {
			rgba unused = findUnusedColor(allPixels,
						      flatspec.dim.width,
						      flatspec.dim.height);
			if( unused && (flatspec.out_color_mode == COLOR_RGB ||
				       degrayPixel(unused) >= 0) ) {
				unsigned x,y ;
				unused = NEWALPHA(unused,0) ;
				for( y=0; y<flatspec.dim.height; y++)
					for( x=0; x<flatspec.dim.width; x++)
						if( allPixels[y][x] == 0 )
							allPixels[y][x] = unused ;
				flatspec.default_pixel = unused ;
			}
		}
		shipoutWithCallback(&flatspec,allPixels,selectCallback());
	} else {
		flattenIncrementally(&flatspec,selectCallback());
	}
	if( libpng ) {
		png_write_end(libpng,libpng2);
		png_destroy_write_struct(&libpng,&libpng2);
	}
	closeout(outfile,output);

	return 0;
}

