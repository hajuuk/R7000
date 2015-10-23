export CROSS_COMPILE = "arm-uclibc-linux-2.6.36-"

all: conf makelsync
conf:
	touch .conf ; \
	cd source ; \
	CFLAGS="${CFLAGS} -I '$(shell pwd)/../lua/source/src'" \
	LUA_LIBS="-L../../lua/source/src -llua -lm -ldl" \
	./configure --target=arm-linux \
			--host=arm-linux \
			--build=$(BUILD) \
			--prefix='$(shell pwd)/tmp' \
			--exec-prefix='$(TARGETDIR)'

makelsync:
	cd source ; make

clean: conf cleanlsync	
	
cleanlsync: 
	cd source ; make clean	

distclean:
	rm -rf conf
	cd source ; make clean

install:
#only need a binary
	mkdir -p ${TARGETDIR}/opt/leafp2p/bin
	install -m 755 $(TOP)/../../ap/gpl/lsync/source/lsyncd  $(TARGETDIR)/opt/leafp2p/bin
