all: ftp

include ../../config.mk
#include ../MCONFIG
include ../MRULES

CFLAGS+=-O2 -Wall -W -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wnested-externs -Winline 
LIBTERMCAP=-lncurses
USE_GLIBC=1

ifeq ($(USE_READLINE),1)
CFLAGS += -D__USE_READLINE__
LIBS += -lreadline $(LIBTERMCAP)
endif

ftp: cmds.o cmdtab.o domacro.o ftp.o glob.o main.o ruserpass.o
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

domacro.o ftp.o glob.o main.o ruserpass.o: ftp_var.h pathnames.h
cmds.o cmdtab.o: ftp_var.h pathnames.h cmds.h
cmds.o glob.o: glob.h

install: ftp
	install -d $(TARGETDIR)/usr/sbin
	install -m 755 ftp $(TARGETDIR)/usr/sbin
	$(STRIP) $(TARGETDIR)/usr/sbin/ftp

clean:
	rm -f *.o ftp

