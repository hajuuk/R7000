
BT_PATH = $(shell pwd)

export CROSS_COMPILE	:= arm-uclibc-linux-2.6.36-
export AS				:= $(CROSS_COMPILE)as
export LD				:= $(CROSS_COMPILE)ld
export CC				:= $(CROSS_COMPILE)gcc
export CPP				:= $(CC) -E
export CXX				:= $(CROSS_COMPILE)g++
export AR				:= $(CROSS_COMPILE)ar
export NM				:= $(CROSS_COMPILE)nm
export STRIP			:= $(CROSS_COMPILE)strip
export OBJCOPY			:= $(CROSS_COMPILE)objcopy
export OBJDUMP			:= $(CROSS_COMPILE)objdump
export RANLIB			:= $(CROSS_COMPILE)ranlib
export SIZE				:= $(CROSS_COMPILE)size

TOOLCHAIN := $(shell cd $(dir $(shell which $(CROSS_COMPILE)gcc))/.. && pwd -P)

CFLAGS += -s -Os -fPIC

CFLAGS += -I$(BT_PATH)/openssl/include
CFLAGS += -I$(BT_PATH)/zlib-1.2.3
CFLAGS += -I$(BT_PATH)/../curl-7.23.1/source/include
CFLAGS += -I$(BT_PATH)/libevent-2.0.20-stable/include

LDFLAGS += -static
LDFLAGS += -L$(TOOLCHAIN)/lib
LDFLAGS += -L$(BT_PATH)/openssl
LDFLAGS += -L$(BT_PATH)/zlib-1.2.3
LDFLAGS += -L$(BT_PATH)/../curl-7.23.1/source/lib/.libs 
LDFLAGS += -L$(BT_PATH)/libevent-2.0.20-stable/.libs

CONFIGURE ?= ./configure --host=arm-linux --build=i386-pc-linux-gnu
CROSS_COMPILE_ENV := CC=$(CC) CPP="$(CPP)" CXX=$(CXX) AS=$(AS) LD=$(LD)
CROSS_COMPILE_ENV += AR=$(AR) NM=$(NM) RANLIB=$(RANLIB) STRIP=$(STRIP)
CROSS_COMPILE_ENV += OBJCOPY=$(OBJCOPY) OBJDUMP=$(OBJDUMP) SIZE=$(SIZE)

all: libevent
	cd transmission-2.73 && ($(CROSS_COMPILE_ENV) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
		OPENSSL_CFLAGS="$(CFLAGS)" OPENSSL_LIBS="$(LDFLAGS)" \
		LIBCURL_CFLAGS="$(CFLAGS)" LIBCURL_LIBS="$(LDFLAGS)" \
		LIBEVENT_CFLAGS="$(CFLAGS)" LIBEVENT_LIBS="$(LDFLAGS)" \
		LIBS="-lssl -lcrypto -lz -lcurl -levent -levent_core -levent_extra -levent_openssl -levent_pthreads -lm -lrt -ldl" \
		$(CONFIGURE) \
		--enable-lightweight \
		--disable-silent-rules \
		) && make

libevent: curl
	cd libevent-2.0.20-stable && ($(CROSS_COMPILE_ENV) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
		$(CONFIGURE) \
		--disable-malloc-replacement \
		) && make
    
curl: zlib openssl
	cd $(BT_PATH)/../curl-7.23.1/source && echo "ac_cv_file___dev_urandom_=yes" > config.cache && ($(CROSS_COMPILE_ENV)\
		CFLAGS="$(CFLAGS)" \
		LDFLAGS="$(LDFLAGS)" \
		$(CONFIGURE) -C \
		--with-ssl=$(BT_PATH)/openssl \
		--with-zlib=$(BT_PATH)/zlib-1.2.3 \
		) && make

zlib:
	cd zlib-1.2.3 && ($(CROSS_COMPILE_ENV) AR="$(AR) cru" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
		./configure \
		) && make

openssl:

install:
	$(STRIP) transmission-2.73/cli/transmission-cli
	cp -a transmission-2.73/cli/transmission-cli $(TARGETDIR)/usr/sbin/

clean:
