export CROSS_COMPILE = "arm-uclibc-linux-2.6.36-"

export CPPFLAGS += -I$(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/include -I$(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/include/openssl
export LDFLAGS += -L$(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/lib
export PKG_CONFIG_PATH += $(TOP)/../../ap/gpl/openssl/tmp/usr/local/ssl/lib/pkgconfig

all: .conf  makexml2  install
.conf:
	cd source ; \
	./configure --host=arm --prefix=$(TARGETDIR)/usr
	touch .conf

makexml2:
	cd source ; make

clean: .conf cleanxml2
	
cleanxml2: 
	cd source ; make clean	

distclean:
	rm -rf .conf
	cd source ; make clean

install:
	cd source ; make install

	
