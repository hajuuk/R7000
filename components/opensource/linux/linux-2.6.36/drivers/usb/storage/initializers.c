/* Special Initializers for certain USB Mass Storage devices
 *
 * Current development and maintenance by:
 *   (c) 1999, 2000 Matthew Dharm (mdharm-usb@one-eyed-alien.net)
 *
 * This driver is based on the 'USB Mass Storage Class' document. This
 * describes in detail the protocol used to communicate with such
 * devices.  Clearly, the designers had SCSI and ATAPI commands in
 * mind when they created this document.  The commands are all very
 * similar to commands in the SCSI-II and ATAPI specifications.
 *
 * It is important to note that in a number of cases this class
 * exhibits class-specific exemptions from the USB specification.
 * Notably the usage of NAK, STALL and ACK differs from the norm, in
 * that they are used to communicate wait, failed and OK on commands.
 *
 * Also, for certain devices, the interrupt endpoint is used to convey
 * status of a command.
 *
 * Please see http://www.one-eyed-alien.net/~mdharm/linux-usb for more
 * information about this driver.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/errno.h>
#include <linux/slab.h>

#include "usb.h"
#include "initializers.h"
#include "debug.h"
#include "transport.h"

/* Add by Jasmine Yang, 01/04/2011 */
#ifndef USB_STORAGE_
#define USB_STORAGE_ "usb-storage: "
#endif
#ifndef USB_DEBUGP
#define USB_DEBUGP(x...) printk( KERN_NOTICE USB_STORAGE_ x )
#endif
#ifndef ZCD_FORCE_MODEM
#define ZCD_FORCE_MODEM			0x01
#endif
#ifndef ZCD_ALLOW_MS
#define ZCD_ALLOW_MS 			0x02
#endif
#ifndef RESPONSE_LEN
#define RESPONSE_LEN 1024
#endif
#ifndef COMMAND_LEN
#define COMMAND_LEN 31
#endif
static unsigned int option_zero_cd = ZCD_FORCE_MODEM;

/* This places the Shuttle/SCM USB<->SCSI bridge devices in multi-target
 * mode */
int usb_stor_euscsi_init(struct us_data *us)
{
	int result;

	US_DEBUGP("Attempting to init eUSCSI bridge...\n");
	us->iobuf[0] = 0x1;
	result = usb_stor_control_msg(us, us->send_ctrl_pipe,
			0x0C, USB_RECIP_INTERFACE | USB_TYPE_VENDOR,
			0x01, 0x0, us->iobuf, 0x1, 5000);
	US_DEBUGP("-- result is %d\n", result);

	return 0;
}

/* This function is required to activate all four slots on the UCR-61S2B
 * flash reader */
int usb_stor_ucr61s2b_init(struct us_data *us)
{
	struct bulk_cb_wrap *bcb = (struct bulk_cb_wrap*) us->iobuf;
	struct bulk_cs_wrap *bcs = (struct bulk_cs_wrap*) us->iobuf;
	int res;
	unsigned int partial;
	static char init_string[] = "\xec\x0a\x06\x00$PCCHIPS";

	US_DEBUGP("Sending UCR-61S2B initialization packet...\n");

	bcb->Signature = cpu_to_le32(US_BULK_CB_SIGN);
	bcb->Tag = 0;
	bcb->DataTransferLength = cpu_to_le32(0);
	bcb->Flags = bcb->Lun = 0;
	bcb->Length = sizeof(init_string) - 1;
	memset(bcb->CDB, 0, sizeof(bcb->CDB));
	memcpy(bcb->CDB, init_string, sizeof(init_string) - 1);

	res = usb_stor_bulk_transfer_buf(us, us->send_bulk_pipe, bcb,
			US_BULK_CB_WRAP_LEN, &partial);
	if (res)
		return -EIO;

	US_DEBUGP("Getting status packet...\n");
	res = usb_stor_bulk_transfer_buf(us, us->recv_bulk_pipe, bcs,
			US_BULK_CS_WRAP_LEN, &partial);
	if (res)
		return -EIO;

	return 0;
}

/* Code added from MBR624 start, Neil Chen, 10/13/2009 mm/dd/yyyy 
   These are functions used by USB modem driver */
/* Foxconn add start, Jasmine Yang, 06/06/2008 */
/* This places the HUAWEI E220 devices in multi-port mode */
int usb_stor_switch_ports_init (struct us_data *us)
{
    int result = 0;

#if 0
    result =
        usb_stor_control_msg (us, us->send_ctrl_pipe, USB_REQ_SET_FEATURE,
                              USB_TYPE_STANDARD | USB_RECIP_DEVICE, 0x01, 0x0,
                              NULL, 0x0, 1000);
#endif
    us->iobuf[0] = 0x1;
    result =
        usb_control_msg (us->pusb_dev, usb_sndctrlpipe (us->pusb_dev, 0),
                         USB_REQ_SET_FEATURE,
                         USB_TYPE_STANDARD | USB_RECIP_DEVICE, 00000001, 0x0,
                         us->iobuf, 0x1, 5*HZ);
                         //USB_TYPE_STANDARD | USB_RECIP_DEVICE, 0x01, 0x0,
                         //us->iobuf, 0x1, 1000);

    USB_DEBUGP ("usb_control_msg performing result is %d\n", result);
    return (result ? 0 : -1);
}
static int option_huawei_rezero (struct us_data *us, int ep_in, int ep_out)
{
    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
        0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    USB_DEBUGP ("Huawei MS: %s", "DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

static int option_huawei_rezero2 (struct us_data *us, int ep_in, int ep_out)
{
    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
        0x06, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    USB_DEBUGP ("Huawei MS: %s", "DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}
/* Add by Jasmine Yang, 01/04/2011 */
static int option_huawei_rezero3 (struct us_data *us, int ep_in, int ep_out)
{
    /*const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x11,
        0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };*/
    
    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
        0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    USB_DEBUGP ("Huawei MS: %s", "DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

static int option_huawei_rezero4 (struct us_data *us, int ep_in, int ep_out)
{
    /*const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x11,
        0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };*/
    
    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
        0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--Huawei MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

static int option_huawei_rezero5 (struct us_data *us, int ep_in, int ep_out)
{
#if 0
5553424312345678
0000000000000a11
0620000000000001
00000000000000

5553424312345678
0000000000000011
0620000001000000
00000000000000
#endif

    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x11,
        0x06, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--Huawei MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int huawei_ms_init5 (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--Huawei MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
#if 1
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || (udev->descriptor.bDeviceProtocol != 0 
            && udev->descriptor.bDeviceProtocol != 0xFF ))
        return USB_STOR_TRANSPORT_GOOD;
#endif
    USB_DEBUGP ("Huawei MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Huawei MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Huawei MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Huawei MS: %s", "Forcing Modem Mode\n");
        result = option_huawei_rezero5 (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Huawei MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}

static int option_huawei_rezero6 (struct us_data *us, int ep_in, int ep_out)
{
#if 0
5553424312345678
0000000000000a11
0620000000000001
00000000000000

5553424312345678
0000000000000011
0620000001000000
00000000000000
#endif

    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
        0x06, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--Huawei MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int huawei_ms_init6 (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--Huawei MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
#if 1
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || (udev->descriptor.bDeviceProtocol != 0 
            && udev->descriptor.bDeviceProtocol != 0xFF ))
        return USB_STOR_TRANSPORT_GOOD;
#endif
    USB_DEBUGP ("Huawei MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Huawei MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Huawei MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Huawei MS: %s", "Forcing Modem Mode\n");
        result = option_huawei_rezero6 (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Huawei MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}



int huawei_ms_init (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;

printk("\n%s %s %d\n\n",__func__,__FILE__,__LINE__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || udev->descriptor.bDeviceProtocol != 0)
        return USB_STOR_TRANSPORT_GOOD;

    USB_DEBUGP ("Huawei MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Huawei MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Huawei MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Huawei MS: %s", "Forcing Modem Mode\n");
        result = option_huawei_rezero (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Huawei MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}
int huawei_ms_init2 (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;

printk("\n%s %s %d\n\n",__func__,__FILE__,__LINE__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || udev->descriptor.bDeviceProtocol != 0)
        return USB_STOR_TRANSPORT_GOOD;

    USB_DEBUGP ("Huawei MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Huawei MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Huawei MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Huawei MS: %s", "Forcing Modem Mode\n");
        result = option_huawei_rezero2 (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Huawei MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}

int huawei_ms_init3 (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;

printk("\n%s %s %d\n\n",__func__,__FILE__,__LINE__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || udev->descriptor.bDeviceProtocol != 0)
        return USB_STOR_TRANSPORT_GOOD;

    USB_DEBUGP ("Huawei MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Huawei MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Huawei MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Huawei MS: %s", "Forcing Modem Mode\n");
        result = option_huawei_rezero3 (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Huawei MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}
int huawei_ms_init4 (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--Huawei MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || udev->descriptor.bDeviceProtocol != 0)
        return USB_STOR_TRANSPORT_GOOD;

    USB_DEBUGP ("Huawei MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Huawei MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Huawei MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Huawei MS: %s", "Forcing Modem Mode\n");
        result = option_huawei_rezero4 (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Huawei MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}


static int option_china_telecom_rezero (struct us_data *us, int ep_in, int ep_out)
{
    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1b,
        0x06, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;


    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int china_telecom_ms_init (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;

    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || udev->descriptor.bDeviceProtocol != 0)
        return USB_STOR_TRANSPORT_GOOD;



    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    { 
        result = option_china_telecom_rezero (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Huawei MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}
/* This places the HUAWEI E220 devices in multi-port mode */
int usb_stor_huawei_e220_init(struct us_data *us)
{
	int result;

	result = usb_stor_control_msg(us, us->send_ctrl_pipe,
				      USB_REQ_SET_FEATURE,
				      USB_TYPE_STANDARD | USB_RECIP_DEVICE,
				      0x01, 0x0, NULL, 0x0, 1000);
	US_DEBUGP("Huawei mode set result is %d\n", result);
	return 0;
}


/*foxconn Han edited start, 07/28/2014 for D-LINK DWP-156*/
static int option_dlink_rezero (struct us_data *us, int ep_in, int ep_out)
{
#if 0
5553424312345678
00000000000003f0
0101000000000000
00000000000000

D-Link DWM-156 HSUPA 3.75G USB Modem
DefaultVendor= 0x07d1
DefaultProduct=0xa800
TargetVendor=  0x07d1
TargetProduct= 0x3e02

5553424312345678
000000000000061b
0000000200000000
00000000000000
#endif

    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1b,
        0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--DLINK MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int dlink_ms_init (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--DLINK MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
#if 1
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || (udev->descriptor.bDeviceProtocol != 0 
            && udev->descriptor.bDeviceProtocol != 0xFF ))
        return USB_STOR_TRANSPORT_GOOD;
#endif
    USB_DEBUGP ("DLINK MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("DLINK MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("DLINK MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("DLINK MS: %s", "Forcing Modem Mode\n");
        result = option_dlink_rezero (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("DLINK MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("DLINK MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}

/*foxconn Han edited start, 07/28/2014 for D-LINK DWP-156*/
static int option_ntgr_rezero (struct us_data *us, int ep_in, int ep_out)
{
#if 0
5553424308902082
000000000000061b
0000000200000000
00000000000000
#endif

    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x08, 0x90, 0x20, 0x82,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1b,
        0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--NTGR MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int ntgr_ms_init (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--NTGR MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
#if 1
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || (udev->descriptor.bDeviceProtocol != 0 
            && udev->descriptor.bDeviceProtocol != 0xFF ))
        return USB_STOR_TRANSPORT_GOOD;
#endif
    USB_DEBUGP ("NTGR MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("NTGR MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("NTGR MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("NTGR MS: %s", "Forcing Modem Mode\n");
        result = option_ntgr_rezero (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("NTGR MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("NTGR MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}
/*foxconn Han edited end, 07/28/2014 for D-LINK DWP-156*/

/*foxconn Han edited start, 08/20/2014 for Micromax*/
static int option_micromax_rezero (struct us_data *us, int ep_in, int ep_out)
{
#if 0
5553424312345678
0000000080000606
f504025270000000
00000000000000
#endif

    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
        0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x06, 0x06,
        0xf5, 0x04, 0x02, 0x52, 0x70, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--Micromax MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int micromax_ms_init (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--Micromax MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
#if 1
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || (udev->descriptor.bDeviceProtocol != 0 
            && udev->descriptor.bDeviceProtocol != 0xFF ))
        return USB_STOR_TRANSPORT_GOOD;
#endif
    USB_DEBUGP ("Micromax MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Micromax MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Micromax MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Micromax MS: %s", "Forcing Modem Mode\n");
        result = option_micromax_rezero (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Micromax MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Micromax MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}

/*foxconn Han edited end, 08/20/2014 for Micromax*/


/*foxconn Han edited start, 08/25/2014 for Micromax MMX377G (Mediatek MT6229 chip inside)*/
static int option_micromax_rezero2 (struct us_data *us, int ep_in, int ep_out)
{
#if 0
5553424308202989
00000000000003f0
0101000000000000
00000000000000
#endif

    const unsigned char rezero_msg[] = {
        0x55, 0x53, 0x42, 0x43, 0x08, 0x20, 0x29, 0x89,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf0,
        0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    char *buffer;
    int result;

    printk ("%s--Micromax MS: %s", __FUNCTION__,"DEVICE MODE SWITCH\n");

    buffer = kzalloc (RESPONSE_LEN, GFP_KERNEL);
    if (buffer == NULL)
        return USB_STOR_TRANSPORT_ERROR;

    memcpy (buffer, rezero_msg, COMMAND_LEN);
    result =
        usb_stor_bulk_transfer_buf (us,
                                    usb_sndbulkpipe (us->pusb_dev, ep_out),
                                    buffer, COMMAND_LEN, NULL);
    if (result != USB_STOR_XFER_GOOD)
    {
        result = USB_STOR_XFER_ERROR;
        goto out;
    }

    /* Some of the devices need to be asked for a response, but we don't
     * care what that response is.
     */
    result =
        usb_stor_bulk_transfer_buf (us, usb_rcvbulkpipe (us->pusb_dev, ep_in),
                                    buffer, RESPONSE_LEN, NULL);
    result = USB_STOR_XFER_GOOD;

  out:
    kfree (buffer);
    return result;
}

int micromax_ms_init2 (struct us_data *us)
{
    struct usb_device *udev;
    struct usb_interface *intf;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint = NULL;
    u8 ep_in = 0, ep_out = 0;
    int ep_in_size = 0, ep_out_size = 0;
    int i, result;

    udev = us->pusb_dev;
    intf = us->pusb_intf;
printk ("%s--Micromax MS: usb_stor_switch_ports_with_msg_init called\n",__FUNCTION__);
printk("0x%X-%X-%X\n",udev->descriptor.bDeviceClass,udev->descriptor.bDeviceSubClass,udev->descriptor.bDeviceProtocol);
    /* Ensure it's really a ZeroCD device; devices that are already
     * in modem mode return 0xFF for class, subclass, and protocol.
     */
#if 1
    if (udev->descriptor.bDeviceClass != 0
        || udev->descriptor.bDeviceSubClass != 0
        || (udev->descriptor.bDeviceProtocol != 0 
            && udev->descriptor.bDeviceProtocol != 0xFF ))
        return USB_STOR_TRANSPORT_GOOD;
#endif
    USB_DEBUGP ("Micromax MS: usb_stor_switch_ports_with_msg_init called\n");

    /* Find the right mass storage interface */
    iface_desc = intf->cur_altsetting;
    if (iface_desc->desc.bInterfaceClass != 0x8
        || iface_desc->desc.bInterfaceSubClass != 0x6
        || iface_desc->desc.bInterfaceProtocol != 0x50)
    {
        USB_DEBUGP ("Micromax MS: mass storage interface not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Find the mass storage bulk endpoints */
    for (i = 0;
         i < iface_desc->desc.bNumEndpoints && (!ep_in_size || !ep_out_size);
         ++i)
    {
        endpoint = &iface_desc->endpoint[i].desc;

        if (usb_endpoint_is_bulk_in (endpoint))
        {
            ep_in = usb_endpoint_num (endpoint);
            ep_in_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_in_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
        else if (usb_endpoint_is_bulk_out (endpoint))
        {
            ep_out = usb_endpoint_num (endpoint);
            ep_out_size = le16_to_cpu (endpoint->wMaxPacketSize);
            /* ep_out_size =
               (endpoint->wMaxPacketSize >
               maxSize) ? endpoint->wMaxPacketSize : maxSize; */
        }
    }

    /* Can't find the mass storage endpoints */
    if (!ep_in_size || !ep_out_size)
    {
        USB_DEBUGP ("Micromax MS: mass storage endpoints not found, no action "
                    "required\n");
        return USB_STOR_TRANSPORT_GOOD;
    }

    /* Force Modem mode */
    if (option_zero_cd == ZCD_FORCE_MODEM)
    {
        USB_DEBUGP ("Micromax MS: %s", "Forcing Modem Mode\n");
        result = option_micromax_rezero2 (us, ep_in, ep_out);
        //result = option_rezero (us, ep_in, ep_out, huawei_rezero_msg);
        if (result != USB_STOR_XFER_GOOD)
            USB_DEBUGP ("Micromax MS: Failed to switch to modem mode.\n");
        return -EIO;
    }
    else if (option_zero_cd == ZCD_ALLOW_MS)
    {
        /* Allow Mass Storage mode (keep CD-Rom) */
        USB_DEBUGP ("Micromax MS: %s",
                    "Allowing Mass Storage Mode if device" " requests it\n");
    }

    return USB_STOR_TRANSPORT_GOOD;
}

/*foxconn Han edited end, 08/25/2014 for Micromax*/
