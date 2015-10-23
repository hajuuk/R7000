

export CROSS_COMPILE = "arm-uclibc-linux-2.6.36-"

#export PATH +=/projects/hnd/tools/linux/hndtools-mipsel-linux-uclibc-4.2.3/bin


#export CC = "mipsel-uclibc-linux26-gcc"
##export AR = "mipsel-uclibc-linux26-ar"
#export AS = "mipsel-uclibc-linux26-as"
#export LD = "mipsel-uclibc-linux26-ld"
#export NM = "mipsel-uclibc-linux26-nm"
#export RANLIB = "mipsel-uclibc-linux26-ranlib"


all: .conf  makencftp
.conf:
	cd source ; \
	./configure 	

	touch .conf
makencftp:
	cd source ; make

clean: .conf cleanncftp
	
cleanncftp: 
	cd source ; make clean	

distclean:
	rm -rf .conf
	cd source ; make clean

install:
	install -d $(TARGETDIR)/sbin/
	install -m 755 $(TOP)/../../ap/gpl/ncftp-3.2.5/source/bin/ncftpput  $(TARGETDIR)/sbin
	$(STRIP) $(TARGETDIR)/sbin/ncftpput
	
