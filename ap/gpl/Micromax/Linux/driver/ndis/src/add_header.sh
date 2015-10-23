#!/bin/bash

KPATH=/lib/modules/$(uname -r)/kernel/drivers

if [ "$1" == "modules" ]
then
	if [ "$(uname -r)" \< "2.6.25" ]
	then
	#	cp $(pwd)/usbnet.h  $2  
	echo "make mdoules"
	fi
elif [ "$1" == "clean" ]
then
	if [ "$(uname -r)" \< "2.6.22" ]
	then
		if [ -f $KPATH/usb/net/lc_ether.ko ]
		then 
			rm -f $KPATH/usb/net/lc_ether.ko
			depmod -a
		fi
	else
		if [ -f $KPATH/net/usb/lc_ether.ko ]
		then 
			rm -f $KPATH/net/usb/lc_ether.ko
			depmod -a
		fi
	fi
	
elif [ "$1" == "install" ]
then
	if [ "$(uname -r)" \< "2.6.22" ]
	then
		if [ -f $(pwd)/lc_ether.ko ]
		then 
			cp $(pwd)/lc_ether.ko $KPATH/usb/net
			chmod 744 $KPATH/usb/net/lc_ether.ko
			depmod -a
		fi
	else
		if [ -f $(pwd)/lc_ether.ko ]
		then 
			cp $(pwd)/lc_ether.ko $KPATH/net/usb
			chmod 744 $KPATH/net/usb/lc_ether.ko
			depmod -a
		fi
	fi
fi


