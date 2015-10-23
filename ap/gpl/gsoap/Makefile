export CROSS_COMPILE = "arm-uclibc-linux-2.6.36-"
export CC=arm-uclibc-linux-2.6.36-gcc
export CXX=arm-uclibc-linux-2.6.36-g++
export ac_cv_func_malloc_0_nonnull=yes
export LD_LIBRARY_PATH=$(TOOLCHAIN)/lib:$(TOOLCHAIN)/usr/lib:$(TARGETDIR)/lib:$(TARGETDIR)/usr/lib
#echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH
#export CPPFLAGS += -I$(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/include -I$(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/include/openssl
#export LDFLAGS += -L$(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/lib
#export PKG_CONFIG_PATH += $(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/lib/pkgconfig

all: .conf  makegsoap  install
.conf:
	cd source ; \
	TOOLCHAIN=/projects/hnd/tools/linux/hndtools-arm-linux-2.6.36-uclibc-4.5.3 \
	CROSS_COMPILE="arm-uclibc-linux-2.6.36-" \
	CC=arm-uclibc-linux-2.6.36-gcc \
	CXX=arm-uclibc-linux-2.6.36-g++ \
	ac_cv_func_malloc_0_nonnull=yes \
	LD_LIBRARY_PATH=$(TOOLCHAIN)/lib:$(TOOLCHAIN)/usr/lib:$(TARGETDIR)/lib:$(TARGETDIR)/usr/lib \
	./configure --target=arm-linux --host=arm-linux --build=i686 --prefix=$(TARGETDIR)/usr --with-gnu-ld ; \
	touch .conf ; \
	cd .. ; \
	cd source_build_platform ; \
	CC=gcc \
	CXX=g++ \
	./configure ; \
	make
	cd ..
	cp source_build_platform/gsoap/src/soapcpp2 source/gsoap/src
	#soapcpp2-soapcpp2_yacc.o soapcpp2-soapcpp2_lex.o soapcpp2-symbol2.o soapcpp2-error2.o soapcpp2-init2.o soapcpp2-soapcpp2.o
	cp source_build_platform/gsoap/src/soapcpp2* source/gsoap/src
makegsoap:
	cd source ; make

clean: .conf cleangsoap
	
cleangsoap: 
	cd source ; make clean	; cd ../source_build_platform ; make clean

distclean:
	rm -rf .conf
	cd source ; make clean ; make distclean

install:
	cd source ; make install

make:
	cd source ; make
