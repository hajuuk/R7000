file list:

make_driver.sh------------------------this is ndis driver make shell.
Makefile------------------------------this is the ndis driver make file.
dhcpcd.sh-----------------------------this is the dhcpcd config file to config arp

src-----------------------------------this is driver source file folder.
src/lc_cdc_ether.c--------------------this is the ndis driver main file.
src/qmi_oper.c------------------------this is the qmi operation file 
src/qmi_oper.h------------------------this is the qmi operation header file.
src/qmi_header.h----------------------this is the qmi operation main structure defination file.
src/lc_ether.ko-----------------------this is the ndis driver object module 

app-----------------------------------this is ndis dail lib and test file folder.
app/build.sh--------------------------this is the ndis lib and ndis test make shell.
app/build_prep.arm--------------------this is used when you need build the test file on a embeded platform,modify and "source" it
app/ndis_driver_test.c----------------this is the ndis dail test file use a static lib.
app/ndis_test-------------------------this is the ndis dail test object file.
app/libndis_dail.a--------------------this is the ndis dail static lib object file.

app/lib_ndis/ndis_qmi_service.h-------this is the ndis dail static lib header file.
app/lib_ndis/ndis_qmi_service.c-------this is the ndis dail static lib source file.
app/lib_ndis/Makefile-----------------this is the ndis dail static lib make file.


how to build ndis driver:
#chmod +x make_driver.sh
#./make_driver.sh

how to use ndis driver:

when you run make_driver.sh, system will insmod automatic, if you want to insmod manualy:
#insmod /lib/modules/$(uname -r)/kernel/drivers/net/mii.ko
#insmod ./lc_ether.ko

how to build the lib and test file
#cd app
#chmod +x build.sh
#./build.sh

how to use the ndis dail test file#
#./ndis_test
1----------get ndis qmi version,must up 1.3
2----------connect to internet
3----------disconnect.
4----------get status.
-1---------exit.


Notes:
    1.this driver is just a beta version, if you find any bugs,please contract to the author: qiaoyongkang@longcheer.net.
    2.this version support arp, has well tested by dhcp software: udhcpc and dhcpcd.
	

version update:
    version 0.1 ....beta version,not support arp ping, and may have bugs, released 2012.1.30
    version 1.0 ....support arp ping, and tested at linux kernel 2.6.33/29/35,support pc and embeded kernel,released 2012.2.3
	version 1.1 ....fixed the bug on linux kernel 2.6.21 of mips, tested on ralink platform released 2012.2.8
	version 1.2 ....fixed the bug on linux kernel 2.6.21 of mips, tested on ralink platform released 2012.2.10
thanks.


