#!/bin/bash
rmmod -f cdc_ether
rmmod -f usbnet
rmmod -f lc_ether

make clean
make modules
make install
