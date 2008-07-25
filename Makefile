WVSTREAMS=.
WVSTREAMS_SRC= # Clear WVSTREAMS_SRC so wvrules.mk uses its WVSTREAMS_foo
VPATH=$(libdir)
include wvrules.mk

qt/wvqtstreamclone.o: include/wvqtstreamclone.moc
qt/wvqthook.o: include/wvqthook.moc

libwvqt.so-LIBS: $(LIBS_QT)

ifneq ("$(with_qt)", "no")
TESTS+=$(patsubst %.cc,%,$(wildcard qt/tests/*.cc))
endif

qt/tests/qtstringtest: libwvqt.a
qt/tests/%: LDLIBS+=libwvqt.a
qt/tests/%: LDLIBS+=-lqt-mt
# qt/tests/%: CPPFLAGS+=-I/usr/include/qt


CXXFLAGS+=-DWVSTREAMS_RELEASE=\"$(PACKAGE_VERSION)\"
DISTCLEAN+=uniconf/daemon/uniconfd.8

libuniconf.so libuniconf.a: \
	$(filter-out uniconf/daemon/uniconfd.o, \
	     $(call objects,uniconf/daemon))

ifeq ($(EXEEXT),.exe)
uniconf/daemon/uniconfd: uniconf/daemon/uniconfd.o libwvwin32.a
else
uniconf/daemon/uniconfd: uniconf/daemon/uniconfd.o $(LIBUNICONF)
endif

%: %.in
	@sed -e "s/#VERSION#/$(PACKAGE_VERSION)/g" < $< > $@

%: %.in
	@sed -e 's/#VERSION#/$(PACKAGE_VERSION)/g' < $< > $@

DISTCLEAN+=uniconf/tests/uni


CPPFLAGS += -Iinclude -pipe
ARFLAGS = rs

DEBUG:=$(filter-out no,$(enable_debug))

# for O_LARGEFILE
CXXFLAGS+=${CXXOPTS}
CFLAGS+=${COPTS}
CXXFLAGS+=-D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS+=-D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

ifneq ($(DEBUG),)
CXXFLAGS+=-ggdb -DDEBUG$(if $(filter-out yes,$(DEBUG)), -DDEBUG_$(DEBUG))
CFLAGS+=-ggdb -DDEBUG$(if $(filter-out yes,$(DEBUG)), -DDEBUG_$(DEBUG))
endif

ifeq ("$(enable_debug)", "no")
#CXXFLAGS+=-fomit-frame-pointer
# -DNDEBUG is disabled because we like assert() to crash
#CXXFLAGS+=-DNDEBUG
#CFLAGS+=-DNDEBUG
endif

ifeq ("$(enable_fatal_warnings)", "yes")
CXXFLAGS+=-Werror
# FIXME: not for C, because our only C file, crypto/wvsslhack.c, has
#        a few warnings.
#CFLAGS+=-Werror
endif

ifneq ("$(enable_optimization)", "no")
CXXFLAGS+=-O2
#CXXFLAGS+=-felide-constructors
CFLAGS+=-O2
endif

ifneq ("$(enable_warnings)", "no")
CXXFLAGS+=-Wall -Woverloaded-virtual
CFLAGS+=-Wall
endif

ifeq ("$(enable_testgui)", "no")
WVTESTRUN=env
endif

ifeq ("$(enable_efence)", "yes")
LDLIBS+=-lefence
endif

libwvbase.so-LIBS+=-lxplc-cxx -lm
libwvbase.so:

ifneq ("$(with_pam)", "no")
  libwvstreams.so: -lpam
endif

DEBUG:=$(filter-out no,$(enable_debug))

# debugging function
showvar = @echo \"'$(1)'\" =\> \"'$($(1))'\"
tbd = $(error "$@" not implemented yet)

# initialization
TARGETS:=
GARBAGE:=
DISTCLEAN:=
REALCLEAN:=
TESTS:=
NO_CONFIGURE_TARGETS:=

NO_CONFIGURE_TARGETS+=clean ChangeLog depend dust configure dist \
		distclean realclean

TARGETS += libwvbase.so libwvbase.a
TARGETS += libwvutils.so libwvutils.a
TARGETS += libwvstreams.so libwvstreams.a
TARGETS += libuniconf.so libuniconf.a
TARGETS += wvtestmain.o libwvtest.a
TARGETS += uniconf/daemon/uniconfd uniconf/tests/uni
TARGETS += crypto/tests/ssltest ipstreams/tests/unixtest
TARGETS += crypto/tests/printcert
ifneq ("$(with_dbus)", "no")
TARGETS += dbus/tests/wvdbus dbus/tests/wvdbusd
endif
ifneq ("$(with_readline)", "no")
TARGETS += ipstreams/tests/wsd
endif
GARBAGE += wvtestmain.o tmp.ini .wvtest-total

ifneq ("$(with_qt)", "no")
  TARGETS += libwvqt.so libwvqt.a
endif

ifneq ("$(with_dbus)", "no")
  TARGETS += libwvdbus.so libwvdbus.a
endif

TARGETS_SO := $(filter %.so,$(TARGETS))
TARGETS_A := $(filter %.a,$(TARGETS))

GARBAGE += $(wildcard lib*.so.*)

DISTCLEAN += autom4te.cache config.mk config.log config.status \
		include/wvautoconf.h config.cache reconfigure

REALCLEAN += stamp-h.in configure include/wvautoconf.h.in

CPPFLAGS += -Iinclude -pipe
ARFLAGS = rs
RELEASE?=$(PACKAGE_VERSION)

DEBUG:=$(filter-out no,$(enable_debug))

CXXFLAGS+=$(if $(filter-out yes,$(DEBUG)), -DDEBUG_$(DEBUG))
CFLAGS+=$(if $(filter-out yes,$(DEBUG)), -DDEBUG_$(DEBUG))

ifeq ("$(enable_fatal_warnings)", "yes")
CXXFLAGS+=-Werror
# FIXME: not for C, because our only C file, crypto/wvsslhack.c, has
#        a few warnings.
#CFLAGS+=-Werror
endif

ifeq ("$(enable_testgui)", "no")
WVTESTRUN=env
endif

ifneq ("$(with_xplc)", "no")
LIBS+=$(LIBS_XPLC) -lm
endif

libwvutils.so-LIBS+=$(LIBS_PAM)

BASEOBJS= \
	utils/wvbuffer.o utils/wvbufferstore.o \
	utils/wvcont.o \
	utils/wverror.o \
	streams/wvfdstream.o \
	utils/wvfork.o \
	utils/wvhash.o \
	utils/wvhashtable.o \
	utils/wvlinklist.o \
	utils/wvmoniker.o \
	utils/wvregex.o \
	utils/wvscatterhash.o utils/wvsorter.o \
	utils/wvstring.o utils/wvstringlist.o \
	utils/wvstringmask.o \
	utils/strutils.o \
	utils/wvtask.o \
	utils/wvtimeutils.o \
	streams/wvistreamlist.o \
	utils/wvstreamsdebugger.o \
	streams/wvlog.o \
	streams/wvstream.o \
	uniconf/uniconf.o \
	uniconf/uniconfgen.o uniconf/uniconfkey.o uniconf/uniconfroot.o \
	uniconf/unihashtree.o \
	uniconf/unimountgen.o \
	uniconf/unitempgen.o \
	utils/wvbackslash.o \
	utils/wvencoder.o \
	utils/wvtclstring.o \
	utils/wvstringcache.o \
	uniconf/uniinigen.o \
	uniconf/unigenhack.o \
	uniconf/unilistiter.o \
	streams/wvfile.o \
	streams/wvstreamclone.o  \
	streams/wvconstream.o \
	utils/wvcrashbase.o

TESTOBJS = utils/wvtest.o 

# print the sizes of all object files making up libwvbase, to help find
# optimization targets.
basesize:
	size --total $(BASEOBJS)

micro: micro.o libwvbase.so

libwvbase.a libwvbase.so: $(filter-out uniconf/unigenhack.o,$(BASEOBJS))
libwvbase.a: uniconf/unigenhack_s.o
libwvbase.so: uniconf/unigenhack.o
libwvbase.so: LIBS+=$(LIBXPLC)

libwvutils.a libwvutils.so: $(filter-out $(BASEOBJS) $(TESTOBJS),$(call objects,utils))
libwvutils.so: libwvbase.so
libwvutils.so: -lz -lcrypt

libwvstreams.a libwvstreams.so: $(filter-out $(BASEOBJS), \
	$(call objects,configfile crypto ipstreams \
		$(ARCH_SUBDIRS) streams urlget))
libwvstreams.so: libwvutils.so libwvbase.so
libwvstreams.so: LIBS+=-lz -lssl -lcrypto 

libuniconf.a libuniconf.so: $(filter-out $(BASEOBJS), \
	$(call objects,uniconf))
libuniconf.a: uniconf/uniconfroot.o
libuniconf.so: libwvstreams.so libwvutils.so libwvbase.so

libwvdbus.a libwvdbus.so: $(call objects,dbus)
libwvdbus.so: libwvstreams.so libwvutils.so libwvbase.so
libwvdbus.so: LIBS+=$(LIBS_DBUS)

libwvtest.a: wvtestmain.o $(TESTOBJS)

ifeq ("$(wildcard /usr/lib/libqt-mt.so)", "/usr/lib/libqt-mt.so")
  libwvqt.so-LIBS+=-lqt-mt
else 
  # RedHat has a pkgconfig file we can use to sort out this mess..
  ifeq ("$(wildcard /usr/lib/pkgconfig/qt-mt.pc)", "/usr/lib/pkgconfig/qt-mt.pc")
    libwvqt.so-LIBS+=`pkg-config --libs qt-mt`
  else
    libwvqt.so-LIBS+=-lqt
  endif
endif
libwvqt.a libwvqt.so: $(call objects,qt)
libwvqt.so: libwvutils.so libwvstreams.so libwvbase.so

libwvgtk.a libwvgtk.so: $(call objects,gtk)
libwvgtk.so: -lgtk -lgdk libwvstreams.so libwvutils.so libwvbase.so
override enable_efence=no

ifneq (${_WIN32},)
  $(error "Use 'make -f Makefile-win32' instead!")
endif

export WVSTREAMS

XPATH=include

SUBDIRS =

all: runconfigure $(TARGETS)

.PHONY: clean depend dust kdoc doxygen install install-shared install-dev uninstall tests dishes dist distclean realclean test

# FIXME: little trick to ensure that the wvautoconf.h.in file is there
.PHONY: dist-hack-clean
dist-hack-clean:
	@rm -f stamp-h.in

export AM_CFLAGS
AM_CFLAGS=-fPIC

# Comment this assignment out for a release.
ifdef PKGSNAPSHOT
SNAPDATE=+$(shell date +%Y%m%d)
endif

dist-hook: dist-hack-clean configure
	@rm -rf autom4te.cache

runconfigure: config.mk include/wvautoconf.h

ifndef CONFIGURING
configure=$(error Please run the "configure" script)
else
configure:=
endif

config.mk: configure config.mk.in
	$(call configure)

include/wvautoconf.h: include/wvautoconf.h.in
	$(call configure)

# FIXME: there is some confusion here
ifdef WE_ARE_DIST
aclocal.m4: acinclude.m4
	$(warning "$@" is old, please run "aclocal")

configure: configure.ac config.mk.in include/wvautoconf.h.in aclocal.m4
	$(warning "$@" is old, please run "autoconf")

include/wvautoconf.h.in: configure.ac aclocal.m4
	$(warning "$@" is old, please run "autoheader")
else
aclocal.m4: acinclude.m4
	aclocal
	@touch $@

configure: configure.ac include/wvautoconf.h.in aclocal.m4
	autoconf
	@rm -f config.mk include/wvautoconf.h
	@touch $@

include/wvautoconf.h.in: configure.ac aclocal.m4
	autoheader
	@touch $@
endif

ifeq ($(VERBOSE),)
define wild_clean
	@list=`echo $(wildcard $(1))`; \
		test -z "$${list}" || sh -c "rm -rf $${list}"
endef
else
define wild_clean
	@list=`echo $(wildcard $(1))`; \
		test -z "$${list}" || sh -cx "rm -rf $${list}"
endef
endif

realclean: distclean
	$(call wild_clean,$(REALCLEAN))


distclean: clean
	$(call wild_clean,$(DISTCLEAN))
	@rm -rf autom4te.cache
	@rm -f pkgconfig/*.pc

clean: depend dust
	$(subdirs)
	$(call wild_clean,$(TARGETS) uniconf/daemon/uniconfd \
		$(GARBAGE) $(TESTS) tmp.ini \
		$(shell find . -name '*.o' -o -name '*.moc'))

depend:
	$(call wild_clean,$(shell find . -name '.*.d'))

dust:
	$(call wild_clean,$(shell find . -name 'core' -o -name '*~' -o -name '.#*') $(wildcard *.d))

kdoc:
	kdoc -f html -d Docs/kdoc-html --name wvstreams --strip-h-path */*.h

doxygen:
	doxygen

uniconfd: uniconf/daemon/uniconfd uniconf/daemon/uniconfd.ini \
          uniconf/daemon/uniconfd.8

