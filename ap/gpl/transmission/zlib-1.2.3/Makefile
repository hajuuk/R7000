# Makefile for zlib
# Copyright (C) 1995-2005 Jean-loup Gailly.
# For conditions of distribution and use, see copyright notice in zlib.h

# To compile and test, type:
#    ./configure; make test
# The call of configure is optional if you don't have special requirements
# If you wish to build zlib as a shared library, use: ./configure -s

# To use the asm code, type:
#    cp contrib/asm?86/match.S ./match.S
#    make LOC=-DASMV OBJA=match.o

# To install /usr/local/lib/libz.* and /usr/local/include/zlib.h, type:
#    make install
# To install in $HOME instead of /usr/local, use:
#    make install prefix=$HOME

CC=arm-uclibc-linux-2.6.36-gcc

CFLAGS=-Os -O2 -DBCMNVRAM -DBCMWPA2 -DRESTART_ALL_PROCESSES -D__CONFIG_USBAP__ -DBCMQOS -DBCMWPS -D__CONFIG_EMF__ -DTRAFFIC_MGMT_RSSI_POLICY -DPHYMON -DSUPPORT_REMOTE_HTTPS -DINCLUDE_WPS_V20 -DINCLUDE_5G_AUTO_CHANNEL -DOPENDNS_PARENTAL_CONTROL -DINCLUDE_ACCESSCONTROL -DINCLUDE_QUICK_QOS -DINCLUDE_UCP -DU12H270 -DR7000 -DMULTIPLE_SSID -DENABLE_ML -DOPEN_SOURCE_SUPPORT -DBCM53125 -DBCM5301X -DACS_INTF_DETECT_PATCH -DCONFIG_RUSSIA_IPTV -DVIDEO_STREAMING_QOS -DFBWIFI_FLAG -DDLNA -DHTTP_ACCESS_USB -DMAX_USB_ACCESS -DSAMBA_ENABLE -DUSB_NEW_SPEC -DINCLUDE_WIFI_BUTTON -DINCLUDE_USB_LED -DINCLUDE_DUAL_BAND -DSINGLE_FIRMWARE -DINCLUDE_GET_ST_CHKSUM -DUNIFIED_STR_TBL -DFIRST_MTD_ROTATION -DWIFI_ON_OFF_SCHE -DAUTO_CONN_24HR -DIGMP_PROXY -DAP_MODE -D__CONFIG_IGMP_SNOOPING__ -DLINUX26 -DINCLUDE_IPV6 -DPRESET_WL_SECURITY -DNEW_BCM_WPS_IPC -DSUPPORT_AC_MODE -DSTA_MODE -DPPP_RU_DESIGN -DEXT_ACS -DARP_PROTECTION -DBCM_DCS -DEXT_ACS -Os -DLINUX26 -DCONFIG_KERNEL_2_6_36 -s -Os -fPIC -I/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/transmission/openssl/include -I/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/transmission/zlib-1.2.3 -I/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/transmission/../curl-7.23.1/source/include -I/home/edward/GPL/V1.0.3.28_1.1.22PRRU/ap/gpl/transmission/libevent-2.0.20-stable/include -DUSE_MMAP
#CFLAGS=-O -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7
#CFLAGS=-g -DDEBUG
#CFLAGS=-O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
#           -Wstrict-prototypes -Wmissing-prototypes

LDFLAGS=-L. libz.a
LDSHARED=arm-uclibc-linux-2.6.36-gcc
CPP=arm-uclibc-linux-2.6.36-gcc -E

LIBS=libz.a
SHAREDLIB=libz.so
SHAREDLIBV=libz.so.1.2.3
SHAREDLIBM=libz.so.1

AR=arm-uclibc-linux-2.6.36-ar cru
RANLIB=arm-uclibc-linux-2.6.36-ranlib
TAR=tar
SHELL=/bin/sh
EXE=

prefix =/usr/local
exec_prefix =${prefix}
libdir =${exec_prefix}/lib
includedir =${prefix}/include
mandir =${prefix}/share/man
man3dir = ${mandir}/man3

OBJS = adler32.o compress.o crc32.o gzio.o uncompr.o deflate.o trees.o \
       zutil.o inflate.o infback.o inftrees.o inffast.o

OBJA =
# to use the asm code: make OBJA=match.o

TEST_OBJS = example.o minigzip.o

all: example$(EXE) minigzip$(EXE)

check: test
test: all
	@LD_LIBRARY_PATH=.:$(LD_LIBRARY_PATH) ; export LD_LIBRARY_PATH; \
	echo hello world | ./minigzip | ./minigzip -d || \
	  echo '		*** minigzip test FAILED ***' ; \
	if ./example; then \
	  echo '		*** zlib test OK ***'; \
	else \
	  echo '		*** zlib test FAILED ***'; \
	fi

libz.a: $(OBJS) $(OBJA)
	$(AR) $@ $(OBJS) $(OBJA)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

match.o: match.S
	$(CPP) match.S > _match.s
	$(CC) -c _match.s
	mv _match.o match.o
	rm -f _match.s

$(SHAREDLIBV): $(OBJS)
	$(LDSHARED) -o $@ $(OBJS)
	rm -f $(SHAREDLIB) $(SHAREDLIBM)
	ln -s $@ $(SHAREDLIB)
	ln -s $@ $(SHAREDLIBM)

example$(EXE): example.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ example.o $(LDFLAGS)

minigzip$(EXE): minigzip.o $(LIBS)
	$(CC) $(CFLAGS) -o $@ minigzip.o $(LDFLAGS)

install: $(LIBS)
	-@if [ ! -d $(exec_prefix) ]; then mkdir -p $(exec_prefix); fi
	-@if [ ! -d $(includedir)  ]; then mkdir -p $(includedir); fi
	-@if [ ! -d $(libdir)      ]; then mkdir -p $(libdir); fi
	-@if [ ! -d $(man3dir)     ]; then mkdir -p $(man3dir); fi
	cp zlib.h zconf.h $(includedir)
	chmod 644 $(includedir)/zlib.h $(includedir)/zconf.h
	cp $(LIBS) $(libdir)
	cd $(libdir); chmod 755 $(LIBS)
	-@(cd $(libdir); $(RANLIB) libz.a || true) >/dev/null 2>&1
	cd $(libdir); if test -f $(SHAREDLIBV); then \
	  rm -f $(SHAREDLIB) $(SHAREDLIBM); \
	  ln -s $(SHAREDLIBV) $(SHAREDLIB); \
	  ln -s $(SHAREDLIBV) $(SHAREDLIBM); \
	  (ldconfig || true)  >/dev/null 2>&1; \
	fi
	cp zlib.3 $(man3dir)
	chmod 644 $(man3dir)/zlib.3
# The ranlib in install is needed on NeXTSTEP which checks file times
# ldconfig is for Linux

uninstall:
	cd $(includedir); \
	cd $(libdir); rm -f libz.a; \
	if test -f $(SHAREDLIBV); then \
	  rm -f $(SHAREDLIBV) $(SHAREDLIB) $(SHAREDLIBM); \
	fi
	cd $(man3dir); rm -f zlib.3

mostlyclean: clean
clean:
	rm -f *.o *~ example$(EXE) minigzip$(EXE) \
	   libz.* foo.gz so_locations \
	   _match.s maketree contrib/infback9/*.o

maintainer-clean: distclean
distclean: clean
	cp -p Makefile.in Makefile
	cp -p zconf.in.h zconf.h
	rm -f .DS_Store

tags:
	etags *.[ch]

depend:
	makedepend -- $(CFLAGS) -- *.[ch]

# DO NOT DELETE THIS LINE -- make depend depends on it.

adler32.o: zlib.h zconf.h
compress.o: zlib.h zconf.h
crc32.o: crc32.h zlib.h zconf.h
deflate.o: deflate.h zutil.h zlib.h zconf.h
example.o: zlib.h zconf.h
gzio.o: zutil.h zlib.h zconf.h
inffast.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inflate.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
infback.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inftrees.o: zutil.h zlib.h zconf.h inftrees.h
minigzip.o: zlib.h zconf.h
trees.o: deflate.h zutil.h zlib.h zconf.h trees.h
uncompr.o: zlib.h zconf.h
zutil.o: zutil.h zlib.h zconf.h
