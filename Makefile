# Generated automatically from Makefile.in by configure.
# KDE tags expanded automatically by am_edit - $Revision$ 
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = .
prefix = /home/bernd/kde2
exec_prefix = ${prefix}
#>- 
bindir = ${exec_prefix}/bin
#>+ 3
DEPDIR = .deps

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/gideon
pkglibdir = $(libdir)/gideon
pkgincludedir = $(includedir)/gideon

top_builddir = .

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/ginstall -c -p
INSTALL_PROGRAM = ${INSTALL} $(INSTALL_STRIP_FLAG) $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = i686-pc-linux
build_triplet = i686-pc-linux-gnu
host_alias = i686-pc-linux
host_triplet = i686-pc-linux-gnu
target_alias = i686-pc-linux
target_triplet = i686-pc-linux-gnu
ARTSCCONFIG = /home/bernd/kde2/bin/artsc-config
AS = @AS@
AUTODIRS = 
CC = gcc
CPP = gcc -E
CXX = g++
CXXCPP = g++ -E
DCOPIDL = /home/bernd/kde2/bin/dcopidl
DCOPIDL2CPP = /home/bernd/kde2/bin/dcopidl2cpp
DCOP_DEPENDENCIES = $(DCOPIDL)
DLLTOOL = @DLLTOOL@
DPMSINC = @DPMSINC@
DPMSLIB = @DPMSLIB@
EXEEXT = 
GCJ = @GCJ@
GCJFLAGS = @GCJFLAGS@
GLINC = @GLINC@
GLLIB = @GLLIB@
GMSGFMT = /usr/bin/msgfmt
GUILE = @GUILE@
GUILE_CFLAGS = @GUILE_CFLAGS@
GUILE_LDFLAGS = @GUILE_LDFLAGS@
IDL = @IDL@
IDL_DEPENDENCIES = @IDL_DEPENDENCIES@
KDB2HTML = /home/bernd/kde2/bin/kdb2html
KDE_CXXFLAGS = 
KDE_EXTRA_RPATH = 
KDE_INCLUDES = -I/home/bernd/kde2/include
KDE_LDFLAGS = -L/home/bernd/kde2/lib
KDE_PLUGIN = -avoid-version -module -no-undefined
KDE_RPATH = -R $(kde_libraries) -R $(qt_libraries) -R $(x_libraries)
KDE_USE_CLOSURE_FALSE = #
KDE_USE_CLOSURE_TRUE = 
KDE_USE_FINAL_FALSE = 
KDE_USE_FINAL_TRUE = #
LIBCOMPAT = 
LIBCRYPT = -lcrypt
LIBDL = -ldl
LIBJPEG = -ljpeg
LIBMICO = @LIBMICO@
LIBOBJS = @LIBOBJS@
LIBPNG = -lpng -lz -lm
LIBPTHREAD = -lpthread
LIBPYTHON = -lpython1.6 -lm -ldl 
LIBQIMGIO = @LIBQIMGIO@
LIBSM = -lSM -lICE
LIBSOCKET = 
LIBTIFF = @LIBTIFF@
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBUCB = 
LIBZ = -lz
LIB_KAB = -lkab
LIB_KDECORE = -lkdecore
LIB_KDEUI = -lkdeui
LIB_KFILE = -lkfile
LIB_KFM = 
LIB_KFORMULA = -lkformula
LIB_KHTML = -lkhtml
LIB_KIMGIO = @LIB_KIMGIO@
LIB_KIO = -lkio
LIB_KPARTS = -lkparts
LIB_KSPELL = -lkspell
LIB_KSYCOCA = -lksycoca
LIB_KWRITE = -lkwrite
LIB_QT = -lqt $(LIBPNG) $(LIBJPEG) -lXext $(LIB_X11) $(LIBSM)
LIB_SMB = -lsmb
LIB_X11 = -lX11 $(LIBSOCKET)
LN_S = ln -s
MAKEINFO = makeinfo
MCOPIDL = /home/bernd/kde2/bin/mcopidl
MICO_INCLUDES = @MICO_INCLUDES@
MICO_LDFLAGS = @MICO_LDFLAGS@
MOC = /home/bernd/kdesrc/qt-copy/bin/moc
MSGFMT = /usr/bin/msgfmt
NOOPT_CXXFLAGS =  -fno-exceptions -fno-check-new -Wall -pedantic -W -Wpointer-arith -Wmissing-prototypes -Wwrite-strings -Wno-long-long -fno-builtin
NOREPO = -fno-repo
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = gideon
PAMINC = @PAMINC@
PAMLIBPATHS = @PAMLIBPATHS@
PAMLIBS = @PAMLIBS@
PYTHONINC = -I/usr/local/python1.6/include/python1.6
PYTHONLIB = -L/usr/local/python1.6/lib/python1.6/config
QT_INCLUDES = -I/home/bernd/kdesrc/qt-copy/include
QT_LDFLAGS = -L/home/bernd/kdesrc/qt-copy/lib
RANLIB = ranlib
REPO = -frepo
STRIP = strip
TOPSUBDIRS =  doc lib parts pics plugins po src
UIC = /home/bernd/kdesrc/qt-copy/bin/uic
USER_INCLUDES = 
USER_LDFLAGS = 
USE_EXCEPTIONS = -fexceptions
USE_RTTI = -frtti
USE_THREADS = @USE_THREADS@
VERSION = 0.1
XGETTEXT = /usr/bin/xgettext
XPMINC = @XPMINC@
XPMLIB = @XPMLIB@
X_EXTRA_LIBS = 
X_INCLUDES = -I/usr/X11R6/include
X_LDFLAGS = -L/usr/X11R6/lib
X_PRE_LIBS = 
all_includes = -I/home/bernd/kde2/include -I/home/bernd/kdesrc/qt-copy/include -I/usr/X11R6/include 
all_libraries = -L/usr/X11R6/lib -L/home/bernd/kdesrc/qt-copy/lib -L/home/bernd/kde2/lib 
idldir = @idldir@
kde_appsdir = ${prefix}/share/applnk
kde_bindir = ${exec_prefix}/bin
kde_confdir = ${prefix}/share/config
kde_datadir = ${prefix}/share/apps
kde_htmldir = ${prefix}/share/doc/HTML
kde_icondir = ${prefix}/share/icons
kde_includes = /home/bernd/kde2/include
kde_libraries = /home/bernd/kde2/lib
kde_locale = ${prefix}/share/locale
kde_mimedir = ${prefix}/share/mimelnk
kde_servicesdir = ${prefix}/share/services
kde_servicetypesdir = ${prefix}/share/servicetypes
kde_sounddir = ${prefix}/share/sounds
kde_templatesdir = ${prefix}/share/templates
kde_wallpaperdir = ${prefix}/share/wallpapers
micodir = @micodir@
qt_includes = /home/bernd/kdesrc/qt-copy/include
qt_libraries = /home/bernd/kdesrc/qt-copy/lib
x_includes = /usr/X11R6/include
x_libraries = /usr/X11R6/lib

SUBDIRS = lib parts plugins src po doc pics

EXTRA_DIST = AUTHORS COPYING NEWS ChangeLog INSTALL README TODO HACKING gideon.lsm admin

apps_DATA = gideon.desktop
appsdir = $(kde_appsdir)/Development

MAINTAINERCLEANFILES = subdirs configure.in acinclude.m4 configure.files 
ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/admin/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES = 
DATA =  $(apps_DATA)

DIST_COMMON =  README ./stamp-h.in AUTHORS COPYING ChangeLog INSTALL \
Makefile.am Makefile.in NEWS TODO acconfig.h acinclude.m4 aclocal.m4 \
config.h.in configure configure.in


#>- DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)
#>+ 4
KDE_DIST=gideonprj Makefile.dist configure.in.in gideon.m4.in gideon.desktop 

DISTFILES= $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST) $(KDE_DIST)


TAR = tar
GZIP_ENV = --best
#>- all: all-redirect
#>+ 1
all: docs-am  all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
#>- 	cd $(top_srcdir) && $(AUTOMAKE) --gnu ./Makefile
#>+ 2
	cd $(top_srcdir) && $(AUTOMAKE) --gnu ./Makefile
	cd $(top_srcdir) && perl admin/am_edit Makefile.in

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in  acinclude.m4
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/config.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) acconfig.h
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f config.h

maintainer-clean-hdr:

install-appsDATA: $(apps_DATA)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(appsdir)
	@list='$(apps_DATA)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(appsdir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(appsdir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(appsdir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(appsdir)/$$p; \
	  fi; fi; \
	done

uninstall-appsDATA:
	@$(NORMAL_UNINSTALL)
	list='$(apps_DATA)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(appsdir)/$$p; \
	done

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.



all-recursive install-data-recursive install-exec-recursive \
installdirs-recursive install-recursive uninstall-recursive  \
check-recursive installcheck-recursive info-recursive dvi-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	rev=''; list='$(SUBDIRS)'; for subdir in $$list; do \
	  rev="$$subdir $$rev"; \
	  test "$$subdir" = "." && dot_seen=yes; \
	done; \
	test "$$dot_seen" = "no" && rev=". $$rev"; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
   if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
   fi; \
	done; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu ./Makefile
#>- 	@for file in $(DISTFILES); do \
#>- 	  d=$(srcdir); \
#>- 	  if test -d $$d/$$file; then \
#>- 	    cp -pr $$/$$file $(distdir)/$$file; \
#>- 	  else \
#>- 	    test -f $(distdir)/$$file \
#>- 	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
#>- 	    || cp -p $$d/$$file $(distdir)/$$file || :; \
#>- 	  fi; \
#>- 	done
#>+ 10
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    chmod 777 $(distdir)/$$subdir; \
	    (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir=../$(distdir) distdir=../$(distdir)/$$subdir distdir) \
	      || exit 1; \
	  fi; \
	done
	$(MAKE) $(AM_MAKEFLAGS) top_distdir="$(top_distdir)" distdir="$(distdir)" dist-hook
info-am:
info: info-recursive
dvi-am:
dvi: dvi-recursive
check-am: all-am
check: check-recursive
installcheck-am:
installcheck: installcheck-recursive
all-recursive-am: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am:
install-exec: install-exec-recursive

install-data-am: install-appsDATA
install-data: install-data-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-recursive
uninstall-am: uninstall-appsDATA
uninstall: uninstall-recursive
all-am: Makefile $(DATA) config.h
all-redirect: all-recursive-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs)  $(DESTDIR)$(appsdir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
	-test -z "$(MAINTAINERCLEANFILES)" || rm -f $(MAINTAINERCLEANFILES)
mostlyclean-am:  mostlyclean-hdr mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-recursive

clean-am:  clean-hdr clean-tags clean-generic mostlyclean-am

#>- clean: clean-recursive
#>+ 1
clean: kde-rpo-clean  clean-recursive

distclean-am:  distclean-hdr distclean-tags distclean-generic clean-am
	-rm -f libtool

distclean: distclean-recursive
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-recursive
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
uninstall-appsDATA install-appsDATA install-data-recursive \
uninstall-data-recursive install-exec-recursive \
uninstall-exec-recursive installdirs-recursive uninstalldirs-recursive \
all-recursive check-recursive installcheck-recursive info-recursive \
dvi-recursive mostlyclean-recursive distclean-recursive clean-recursive \
maintainer-clean-recursive tags tags-recursive mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir info-am info \
dvi-am dvi check check-am installcheck-am installcheck all-recursive-am \
install-exec-am install-exec install-data-am install-data install-am \
install uninstall-am uninstall all-redirect all-am all installdirs-am \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


$(top_srcdir)/configure.in: configure.in.in $(top_srcdir)/subdirs
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common configure.in

$(top_srcdir)/subdirs:
	cd $(top_srcdir) && $(MAKE) -f admin/Makefile.common subdirs

$(top_srcdir)/acinclude.m4: $(top_srcdir)/admin/acinclude.m4.in $(top_srcdir)/admin/libtool.m4.in $(top_srcdir)/gideon.m4.in
	@cd $(top_srcdir) && cat admin/acinclude.m4.in admin/libtool.m4.in gideon.m4.in > acinclude.m4

package-messages:
	$(MAKE) -f admin/Makefile.common package-messages
	$(MAKE) -C po merge

dist-hook:
	cd $(top_distdir) && perl admin/am_edit -padmin
	cd $(top_distdir) && $(MAKE) -f admin/Makefile.common subdirs

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:

#>+ 2
docs-am:

#>+ 5
force-reedit:
		cd $(top_srcdir) && $(AUTOMAKE) --gnu ./Makefile
	cd $(top_srcdir) && perl admin/am_edit Makefile.in


#>+ 2
final:
	$(MAKE) all-am
#>+ 2
no-final:
	$(MAKE) all-am
#>+ 3
cvs-clean:
	$(MAKE) -f $(top_srcdir)/admin/Makefile.common cvs-clean

#>+ 3
kde-rpo-clean:
	-rm -f *.rpo
