#  Copyright (C) 2011  caoweigang <caoweigang@longcheer.net> <caowai@gmail.com>
#
#  This is free software; you can redistribute it and/or modify
#  it under the terms of Version 2 of the GNU General Public License as
#  published by the Free Software Foundation.

# **********************************************************************
# Foxconn Modified start
# **********************************************************************
#include ../config.mk

KERNELPATH := $(LINUXDIR)
#$(BASEDIR)/components/opensource/linux/linux-2.6.36
KDIR := $(KERNELPATH)
#for local build test
#KDIR := /disk2/han/work2/R7000/0724_3g/components/opensource/linux/linux-2.6.36

#CROSS_COMPILE=arm-brcm-linux-uclibcgnueabi-
SRC_DIR :=$(shell pwd)
EXTRA_CFLAGS += -DFX_HACK

# **********************************************************************
# Foxconn Modified end
# **********************************************************************

obj-m := lctserial.o

#KDIR := /lib/modules/$(shell uname -r)/build
#MODULE_DIR := /lib/modules/$(shell uname -r)/kernel/drivers/usb/serial/
PWD := $(shell pwd)

#default:
	#$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
all:
	$(MAKE) -C $(KDIR) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules


clean:
	rm -rf *.mod.c *.o *.markers *.symvers *.order  *.ko .tmp_versions .*.cmd

install:
	#cp -f *.ko $(MODULE_DIR)
	#depmod
	$(STRIP) -S lctserial.ko
	install -d $(TARGETDIR)/lib/modules/
	install -m 755 lctserial.ko $(TARGETDIR)/lib/modules/

uninstall:
	#rm /lib/modules/$(shell uname -r)/kernel/drivers/usb/serial/lctserial.ko
	#depmod
	#rmmod lctserial 
	
	




