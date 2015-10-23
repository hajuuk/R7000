################################################################
# Process this file with top-level configure script to produce Makefile
#
# Copyright 2000 Clark Cooper
#
#  This file is part of EXPAT.
#
#  EXPAT is free software; you can redistribute it and/or modify it
#  under the terms of the License (based on the MIT/X license) contained
#  in the file COPYING that comes with this distribution.
#
# EXPAT IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN EXPAT.
#

SHELL = /bin/sh

srcdir = .
top_srcdir = .


prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
mandir = ${prefix}/man/man1

top_builddir = .


INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
mkinstalldirs = $(SHELL) $(top_srcdir)/conftools/mkinstalldirs

MANFILE = $(srcdir)/doc/xmlwf.1
APIHEADER = $(srcdir)/lib/expat.h
LIBRARY = libexpat.la


default:  buildlib xmlwf/xmlwf

buildlib: $(LIBRARY)

all: $(LIBRARY) xmlwf/xmlwf examples/elements examples/outline

clean:
	cd lib && rm -f $(LIBRARY) *.o *.lo && rm -rf .libs _libs
	cd xmlwf && rm -f xmlwf *.o *.lo && rm -rf .libs _libs
	cd examples && rm -f elements outline *.o *.lo && rm -rf .libs _libs
	cd tests && rm -rf .libs runtests runtests.o chardata.o
	rm -rf .libs libexpat.la
	rm -f examples/core tests/core xmlwf/core

clobber: clean

distclean: clean
	rm -f expat_config.h config.status config.log config.cache libtool
	rm -f Makefile

extraclean: distclean
	rm -f expat_config.h.in configure
	rm -f conftools/ltconfig conftools/ltmain.sh conftools/libtool.m4

check: tests/runtests
	tests/runtests

install: xmlwf/xmlwf installlib
	$(mkinstalldirs) $(bindir) $(mandir)
	$(LIBTOOL) --mode=install $(INSTALL_PROGRAM) xmlwf/xmlwf $(bindir)/xmlwf
	$(INSTALL_DATA) $(MANFILE) $(mandir)

installlib: $(LIBRARY) $(APIHEADER)
	$(mkinstalldirs) $(libdir) $(includedir)
	$(LIBTOOL) --mode=install $(INSTALL) $(LIBRARY) $(libdir)/$(LIBRARY)
	$(INSTALL_DATA) $(APIHEADER) $(includedir)

uninstall: uninstalllib
	$(LIBTOOL) --mode=uninstall rm -f $(bindir)/xmlwf
	rm -f $(mandir)/xmlwf.1

uninstalllib:
	$(LIBTOOL) --mode=uninstall rm -f $(libdir)/$(LIBRARY)
	rm -f $(includedir)/$(APIHEADER)

# for VPATH builds (invoked by configure)
mkdir-init:
	@for d in lib xmlwf examples tests ; do \
		(mkdir $$d 2> /dev/null || test 1) ; \
	done

CC = arm-uclibc-linux-2.6.36-gcc
LIBTOOL = $(SHELL) $(top_builddir)/libtool

INCLUDES = -I$(srcdir)/lib -I.
LDFLAGS = -L/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/amule/wxWidgets-2.8.12/lib -L/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/amule/libcryptoxx-5.6.0
CPPFLAGS = 
CFLAGS = -Os -O2 -DBCMNVRAM -DBCMWPA2 -DRESTART_ALL_PROCESSES -D__CONFIG_USBAP__ -DBCMQOS -DBCMWPS -D__CONFIG_EMF__ -DTRAFFIC_MGMT_RSSI_POLICY -DPHYMON -DSUPPORT_REMOTE_HTTPS -DINCLUDE_WPS_V20 -DINCLUDE_5G_AUTO_CHANNEL -DOPENDNS_PARENTAL_CONTROL -DINCLUDE_ACCESSCONTROL -DINCLUDE_QUICK_QOS -DINCLUDE_UCP -DU12H270 -DR7000 -DMULTIPLE_SSID -DENABLE_ML -DOPEN_SOURCE_SUPPORT -DBCM53125 -DBCM5301X -DACS_INTF_DETECT_PATCH -DCONFIG_RUSSIA_IPTV -DVIDEO_STREAMING_QOS -DFBWIFI_FLAG -DDLNA -DHTTP_ACCESS_USB -DMAX_USB_ACCESS -DSAMBA_ENABLE -DUSB_NEW_SPEC -DINCLUDE_WIFI_BUTTON -DINCLUDE_USB_LED -DINCLUDE_DUAL_BAND -DSINGLE_FIRMWARE -DINCLUDE_GET_ST_CHKSUM -DUNIFIED_STR_TBL -DFIRST_MTD_ROTATION -DWIFI_ON_OFF_SCHE -DAUTO_CONN_24HR -DIGMP_PROXY -DAP_MODE -D__CONFIG_IGMP_SNOOPING__ -DLINUX26 -DINCLUDE_IPV6 -DPRESET_WL_SECURITY -DNEW_BCM_WPS_IPC -DSUPPORT_AC_MODE -DSTA_MODE -DPPP_RU_DESIGN -DEXT_ACS -DARP_PROTECTION -DBCM_DCS -DEXT_ACS -Os -DLINUX26 -DCONFIG_KERNEL_2_6_36 -s -Os -fPIC -I/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/amule/wxWidgets-2.8.12/include -Wall -Wmissing-prototypes -Wstrict-prototypes -fexceptions
VSNFLAG = -version-info 4:0:4

### autoconf this?
LTFLAGS = --silent

COMPILE = $(CC) $(CFLAGS) $(DEFS) $(CPPFLAGS) $(INCLUDES)
LTCOMPILE = $(LIBTOOL) $(LTFLAGS) --mode=compile $(COMPILE)
LINK_LIB = $(LIBTOOL) $(LTFLAGS) --mode=link $(COMPILE) -no-undefined $(VSNFLAG) -rpath $(libdir) $(LDFLAGS) -o $@
LINK_EXE = $(LIBTOOL) $(LTFLAGS) --mode=link $(COMPILE) $(LDFLAGS) -o $@

LIB_OBJS = lib/xmlparse.lo lib/xmltok.lo lib/xmlrole.lo
$(LIBRARY): $(LIB_OBJS)
	$(LINK_LIB) $(LIB_OBJS)

lib/xmlparse.lo: lib/xmlparse.c lib/expat.h lib/xmlrole.h lib/xmltok.h \
	$(top_builddir)/expat_config.h lib/internal.h

lib/xmlrole.lo: lib/xmlrole.c lib/ascii.h lib/xmlrole.h \
	$(top_builddir)/expat_config.h lib/internal.h

lib/xmltok.lo: lib/xmltok.c lib/xmltok_impl.c lib/xmltok_ns.c \
	lib/ascii.h lib/asciitab.h lib/iasciitab.h lib/latin1tab.h \
	lib/nametab.h lib/utf8tab.h lib/xmltok.h lib/xmltok_impl.h \
	$(top_builddir)/expat_config.h


XMLWF_OBJS = xmlwf/xmlwf.o xmlwf/xmlfile.o xmlwf/codepage.o xmlwf/readfilemap.o
xmlwf/xmlwf.o: xmlwf/xmlwf.c
xmlwf/xmlfile.o: xmlwf/xmlfile.c
xmlwf/codepage.o: xmlwf/codepage.c
xmlwf/readfilemap.o: xmlwf/readfilemap.c
xmlwf/xmlwf: $(XMLWF_OBJS) $(LIBRARY)
	$(LINK_EXE) $(XMLWF_OBJS) $(LIBRARY)

examples/elements.o: examples/elements.c
examples/elements: examples/elements.o $(LIBRARY)
	$(LINK_EXE) $< $(LIBRARY)

examples/outline.o: examples/outline.c
examples/outline: examples/outline.o $(LIBRARY)
	$(LINK_EXE) $< $(LIBRARY)

tests/chardata.o: tests/chardata.c tests/chardata.h
tests/runtests.o: tests/runtests.c tests/chardata.h
tests/runtests: tests/runtests.o tests/chardata.o $(LIBRARY)
	$(LINK_EXE) $^ -lcheck

tests/xmlts.zip:
	wget --output-document=tests/xmlts.zip \
		http://www.w3.org/XML/Test/xmlts20020606.zip

tests/XML-Test-Suite: tests/xmlts.zip
	cd tests && unzip -q xmlts.zip

run-xmltest: xmlwf/xmlwf tests/XML-Test-Suite
	tests/xmltest.sh

.SUFFIXES: .c .lo .o

.c.o:
	$(COMPILE) -o $@ -c $<
.c.lo:
	$(LTCOMPILE) -o $@ -c $<

.PHONY: buildlib all \
	clean distclean extraclean maintainer-clean \
	dist distdir \
	install uninstall
