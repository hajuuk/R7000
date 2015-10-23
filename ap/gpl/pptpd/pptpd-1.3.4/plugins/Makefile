CC	= arm-uclibc-linux-2.6.36-gcc
COPTS	= -O2 -g
CFLAGS	= $(COPTS) -I.. -I../../include -fPIC
LDFLAGS	= -shared
LDADD   = -lutil
#INSTALL	= install -o root
INSTALL	= install 
prefix  = /usr/local

PLUGINS = pptpd-logwtmp.so
LIBUTIL = libutil.so.0

# include dependencies if present
ifeq (.depend,$(wildcard .depend))
include .depend
endif

all:	$(PLUGINS)

%.so: %.c
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) $^ $(LDADD)

LIBDIR	?= $(DESTDIR)$(prefix)/lib/pptpd

install: $(PLUGINS)
	$(INSTALL) -d $(LIBDIR)
	$(INSTALL) $? $(LIBDIR)
	$(INSTALL) $(LIBUTIL) $(LIBDIR)/../

uninstall:
	rm -f $(LIBDIR)$(PLUGINS)

clean:
	rm -f *.o *.so *.a

depend:
	$(CPP) -M $(CFLAGS) *.c >.depend
