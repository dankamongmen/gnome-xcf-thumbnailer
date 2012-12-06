
bin_PROGRAMS = gnome-xcf-thumbnailer

gnome_xcf_thumbnailer_SOURCES =					\
	gnome-xcf-thumbnailer.c					\
	enums.c enums.h						\
	flatspec.c						\
	flatten.c flatten.h					\
	io-unix.c						\
	palette.c palette.h					\
	pixels.c pixels.h					\
	scaletab.c						\
	table.c							\
	utils.c							\
	xcf-general.c						\
	xcftools.h						\
	options.i xcf2png.oi

gnome_xcf_thumbnailer_CPPFLAGS = $(LIBPNG_CFLAGS) $(GIO_CFLAGS) -DOPTSTRING=\"h\"

gnome_xcf_thumbnailer_LDADD = $(LIBPNG_LIBS) $(GIO_LIBS) -lpopt


schemasdir = $(GCONF_SCHEMA_FILE_DIR)
schemas_in_files = gnome-xcf-thumbnailer.schemas.in
schemas_DATA = gnome-xcf-thumbnailer.schemas

# Rule to make the service file with bindir expanded
$(schemas_DATA): $(schemas_in_files) Makefile
	@$(SED) -e "s|\@bindir\@|$(bindir)|" $< > $@

if GCONF_SCHEMAS_INSTALL
install-data-local:
	if test -z "$(DESTDIR)" ; then \
          for p in $(schemas_DATA) ; do \
            GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $$p ; \
          done \
        fi
else
install-data-local:
endif

EXTRA_DIST = gnome-xcf-thumbnailer.schemas.in AUTHORS COPYING README TODO xcf2png.c

CLEANFILES = gnome-xcf-thumbnailer.schemas
