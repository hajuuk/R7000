export CROSS_COMPILE=arm-uclibc-linux-2.6.36-
export CC=arm-uclibc-linux-2.6.36-gcc
export CXX=arm-uclibc-linux-2.6.36-g++
export LD_LIBRARY_PATH=$(TOOLCHAIN)/lib:$(TOOLCHAIN)/usr/lib:$(TARGETDIR)/lib:$(TARGETDIR)/usr/lib
export PPTPD=pptpd-1.3.4

all: .conf  makepptpd  install
.conf:
	cd $(PPTPD) ; \
	./configure --target=arm-linux --host=arm-linux --build=i686 --prefix=$(TARGETDIR) CC=$(CC) CXX=$(CXX); 

makepptpd:
	cd $(PPTPD) ; make

clean: .conf cleangsoap
	
cleangsoap: 
	cd $(PPTPD) ; make clean	

distclean:
	cd $(PPTPD) ; make clean ; make distclean

install:
	cd $(PPTPD) ; make install

make:
	cd $(PPTPD) ; make
