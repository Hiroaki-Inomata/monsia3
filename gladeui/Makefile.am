
common_defines = \
	-DG_LOG_DOMAIN=\"GladeUI\"                      \
	-DGLADE_DATADIR="\"$(pkgdatadir)\""		\
	-DGLADE_LIBDIR="\"$(pkglibdir)\""		\
	-DGLADE_CATALOGSDIR="\"$(pkgdatadir)/catalogs\""\
	-DGLADE_MODULESDIR="\"$(pkglibdir)/modules\""	\
	-DGLADE_PLUGINSDIR="\"$(pkglibdir)/plugins\""	\
	-DGLADE_PIXMAPSDIR="\"$(pkgdatadir)/pixmaps\""	\
	-DGLADE_LOCALEDIR="\"$(datadir)/locale\""

lib_LTLIBRARIES = libmonsia3.la

BUILT_SOURCES = glade-marshallers.c glade-marshallers.h

EXTRA_DIST = glade-marshallers.list icon-naming-spec.c glade-property-save-list.txt

# The glade-3 core library
libmonsia3_la_SOURCES = \
	glade-widget-adaptor.c \
	glade-debug.c \
	glade-project.c \
	glade-parameter.c \
	glade-placeholder.c \
	glade-custom.c \
	glade-inspector.c \
	glade-xml-utils.c \
	glade-palette.c \
	glade-palette-item.c \
	glade-palette-box.c \
	glade-palette-expander.c \
	glade-design-layout.c \
	glade-design-view.c \
	glade-named-icon-chooser-dialog.c \
	glade-named-icon-chooser-dialog.h \
	glade-cursor.c \
	glade-catalog.c \
	glade-property.c \
	glade-property-class.c \
	glade-popup.c \
	glade-widget.c \
	glade-editor-property.c \
	glade-editor.c \
	glade-utils.c \
	glade-signal.c \
	glade-signal-editor.c \
	glade-clipboard.c \
	glade-clipboard-view.c \
	glade-command.c \
	glade-id-allocator.c \
	glade-id-allocator.h \
	glade-marshallers.c \
	glade-accumulators.c \
	glade-parser.c \
	glade-builtins.c \
	glade-app.c \
	glade-keysyms.c \
	glade-fixed.c \
	glade-base-editor.c \
	glade-palette-item.h \
	glade-palette-box.h \
	glade-palette-expander.h \
	glade-popup.h \
	glade-catalog.h \
	glade-marshallers.h \
	glade-accumulators.h \
	glade-widget-action.c \
	glade-property-save-list.txt

libmonsia3_la_CPPFLAGS =  \
	$(common_defines)   \
	-I$(top_srcdir)     \
	-I$(top_builddir)   \
	$(AM_CPPFLAGS)

libmonsia3_la_CFLAGS =    \
	$(GTK_CFLAGS)       \
	$(WARN_CFLAGS)      \
	$(AM_CFLAGS)

libmonsia3_la_LDFLAGS = -version-info $(GLADE_CURRENT):$(GLADE_REVISION):$(GLADE_AGE) $(AM_LDFLAGS)
libmonsia3_la_LIBADD = $(GTK_LIBS)

if PLATFORM_WIN32
libmonsia3_la_LDFLAGS += -no-undefined
endif

if NATIVE_WIN32
libmonsia3_la_LDFLAGS += -Wl,gladeui-win32-res.o -export-symbols gladeui.def
libmonsia3_la_DEPENDENCIES = gladeui-win32-res.o gladeui.def
endif

# gladeui.def: auto-generated DEF file for linker input
# The routines below will scan the header files and extracts symbol declarations.
# The symbol 'custom_get_type' is added manually. It's naming deviates from
# the pattern set by all other public symbols in the glade namespace.
#
gladeui.def: $(libgladeuiinclude_HEADERS)
	@echo Generating $@
	@(echo EXPORTS; \
	echo 'custom_get_type'; \
	(cd $(srcdir); cat $(libgladeuiinclude_HEADERS) || echo 'glade_ERROR ()' ) | \
	grep -v -E '^# *include' | \
	grep -v -E '^# *define' | \
	grep -E '[^\(]glade_.*[[:space:]]+\(.*' | \
	sed -e 's/[ 	]*(.*//' | \
	sed -e 's/.*glade_/glade_/' | \
	sort; \
	) >$@
	@ ! grep -q glade_ERROR $@ || ($(RM) $@; false)

#
# Marshaller generation
#
glade-marshallers.h: glade-marshallers.list $(GLIB_GENMARSHAL)
	$(GLIB_GENMARSHAL) --prefix=glade_marshal $(srcdir)/glade-marshallers.list --header > glade-marshallers-h.tmp \
	&& mv glade-marshallers-h.tmp glade-marshallers.h \
	|| ( rm -f glade-marshallers-h.tmp && exit 1)
glade-marshallers.c: glade-marshallers.list $(GLIB_GENMARSHAL)
	$(GLIB_GENMARSHAL) --prefix=glade_marshal $(srcdir)/glade-marshallers.list --body --header > glade-marshallers-c.tmp     \
	&& mv glade-marshallers-c.tmp glade-marshallers.c \
	|| ( rm -f glade-marshallers-c.tmp && exit 1 )


CLEANFILES = $(BUILT_SOURCES) gladeui.def
