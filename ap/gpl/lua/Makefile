export CROSS_COMPILE = "arm-uclibc-linux-2.6.36-"

all: .conf  makelua
.conf:
	cd source ;
	touch .conf

makelua:
	cd source ; make r7000 

clean: .conf cleanlua
	
cleanlua: 
	cd source ; make clean	

distclean:
	rm -rf .conf
	cd source ; make clean

install:
	#cd source ; make r7000 install
	
