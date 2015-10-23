#!/bin/bash

/sbin/depmod
if [ "$1" == "ins" ]
then
#	/sbin/modprobe mii
#    /sbin/rmmod option
	/sbin/modprobe lc_ether
	/sbin/modprobe option
    chmod 777 /sys/bus/usb-serial/drivers/option1/new_id
    echo 1c9e $2 > /sys/bus/usb-serial/drivers/option1/new_id
    ifconfig wan0 up
elif [ "$1" == "rmm" ]
then
	/sbin/rmmod option
#	/sbin/rmmod mii
	/sbin/rmmod lc_ether	
fi
