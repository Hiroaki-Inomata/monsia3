SUBDIRS = po data gladeui src plugins 

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST = \
	autogen.sh \
	COPYING.GPL \
	COPYING.LGPL \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update

DISTCHECK_CONFIGURE_FLAGS = --disable-scrollkeeper --enable-gtk-doc
