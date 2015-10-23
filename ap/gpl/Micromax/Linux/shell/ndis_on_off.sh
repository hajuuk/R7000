#!/bin/bash

if [ "$1" == "on" ]
then
#	gateway=$(route -n | grep 'UG[ \t]' | awk '{print $2}')
#	interface = $(route -n | grep 'UG[ \t]' | awk '{print $8}')
	sudo /sbin/route del default
	sudo /sbin/dhclient wan0 
elif [ "$1" == "off" ]
then
#	gateway=$(route -n | grep 'UG[ \t]' | awk '{print $2}')
#	interface = $(route -n | grep 'UG[ \t]' | awk '{print $8}')
	sudo /sbin/route del default
	sudo /sbin/ifconfig wan0 0.0.0.0
	sudo /sbin/dhclient
elif [ "$1" == "open" ]
then
	sudo /sbin/ifconfig wan0 up
fi
exit 0
