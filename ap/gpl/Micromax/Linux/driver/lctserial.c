/*
  USB Modem Driver for Longcheer devices

  Copyright (C) 2011  caoweigang <caoweigang@longcheer.net> <caowai@gmail.com>

  This driver is free software; you can redistribute it and/or modify
  it under the terms of Version 2 of the GNU General Public License as
  published by the Free Software Foundation.

*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/serial.h>
#include <linux/version.h>

/*
 * Driver information
 */
#define LCT_DRIVER_VERSION 	"v0.03"
#define LCT_DRIVER_DESC 	"Longcheer usb modem driver"
#define LCT_DRIVER_NAME		"Longcheer"	
#define LCT_DEVICE_DESC		"Longcheer usb modem"

#define LCT_VENDOR_ID 		0x1c9e
#define LCT_CM55_PID 		0x9e00
#define LCT_WM6100_PID 		0x9604
#define LCT_WM71_PID 		0x9603
#define LCT_WM7608_PID 		0x9800
#define LCT_WM7211_PID 		0x9605
#define LCT_WM7211_TATA_PID 		0x9803

static struct usb_device_id lct_id_table [] = {
	{ USB_DEVICE(LCT_VENDOR_ID,LCT_CM55_PID) },
	{ USB_DEVICE(LCT_VENDOR_ID,LCT_WM6100_PID)},
	{ USB_DEVICE(LCT_VENDOR_ID,LCT_WM7608_PID)},
	{ USB_DEVICE(LCT_VENDOR_ID,LCT_WM71_PID)},
	{ USB_DEVICE(LCT_VENDOR_ID,LCT_WM7211_PID)},
	{ USB_DEVICE(LCT_VENDOR_ID,LCT_WM7211_TATA_PID)},
	{ }					
};

MODULE_DEVICE_TABLE(usb, lct_id_table);

#if	LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,31)
static int lct_serial_open(struct tty_struct *tty,
			struct usb_serial_port *port, struct file *filp)
#else
static int lct_serial_open(struct tty_struct *tty,
			struct usb_serial_port *port)
#endif
{
	struct usb_serial *serial = port->serial;
	usb_control_msg(serial->dev, usb_rcvctrlpipe(serial->dev, 0), 0x22, 0x21, 3, 0, NULL, 0, USB_CTRL_SET_TIMEOUT);
#if	LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,31)	
	return usb_serial_generic_open(tty,port,filp);
#else
	return usb_serial_generic_open(tty,port);
#endif
}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,30)
static void lct_serial_close(struct tty_struct *tty,
			struct usb_serial_port *port, struct file *filp)
#else
static void lct_serial_close(struct usb_serial_port *port)
#endif
{
	struct usb_serial *serial = port->serial;
	usb_control_msg(serial->dev, usb_rcvctrlpipe(serial->dev, 0), 0x22, 0x21, 0, 0, NULL, 0, USB_CTRL_SET_TIMEOUT);

	if (serial->dev) 
	{
		if (serial->num_bulk_out)
			usb_kill_urb(port->write_urb);
		if (serial->num_bulk_in)
			usb_kill_urb(port->read_urb);
	}
}
//#endif


static struct usb_driver lct_modem_driver = {
	.name =	LCT_DRIVER_NAME,
	.probe = usb_serial_probe,
	.disconnect = usb_serial_disconnect,
	.no_dynamic_id = 1,
	.id_table = lct_id_table,
};


static struct usb_serial_driver lct_modem_device = {
	.driver = {
		.owner = THIS_MODULE,
		.name =	LCT_DRIVER_NAME,
	},
	.description = LCT_DEVICE_DESC,
	.id_table =	lct_id_table,
	.usb_driver = &lct_modem_driver,
	.num_ports  = 1,
//#if	LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,30)
	.open = lct_serial_open,
	.close = lct_serial_close,
//#endif	
};

static int __init lct_modem_init(void)
{
	int retval;

	retval = usb_serial_register(&lct_modem_device);

	if (retval)
	{
		return retval;
	}

	retval = usb_register(&lct_modem_driver);

	if (retval)
	{
		usb_serial_deregister(&lct_modem_device);
		return retval;
	}

	printk(LCT_DRIVER_VERSION": "LCT_DRIVER_DESC"\n");

	return 0;
}

static void __exit lct_modem_exit(void)
{
	usb_deregister(&lct_modem_driver);
	usb_serial_deregister(&lct_modem_device);
}

module_init(lct_modem_init);
module_exit(lct_modem_exit);

MODULE_DESCRIPTION(LCT_DRIVER_DESC);
MODULE_VERSION(LCT_DRIVER_VERSION);
MODULE_LICENSE("GPL");
