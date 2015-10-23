
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
CFLAGS += -I$(BT_PATH)/wxWidgets-2.8.12/include

#LDFLAGS += -static
#LDFLAGS += -L$(TOOLCHAIN)/lib
LDFLAGS += -L$(BT_PATH)/wxWidgets-2.8.12/lib
LDFLAGS += -L$(BT_PATH)/libcryptoxx-5.6.0

CONFIGURE ?= ./configure --host=arm-linux --build=i386-pc-linux-gnu
CROSS_COMPILE_ENV := CC=$(CC) CPP="$(CPP)" CXX=$(CXX) AS=$(AS) LD=$(LD)
CROSS_COMPILE_ENV += AR=$(AR) NM=$(NM) RANLIB=$(RANLIB) STRIP=$(STRIP)
CROSS_COMPILE_ENV += OBJCOPY=$(OBJCOPY) OBJDUMP=$(OBJDUMP) SIZE=$(SIZE)


all: wxWidgets libcryptoxx 
	cd aMule-2.3.1 && ($(CROSS_COMPILE_ENV) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
		$(CONFIGURE) \
		--disable-nls  \
		--disable-ipv6 \
		--enable-shared \
		--disable-static \
		--disable-rpath \
		--with-gnu-ld \
		--disable-ccache \
		--disable-debug \
		--disable-optimize \
		--disable-profile \
		--disable-monolithic \
		--enable-amule-daemon \
		--enable-amulecmd \
		--disable-amulecmdgui \
		--disable-webserver \
		--disable-webservergui \
		--disable-amule-gui \
		--disable-cas \
		--disable-wxcas \
		--disable-ed2k \
		--disable-alc \
		--disable-alcc \
		--disable-systray \
		--disable-utf8-systray \
		--enable-embedded-crypto \
		--enable-gsocket \
		--disable-gtktest \
		--disable-crypto\
		--with-wx-prefix=$(BT_PATH)/wxWidgets-2.8.12/lib \
		--with-wxdir=$(BT_PATH)/wxWidgets-2.8.12 \
		--with-wx-config=$(BT_PATH)/wxWidgets-2.8.12/wx-config \
		--with-crypto-prefix=$(BT_PATH)/libcryptoxx-5.6.0 \
		) && make 
		cd .. 
		make copy 

wxWidgets:
	cd wxWidgets-2.8.12 && (rm -rf config.cache) \
		&& ($(CROSS_COMPILE_ENV) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
		$(CONFIGURE) -C \
		--disable-gui \
		--enable-unicode \
		) && make
        
libcryptoxx:
	cd libcryptoxx-5.6.0 && ($(CROSS_COMPILE_ENV) CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" \
		) && make

copy:
	cp $(TOOLCHAIN)/usr/lib/libstdc++.so.6                      ./lib/libstdc++.so.6 && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu-2.8.so.0              ./lib/libwx_baseu-2.8.so.0 && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu-2.8.so.0.8.0          ./lib/libwx_baseu-2.8.so.0.8.0 && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu-2.8-arm-linux.so      ./lib/libwx_baseu-2.8-arm-linux.so && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu_net-2.8.so.0          ./lib/libwx_baseu_net-2.8.so.0  && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu_net-2.8.so.0.8.0      ./lib/libwx_baseu_net-2.8.so.0.8.0 && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu_net-2.8-arm-linux.so  ./lib/libwx_baseu_net-2.8-arm-linux.so && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu_xml-2.8.so.0          ./lib/libwx_baseu_xml-2.8.so.0 && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu_xml-2.8.so.0.8.0      ./lib/libwx_baseu_xml-2.8.so.0.8.0 && \
	cp ./wxWidgets-2.8.12/lib/libwx_baseu_xml-2.8-arm-linux.so  ./lib/libwx_baseu_xml-2.8-arm-linux.so && \
	cp ./aMule-2.3.1/src/amulecmd   ./lib/amulecmd  && \
	cp ./aMule-2.3.1/src/amuled     ./lib/amuled

install:
	$(CROSS_COMPILE)strip ./lib/*
	cp -rf ./wxWidgets-2.8.12/lib/wx $(TARGETDIR)/lib
	cp -rf ./aMule-2.3.1/aMule $(TARGETDIR)/etc
	install -m 755 lib/libstdc++.so.6  $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu-2.8.so.0  $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu-2.8-arm-linux.so $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu-2.8.so.0.8.0 $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu_net-2.8.so.0 $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu_net-2.8.so.0.8.0 $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu_net-2.8-arm-linux.so $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu_xml-2.8.so.0 $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu_xml-2.8.so.0.8.0 $(TARGETDIR)/lib
	install -m 755 lib/libwx_baseu_xml-2.8-arm-linux.so $(TARGETDIR)/lib
	install -m 755 lib/amulecmd $(TARGETDIR)/usr/sbin/
	install -m 755 lib/amuled $(TARGETDIR)/usr/sbin/

clean:
	cd wxWidgets-2.8.12  && make distclean  
	cd libcryptoxx-5.6.0  && make clean 
	cd aMule-2.3.1  && make distclean 
	rm ./lib/*.so.*
    
