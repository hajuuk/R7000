/*
 * CDC Ethernet based the networking peripherals of Longcheer data card devices
 * This driver is developed based on usbnet.c and cdc_ether.c
 * Copyright (C) 2009 by kahn.qiao (Longcheer Technologies Co., Ltd.)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will support Longcheer data card devices for Linux networking,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/workqueue.h>
#include <linux/mii.h>
#include <linux/usb.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <linux/usb/cdc.h>
#include <linux/usbdevice_fs.h>

#include <linux/version.h>
#include "qmi_header.h"
#include "qmi_oper.h"
/////////////////////////////////////////////////////////////////////////////////////////////////
#define DRIVER_VERSION "v2.07.00.00"
#define DRIVER_AUTHOR "kahn qiao <kahn.qiao@Longcheer.com>"
#define DRIVER_DESC "Longcheer ether driver for 3G data card ether device"
//////////////////////////////////////////////////////////////////////////////////////////////////////
#define RX_MAX_QUEUE_MEMORY (60 * 1518)
#define	RX_QLEN(dev) (((dev)->udev->speed == USB_SPEED_HIGH) ? \
			(RX_MAX_QUEUE_MEMORY/(dev)->rx_urb_size) : 4)
#define	TX_QLEN(dev) (((dev)->udev->speed == USB_SPEED_HIGH) ? \
			(RX_MAX_QUEUE_MEMORY/(dev)->hard_mtu) : 4)

// reawaken network queue this soon after stopping; else watchdog barks
#define TX_TIMEOUT_JIFFIES	(5*HZ)

// throttle rx/tx briefly after some faults, so khubd might disconnect()
// us (it polls at HZ/4 usually) before we report too many false errors.
#define THROTTLE_JIFFIES	(HZ/8)

// between wakeups
#define UNLINK_TIMEOUT_MS	3
#define PROCESS_ARP_PACKAGE
#define NDIS_INIT_STATUS
//////////////////////////////////////////////////////////////////////////////////////////////
// randomly generated ethernet address
static u8	node_id [ETH_ALEN];

static const char driver_name [] = "lc_ether";

/* use ethtool to change the level for any given device */
static int msg_level = -1;
module_param (msg_level, int, 0);
MODULE_PARM_DESC (msg_level, "Override default message level");

#pragma pack(push, 1)

// ethernet headers
#define ETH_TYPE_ARP  0x0806
#define ETH_TYPE_IPV4 0x0800
#define ETH_TYPE_IPV6 0x86DD

#define ETH_LENGTH_OF_ADDRESS 6

#define LC_INTERFACE_INDEX 0X04

static QC_ETH_HDR fEthernetHeader;

/*max ethernet pkt size 1514*/
#define lc_USB_RECEIVE_BUFFER_SIZE    1600L  
/*for Tin-layer-protocol (TLP)*/
#define lc_USB_MRECEIVE_BUFFER_SIZE   4096L  
/*for TLP*/
#define lc_USB_MRECEIVE_MAX_BUFFER_SIZE (1024*16)  

///////////////////////////////////////////////////////////////////////////////////////////
#define EVENT_TX_HALT 0
#define EVENT_RX_HALT 1
#define EVENT_RX_MEMORY 2
#define EVENT_STS_SPLIT 3
#define EVENT_LINK_RESET 4

static int rt_debug = 0;
module_param(rt_debug, bool, S_IRUGO|S_IWUSR);

#if 0
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/unaligned/access_ok.h>
#else
static inline u16 get_unaligned_le16(const void *p)
{
	return le16_to_cpup((__le16 *)p);
}

static inline u32 get_unaligned_le32(const void *p)
{
	return le32_to_cpup((__le32 *)p);
}

static inline void put_unaligned_le16(u16 val, void *p)
{
	*((__le16 *)p) = cpu_to_le16(val);
}

static inline void put_unaligned_le32(u32 val, void *p)
{
	*((__le32 *)p) = cpu_to_le32(val);
}
#endif
#endif
/* Add for DTS2011050903736 lxz 20110520 start*/
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37)
#define LINUX_VERSION37_LATER 1
#else
#define LINUX_VERSION37_LATER 0 
#endif

static UInt8 fCurMacAddress[6]=
	{0x00,0xA0,0xC6,0x00,0x00,0x00};
static UInt8 fDstMacAddress[6]=
	{0x00,0xA0,0xC6,0x00,0x00,0x01};

#ifdef INIT_NDP16_OPTS 
#undef INIT_NDP16_OPTS 
#endif
#ifdef INIT_NDP32_OPTS 
#undef INIT_NDP32_OPTS 
#endif

static   NDIS_QMI_STATUS     ndis_status;

#ifndef max
#define max(_a, _b)     (((_a) > (_b)) ? (_a) : (_b))
#endif

#ifndef min
#define min(_a, _b)     (((_a) < (_b)) ? (_a) : (_b))
#endif

struct lc_cdc_net{
	/* housekeeping */
	struct usb_device	*udev;
	struct usb_interface	*intf;
	const char		*driver_name;
	const char 		*driver_desc;
	void			*driver_priv;
	wait_queue_head_t	*wait;
	struct mutex		phy_mutex;	

	/* i/o info: pipes etc */
	unsigned		in, out;
	struct usb_host_endpoint *status;
	unsigned		maxpacket;
	struct timer_list	delay;

	/* protocol/interface state */
	struct net_device	*net;
	struct net_device_stats	stats;
	int			msg_enable;
	unsigned long		data [5];
	u32			xid;
	u32			hard_mtu;	/* count any extra framing */
	size_t			rx_urb_size;	/* size for rx urbs */
	struct mii_if_info	mii;

	/* various kinds of pending driver work */
	struct sk_buff_head	rxq;
	struct sk_buff_head	txq; 
	struct sk_buff_head	done;
	struct urb		*interrupt;
	struct tasklet_struct	bh;

	struct work_struct	kevent;
	struct delayed_work status_work;
	int			qmi_sync;
	unsigned long		flags;
	unsigned char		suspend_count;
}__attribute__((packed)) ;

static inline struct usb_driver *driver_of(struct usb_interface *intf)
{
	return to_usb_driver(intf->dev.driver);
}


/* Drivers that reuse some of the standard USB CDC infrastructure
 * (notably, using multiple interfaces according to the CDC
 * union descriptor) get some helper code.
 */
struct lc_dev_state {
	struct usb_cdc_header_desc	*header;
	struct usb_cdc_union_desc	*u;
	struct usb_cdc_ether_desc	*ether;
	struct usb_interface		*control;
	struct usb_interface		*data;
};


/* we record the state for each of our queued skbs */
enum skb_state {
	illegal = 0,
	tx_start, tx_done,
	rx_start, rx_done, rx_cleanup
};

struct skb_data {	/* skb->cb is one of these */
	struct urb		*urb;
	struct lc_cdc_net		*dev;
	enum skb_state		state;
	size_t			length;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define devdbg(lc_cdc_net, fmt, arg...) \
	((void)(rt_debug && printk(KERN_ERR "lc_cdc_driver######: " fmt "\n" , ## arg)))


#define deverr(lc_cdc_net, fmt, arg...) \
	printk(KERN_ERR "%s: " fmt "\n" , (lc_cdc_net)->net->name , ## arg)
#define devwarn(lc_cdc_net, fmt, arg...) \
	printk(KERN_WARNING "%s: " fmt "\n" , (lc_cdc_net)->net->name , ## arg)

#define devinfo(lc_cdc_net, fmt, arg...) \
	printk(KERN_INFO "%s: " fmt "\n" , (lc_cdc_net)->net->name , ## arg); \


////////////////////////////////////////////////////////////////////////////////
#ifdef NDIS_INIT_STATUS
static void lc_cdc_status(struct lc_cdc_net *dev, struct urb *urb);
#endif
static inline int lc_get_ethernet_addr(struct lc_cdc_net *dev);
static int lc_cdc_bind(struct lc_cdc_net *dev, struct usb_interface *intf);
void lc_cdc_unbind(struct lc_cdc_net *dev, struct usb_interface *intf);

static void lc_cdc_check_status_work();
/*{
	struct delayed_work *option_suspend_wq
}*/

static struct lc_cdc_net *g_cdc_dev = NULL;
static int ndis_get_client_ID(struct lc_cdc_net *dev);
static int ndis_get_version_info(struct lc_cdc_net *dev);
static int ndis_set_instance_ID(struct lc_cdc_net *dev);
static int ndis_set_data_format(struct lc_cdc_net *dev);
static int ndis_release_client_ID(struct lc_cdc_net *dev);
static int ndis_connect(struct lc_cdc_net *dev,char *accessString, 
					char *userName, 
					char *passWord,
					UInt8 compression,
					UInt32 ConnectIpMode);

static int ndis_get_ip_addr(struct lc_cdc_net *dev);
static int ndis_disconnect(struct lc_cdc_net *dev);
static int lc_get_qmi_status(struct usb_interface *intf);

#ifdef PROCESS_ARP_PACKAGE
static void lc_skb_return_arp (struct lc_cdc_net *dev, struct sk_buff *skb);
static void  ProcessArpPacket(UInt8 *pData, UInt32 packetlength);
static void  ResponseArpPacket(UInt8 *pData, UInt32 packetlength);
static bool  FilterArpPacket(UInt8 *pData, UInt32 packetlength);
#endif
enum{
	NDIS_CMD_CONNECT_CMD = 0xA0,
	NDIS_CMD_DISCONN_CMD,
	NDIS_CMD_GET_VERSION,
	NDIS_CMD_GET_STATUS,
	NDIS_CMD_INIT_STATUS
};
static spinlock_t  qmi_request_lock; 
typedef struct __ndis_command_t{
int            cmd;
UInt8  data[WWAN_STRING_LEN*3+sizeof(UInt32)*2];
} ndis_command_t;

typedef struct 
{
	int connection_status;
	UInt32 ip_address;
} WWAN_STATUS;

/* handles CDC Ethernet and many other network "bulk data" interfaces */
int lc_get_endpoints(struct lc_cdc_net *dev, struct usb_interface *intf)
{
	int				tmp;
	struct usb_host_interface	*alt = NULL;
	struct usb_host_endpoint	*in = NULL, *out = NULL;
	struct usb_host_endpoint	*status = NULL;

	for (tmp = 0; tmp < intf->num_altsetting; tmp++) {
		unsigned	ep;

		in = out = status = NULL;
		alt = intf->altsetting + tmp;

		/* take the first altsetting with in-bulk + out-bulk;
		 * remember any status endpoint, just in case;
		 * ignore other endpoints and altsetttings.
		 */
		for (ep = 0; ep < alt->desc.bNumEndpoints; ep++) {
			
			struct usb_host_endpoint	*e;
			int				intr = 0;

			e = alt->endpoint + ep;
			switch (e->desc.bmAttributes) {
			case USB_ENDPOINT_XFER_INT:
				if (!usb_endpoint_dir_in(&e->desc))
					continue;
				intr = 1;
				/* FALLTHROUGH */
			case USB_ENDPOINT_XFER_BULK:
				break;
			default:
				continue;
			}
			if (usb_endpoint_dir_in(&e->desc)) {
				if (!intr && !in)
					in = e;
				else if (intr && !status)
					status = e;
			} else {
				if (!out)
					out = e;
			}
		}
		if (in && out)
			break;
	}
	if (!alt || !in || !out)
		return -EINVAL;
	if (alt->desc.bAlternateSetting != 0) {
		tmp = usb_set_interface (dev->udev, alt->desc.bInterfaceNumber,
				alt->desc.bAlternateSetting);
		if (tmp < 0)
			return tmp;
	}

	dev->in = usb_rcvbulkpipe (dev->udev,
			in->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
	dev->out = usb_sndbulkpipe (dev->udev,
			out->desc.bEndpointAddress & USB_ENDPOINT_NUMBER_MASK);
	dev->status = status;
	return 0;
}
EXPORT_SYMBOL_GPL(lc_get_endpoints);
#ifdef  NDIS_INIT_STATUS
static void intr_complete (struct urb *urb);

static int init_status (struct lc_cdc_net *dev, struct usb_interface *intf)
{
	char		*buf = NULL;
	unsigned	pipe = 0;
	unsigned	maxp;
	unsigned	period;


	pipe = usb_rcvintpipe (dev->udev,
			dev->status->desc.bEndpointAddress
				& USB_ENDPOINT_NUMBER_MASK);
	maxp = usb_maxpacket (dev->udev, pipe, 0);

	/* avoid 1 msec chatter:  min 8 msec poll rate */
	period = max ((int) dev->status->desc.bInterval,
		(dev->udev->speed == USB_SPEED_HIGH) ? 7 : 3);

	buf = kmalloc (maxp, GFP_KERNEL);
	if (buf) {
		dev->interrupt = usb_alloc_urb (0, GFP_KERNEL);
		if (!dev->interrupt) {
			kfree (buf);
			return -ENOMEM;
		} else {
			usb_fill_int_urb(dev->interrupt, dev->udev, pipe,
				buf, maxp, intr_complete, dev, period);
			devdbg(g_cdc_dev,"status ep%din, %d bytes period %d\n",
				usb_pipeendpoint(pipe), maxp, period);
		}
	}
	return 0;
}
#endif

#ifdef PROCESS_ARP_PACKAGE

static void  ProcessArpPacket(UInt8 *pData, UInt32 packetlength)
{
  //  QC_ETH_HDR* pHdr = (QC_ETH_HDR*)pData;
	QC_ARP_HDR* pARPHdr = (QC_ARP_HDR*)(pData + sizeof(QC_ETH_HDR));
	UInt8       tempHA[ETH_LENGTH_OF_ADDRESS] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	UInt32      sz = 0;
	
	// ignore non-ethernet HW type and non-request operation
	if (ntohs(pARPHdr->HardwareType) != 1
		|| ntohs(pARPHdr->Operation) != 1)
	{
	    devdbg(g_cdc_dev,"%s : drop arp without hw type and request,HardwareType=%u,Operation=%u.\n",
			__func__, pARPHdr->HardwareType, pARPHdr->Operation);
		return;
	}
	
	// ignore non ipv4 protocol type
	if (ntohs(pARPHdr->ProtocolType) != ETH_TYPE_IPV4)
	{
		devdbg(g_cdc_dev,"%s : ignore packet not using ip v4,ProtocolType=%u.\n",
			__func__, pARPHdr->ProtocolType);
	    return;
	}
	
	// validate HLEN and PLEN
	if (pARPHdr->HLEN != ETH_LENGTH_OF_ADDRESS)
	{
	    devdbg(g_cdc_dev,"ProcessArpPacket : invalid HLEN=%u,\n", pARPHdr->HLEN);
	    return;
	}
	
	if (pARPHdr->PLEN != 4)
	{
		devdbg(g_cdc_dev,"ProcessArpPacket : invalid PLEN=%u.\n", pARPHdr->PLEN);
	    return;
	}
	
	// Ignore gratuitous arp
	if (pARPHdr->SenderIP == pARPHdr->TargetIP)
	{
        devdbg(g_cdc_dev,"ProcessArpPacket : ignore gratuitous arp.\n");
		return;
	}
	
	sz = memcmp(pARPHdr->TargetHA, tempHA, ETH_LENGTH_OF_ADDRESS);
	
	if (pARPHdr->SenderIP != 0 && pARPHdr->TargetIP != ndis_status.fIPAddress)
	{
	    devdbg(g_cdc_dev, "ProcessArpPacket : process sender ip is not null,SenderIP=%u,fIPAddress=%u,TargetIP=%u.\n",
			pARPHdr->SenderIP, ndis_status.fIPAddress, pARPHdr->TargetIP);
		ResponseArpPacket(pData, packetlength);
	}
	else if (pARPHdr->SenderIP == 0 && ETH_LENGTH_OF_ADDRESS == sz)
	{
	    devdbg(g_cdc_dev,"ProcessArpPacket : process sender ip is null,senderIP=%u,TargetIP=%u.\n", pARPHdr->SenderIP, pARPHdr->TargetIP); 
		ResponseArpPacket(pData, packetlength);
	}
	else 
	{
	   devdbg(g_cdc_dev,"ProcessArpPacket : drop other arp.\n"); 	
	}
}

static void  ResponseArpPacket(UInt8 *pData, UInt32 packetlength)
{
    QC_ETH_HDR* pHdr = (QC_ETH_HDR*)pData;
	QC_ARP_HDR* pARPHdr = (QC_ARP_HDR*)(pData + sizeof(QC_ETH_HDR));
	
	QC_ETH_HDR* pSkbHdr = NULL;
	QC_ARP_HDR* pSkbARPHdr = NULL;
	
	struct sk_buff		*skb;
	struct skb_data		*entry;
	
	
	if ((skb = alloc_skb (packetlength + NET_IP_ALIGN, GFP_ATOMIC)) == NULL) {
		deverr (g_cdc_dev, "no rx skb");
	//	lc_defer_kevent (g_cdc_dev, EVENT_RX_MEMORY);
		return;
	}
	skb->len = packetlength;
	entry = (struct skb_data *) skb->cb;
	entry->dev = g_cdc_dev;
	entry->state = rx_start;
	entry->length = 0;
	
	pSkbHdr = (QC_ETH_HDR*)(skb->data);
	pSkbARPHdr = (QC_ARP_HDR*)((UInt8*)pSkbHdr + sizeof(QC_ETH_HDR));
	
	memcpy(pSkbHdr,pData,packetlength);
	
	// process ethernet header
	memcpy(pSkbHdr->DstMacAddress, pHdr->SrcMacAddress, ETH_LENGTH_OF_ADDRESS);
	// mac address from firmware
	memcpy(pSkbHdr->SrcMacAddress, fDstMacAddress, ETH_LENGTH_OF_ADDRESS);
	
	// process arp header
	memcpy(pSkbARPHdr->TargetHA, pARPHdr->SenderHA, ETH_LENGTH_OF_ADDRESS);
	pSkbARPHdr->SenderIP = pARPHdr->TargetIP;
	
	memcpy(pSkbARPHdr->SenderHA, fDstMacAddress, ETH_LENGTH_OF_ADDRESS);
	pSkbARPHdr->TargetIP = ndis_status.fIPAddress;
	
	// operation reply
	pSkbARPHdr->Operation =ntohs(0x0002);
	
	// send back this packet
	devdbg(g_cdc_dev,"ResponseArpPacket : send back the packet.\n"); 
	lc_skb_return_arp(g_cdc_dev, skb);
	
	return;
}

static bool  FilterArpPacket(UInt8 *pData, UInt32 packetlength)
{
    QC_ETH_HDR* pHdr = (QC_ETH_HDR*)pData;
	
	if(ETH_TYPE_ARP==ntohs(pHdr->EtherType))
	{
		devdbg (g_cdc_dev, "ETH_TYPE_ARP.");
		if (ndis_status.fIPAddress != 0)
		{
			ProcessArpPacket(pData, packetlength);
		}
		return true;
	}
	
	devdbg (g_cdc_dev, "not a arp package.");
	
	return false;
}
#endif
/* Passes this packet up the stack, updating its accounting.
 * Some link protocols batch packets, so their rx_fixup paths
 * can return clones as well as just modify the original skb.
 */
void lc_skb_return (struct lc_cdc_net *dev, struct sk_buff *skb)
{
	int	status;
	u32 	sn;

	devdbg(dev,"%s:......start.",__func__);
	if(skb->len > 128)
	{
		sn = be32_to_cpu(*(u32 *)(skb->data + 0x26));
		devdbg(dev,"lc_skb_return,len:%d receive sn:%x,  time:%ld-%ld",skb->len,sn,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);
	}	
	else
	{
		sn = be32_to_cpu(*(u32 *)(skb->data + 0x2a));
		devdbg(dev,"lc_skb_return,len:%d receive ack sn:%x,  time:%ld-%ld",skb->len,sn,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);
	}
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,26)
	skb->dev = dev->net;
#endif
	skb->data = skb->data -14;
	memcpy(skb->data,&fEthernetHeader,sizeof(fEthernetHeader));
	skb->len = skb->len +14;
//	printk_hex(skb->data,skb->len);
	skb->protocol = eth_type_trans (skb, dev->net);
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += skb->len;

	if (netif_msg_rx_status (dev))
		devdbg(g_cdc_dev,"< rx, len %zu, type 0x%x.\n",
			skb->len + sizeof (struct ethhdr), skb->protocol);
	memset (skb->cb, 0, sizeof (struct skb_data));
	status = netif_rx (skb);
	if (status != NET_RX_SUCCESS && netif_msg_rx_err (dev))
		devdbg(g_cdc_dev,"netif_rx status %d.\n", status);
}
#ifdef PROCESS_ARP_PACKAGE
static void lc_skb_return_arp (struct lc_cdc_net *dev, struct sk_buff *skb)
{
	int	status;
	u32 	sn;

	devdbg(dev,"%s:......start.",__func__);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,26)
	skb->dev = dev->net;
#endif	
	if(skb->len > 128)
	{
		sn = be32_to_cpu(*(u32 *)(skb->data + 0x26));
		devdbg(dev,"lc_skb_return,len:%d receive sn:%x,  time:%ld-%ld",skb->len,sn,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);
	}	
	else
	{
		sn = be32_to_cpu(*(u32 *)(skb->data + 0x2a));
		devdbg(dev,"lc_skb_return,len:%d receive ack sn:%x,  time:%ld-%ld",skb->len,sn,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);
	}
	skb->protocol = eth_type_trans (skb, dev->net);
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += skb->len;

	if (netif_msg_rx_status (dev))
		devdbg(g_cdc_dev,"< rx, len %zu, type 0x%x.\n",
			skb->len + sizeof (struct ethhdr), skb->protocol);
	memset (skb->cb, 0, sizeof (struct skb_data));
	status = netif_rx (skb);
	if (status != NET_RX_SUCCESS && netif_msg_rx_err (dev))
		devdbg(g_cdc_dev,"netif_rx status %d.\n", status);
}
#endif
EXPORT_SYMBOL_GPL(lc_skb_return);

// unlink pending rx/tx; completion handlers do all other cleanup

static int unlink_urbs (struct lc_cdc_net *dev, struct sk_buff_head *q)
{
	unsigned long		flags;
	struct sk_buff		*skb, *skbnext;
	int			count = 0;

	spin_lock_irqsave (&q->lock, flags);
	for (skb = q->next; skb != (struct sk_buff *) q; skb = skbnext) {
		struct skb_data		*entry;
		struct urb		*urb;
		int			retval;

		entry = (struct skb_data *) skb->cb;
		urb = entry->urb;
		skbnext = skb->next;

		// during some PM-driven resume scenarios,
		// these (async) unlinks complete immediately
		retval = usb_unlink_urb (urb);
		if (retval != -EINPROGRESS && retval != 0)
			devdbg (dev, "unlink urb err, %d", retval);
		else
			count++;
	}
	spin_unlock_irqrestore (&q->lock, flags);
	return count;
}


// Flush all pending rx urbs
// minidrivers may need to do this when the MTU changes

void lc_unlink_rx_urbs(struct lc_cdc_net *dev)
{
	if (netif_running(dev->net)) {
		(void) unlink_urbs (dev, &dev->rxq);
		tasklet_schedule(&dev->bh);
	}
}
EXPORT_SYMBOL_GPL(lc_unlink_rx_urbs);

/*-------------------------------------------------------------------------
 *
 * Network Device Driver (peer link to "Host Device", from USB host)
 *
 *-------------------------------------------------------------------------*/

static int lc_change_mtu (struct net_device *net, int new_mtu)
{
	struct lc_cdc_net	*dev = netdev_priv(net);
	int		ll_mtu = new_mtu + net->hard_header_len;
	int		old_hard_mtu = dev->hard_mtu;
	int		old_rx_urb_size = dev->rx_urb_size;

//	printk("%s............................%d.\n",__func__,__LINE__);
	if (new_mtu <= 0)
		return -EINVAL;
	// no second zero-length packet read wanted after mtu-sized packets
	if ((ll_mtu % dev->maxpacket) == 0)
		return -EDOM;
	net->mtu = new_mtu;

	dev->hard_mtu = net->mtu + net->hard_header_len;
	if (dev->rx_urb_size == old_hard_mtu) {
		dev->rx_urb_size = dev->hard_mtu;
		if (dev->rx_urb_size > old_rx_urb_size)
			lc_unlink_rx_urbs(dev);
	}

	devdbg(g_cdc_dev,"change mtu :%d, urb_size:%d",new_mtu,dev->rx_urb_size);

	return 0;
}

/*-------------------------------------------------------------------------*/
//#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30)
static struct net_device_stats *lc_get_stats (struct net_device *net)
{
	struct lc_cdc_net	*dev = netdev_priv(net);
	return &dev->stats;
}
//#endif
/*-------------------------------------------------------------------------*/

static void tx_defer_bh(struct lc_cdc_net *dev, struct sk_buff *skb, struct sk_buff_head *list)
{
	unsigned long		flags;

	spin_lock_irqsave(&list->lock, flags);
	__skb_unlink(skb, list);
	spin_unlock(&list->lock);
	spin_lock(&dev->done.lock);
	__skb_queue_tail(&dev->done, skb);
	if (1 <= dev->done.qlen)
		tasklet_schedule(&dev->bh);
	spin_unlock_irqrestore(&dev->done.lock, flags);
}
static void rx_defer_bh(struct lc_cdc_net *dev, struct sk_buff *skb, struct sk_buff_head *list)
{
	unsigned long		flags;
	spin_lock_irqsave(&list->lock, flags);
	__skb_unlink(skb, list);
	spin_unlock_irqrestore(&list->lock, flags);
	
	{
		spin_lock_irqsave(&dev->done.lock, flags);
		__skb_queue_tail(&dev->done, skb);
		if (1 <= dev->done.qlen){
			tasklet_schedule(&dev->bh);
		}
		spin_unlock_irqrestore(&dev->done.lock, flags);
	}
}
////////////////////////

/* some work can't be done in tasklets, so we use keventd
 *
 * NOTE:  annoying asymmetry:  if it's active, schedule_work() fails,
 * but tasklet_schedule() doesn't.  hope the failure is rare.
 */
void lc_defer_kevent (struct lc_cdc_net *dev, int work)
{
	set_bit (work, &dev->flags);
	if (!schedule_work (&dev->kevent))
		deverr (dev, "kevent %d may have been dropped", work);
	else
		devdbg (dev, "kevent %d scheduled", work);
}
EXPORT_SYMBOL_GPL(lc_defer_kevent);

/*-------------------------------------------------------------------------*/




static void rx_complete (struct urb *urb);
static void rx_submit (struct lc_cdc_net *dev, struct urb *urb, gfp_t flags)
{
	struct sk_buff		*skb;
	struct skb_data		*entry;
	int			retval = 0;
	unsigned long		lockflags;
	size_t			size = dev->rx_urb_size;
//	UInt8  

	devdbg(dev,"%s.....started.",__func__);
	if ((skb = alloc_skb (size +14+ NET_IP_ALIGN, flags)) == NULL) {
		deverr (dev, "no rx skb");
		lc_defer_kevent (dev, EVENT_RX_MEMORY);
		usb_free_urb (urb);
		return;
	}
//	memcpy(skb->data+2,&fEthernetHeader,sizeof(fEthernetHeader));
	skb_reserve (skb, 14+NET_IP_ALIGN);

	entry = (struct skb_data *) skb->cb;
	entry->urb = urb;
	entry->dev = dev;
	entry->state = rx_start;
	entry->length = 0;

	devdbg(dev,"%s:get data size=%d.",__func__,size);
//	printk_hex((void*)skb->data,size);//memcpy(fEthernetHeader.DstMacAddress, fCurMacAddress, ETH_LENGTH_OF_ADDRESS);
	
	usb_fill_bulk_urb (urb, dev->udev, dev->in,
		skb->data, size, rx_complete, skb);
	

	spin_lock_irqsave (&dev->rxq.lock, lockflags);


	if (netif_running (dev->net)
			&& netif_device_present (dev->net)
			&& !test_bit (EVENT_RX_HALT, &dev->flags)) {
		switch (retval = usb_submit_urb (urb, GFP_ATOMIC)) {

		case 0://submit successfully
			__skb_queue_tail (&dev->rxq, skb);
			break;
		case -EPIPE:
			lc_defer_kevent (dev, EVENT_RX_HALT);
			break;
		case -ENOMEM:
			lc_defer_kevent (dev, EVENT_RX_MEMORY);
			break;
		case -ENODEV:
			if (netif_msg_ifdown (dev))
				printk ("%s:device gone.",__func__);
			netif_device_detach (dev->net);
			break;
		default:
			if (netif_msg_rx_err (dev))
				printk ("%s:rx submit, %d",__func__, retval);
			tasklet_schedule (&dev->bh);
			break;
		}
	} else {
		if (netif_msg_ifdown (dev))
			printk ("%s:rx: stopped",__func__);
		retval = -ENOLINK;
	}
	spin_unlock_irqrestore (&dev->rxq.lock, lockflags);
	
	devdbg(dev,"usb_submit_urb status:%x, time:%ld-%ld.",retval,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);

	if (retval) {

		dev_kfree_skb_any (skb);
		usb_free_urb (urb);
	}
}

/*-------------------------------------------------------------------------*/

static inline void rx_process (struct lc_cdc_net *dev, struct sk_buff *skb)
{
	//devdbg(dev,"get into %s .",__func__);

    //skb->len = skb->len;
    devdbg(dev,"%s: stk->len=%d.",__func__,skb->len);
 /*   if(skb->len<500&&skb->len>10)
    {
		printk_hex(skb->data,skb->len);
	}*/
	if (skb->len){
		lc_skb_return (dev, skb);
	}
	else {
		if (netif_msg_rx_err (dev))
			devdbg (dev, "drop");
		dev->stats.rx_errors++;
		skb_queue_tail (&dev->done, skb);
	}
}

/*-------------------------------------------------------------------------*/
static void rx_complete (struct urb *urb)
{
	struct sk_buff		*skb = (struct sk_buff *) urb->context;
	struct skb_data		*entry = (struct skb_data *) skb->cb;
	struct lc_cdc_net		*dev = entry->dev;
	int			urb_status = urb->status;

    devdbg(g_cdc_dev,"rx_complete,urb:%p,rx length %d, time %ld-%ld.\n",urb, urb->actual_length,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);
	skb_put (skb, urb->actual_length);
	entry->state = rx_done;
	entry->urb = NULL;

	switch (urb_status) {
	/* success */
	case 0:
		if (skb->len < dev->net->hard_header_len) {
			entry->state = rx_cleanup;
			dev->stats.rx_errors++;
			dev->stats.rx_length_errors++;
			if (netif_msg_rx_err (dev))
				devdbg (dev, "rx length %d", skb->len);
		}
		break;

	/* stalls need manual reset. this is rare ... except that
	 * when going through USB 2.0 TTs, unplug appears this way.
	 * we avoid the highspeed version of the ETIMEOUT/EILSEQ
	 * storm, recovering as needed.
	 */
	case -EPIPE:
		dev->stats.rx_errors++;
		lc_defer_kevent (dev, EVENT_RX_HALT);
		// FALLTHROUGH

	/* software-driven interface shutdown */
	case -ECONNRESET:		/* async unlink */
	case -ESHUTDOWN:		/* hardware gone */
		if (netif_msg_ifdown (dev))
			devdbg (dev, "rx shutdown, code %d", urb_status);
		goto block;

	/* we get controller i/o faults during khubd disconnect() delays.
	 * throttle down resubmits, to avoid log floods; just temporarily,
	 * so we still recover when the fault isn't a khubd delay.
	 */
	case -EPROTO:
	case -ETIME:
	case -EILSEQ:
		dev->stats.rx_errors++;
		if (!timer_pending (&dev->delay)) {
			mod_timer (&dev->delay, jiffies + THROTTLE_JIFFIES);
			if (netif_msg_link (dev))
				devdbg (dev, "rx throttle %d", urb_status);
		}
block:
		entry->state = rx_cleanup;
		entry->urb = urb;
		urb = NULL;
		break;

	/* data overrun ... flush fifo? */
	case -EOVERFLOW:
		dev->stats.rx_over_errors++;
		// FALLTHROUGH

	default:
		entry->state = rx_cleanup;
		dev->stats.rx_errors++;
		if (netif_msg_rx_err (dev))
			devdbg (dev, "rx status %d", urb_status);
		break;
	}

	rx_defer_bh(dev, skb, &dev->rxq);

	if (urb) {
		if (netif_running (dev->net)
				&& !test_bit (EVENT_RX_HALT, &dev->flags)) {
			rx_submit (dev, urb, GFP_ATOMIC);
			return;
		}
		usb_free_urb (urb);
	}
	if (netif_msg_rx_err (dev))
		printk ("no read resubmitted.\n");
}
#ifdef NDIS_INIT_STATUS
static void intr_complete (struct urb *urb)
{
	struct lc_cdc_net	*dev = urb->context;
	int		status = urb->status;
	
	devdbg(g_cdc_dev,"%s:...init status complete.\n",__func__);
	switch (status) {
	/* success */
	case 0:
		lc_cdc_status(dev, urb);
		break;

	/* software-driven interface shutdown */
	case -ENOENT:		/* urb killed */
	case -ESHUTDOWN:	/* hardware gone */
		if (netif_msg_ifdown (dev))
			devdbg (dev, "intr shutdown, code %d", status);
		return;

	/* NOTE:  not throttling like RX/TX, since this endpoint
	 * already polls infrequently
	 */
	default:
		devdbg(g_cdc_dev,"intr status %d.\n", status);
		break;
	}

	if (!netif_running (dev->net))
		return;

	memset(urb->transfer_buffer, 0, urb->transfer_buffer_length);
	status = usb_submit_urb (urb, GFP_ATOMIC);
	if (status != 0 && netif_msg_timer (dev))
		devdbg(g_cdc_dev,"intr resubmit --> %d.\n", status);
}
#endif
/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/

// precondition: never called in_interrupt

static int lc_stop (struct net_device *net)
{
	struct lc_cdc_net		*dev = netdev_priv(net);
	int			temp;
	DECLARE_WAIT_QUEUE_HEAD_ONSTACK (unlink_wakeup);
	DECLARE_WAITQUEUE (wait, current);

	netif_stop_queue (net);

	if (netif_msg_ifdown (dev))
		devdbg(g_cdc_dev,"stop stats: rx/tx %ld/%ld, errs %ld/%ld",
			dev->stats.rx_packets, dev->stats.tx_packets,
			dev->stats.rx_errors, dev->stats.tx_errors
			);

	// ensure there are no more active urbs
	add_wait_queue (&unlink_wakeup, &wait);
	dev->wait = &unlink_wakeup;
	temp = unlink_urbs (dev, &dev->txq) + unlink_urbs (dev, &dev->rxq);

	// maybe wait for deletions to finish.
	while (!skb_queue_empty(&dev->rxq)
			&& !skb_queue_empty(&dev->txq)
			&& !skb_queue_empty(&dev->done)) {
		msleep(UNLINK_TIMEOUT_MS);
		if (netif_msg_ifdown (dev))
			devdbg(g_cdc_dev,"waited for %d urb completions", temp);
	}
	dev->wait = NULL;
	remove_wait_queue (&unlink_wakeup, &wait);	

	usb_kill_urb(dev->interrupt);

	/* deferred work (task, timer, softirq) must also stop.
	 * can't flush_scheduled_work() until we drop rtnl (later),
	 * else workers could deadlock; so make workers a NOP.
	 */
	dev->flags = 0;
	del_timer_sync (&dev->delay);
	tasklet_kill (&dev->bh);
	usb_autopm_put_interface(dev->intf);

	return 0;
}

/*-------------------------------------------------------------------------*/

// posts reads, and enables write queuing

// precondition: never called in_interrupt

static int lc_open (struct net_device *net)
{
	struct lc_cdc_net		*dev = netdev_priv(net);
	int			retval;
	if ((retval = usb_autopm_get_interface(dev->intf)) < 0) {
		if (netif_msg_ifup (dev))
			devdbg(g_cdc_dev,"resumption fail (%d) lc_cdc_net usb-%s-%s, %s",
				retval,
				dev->udev->bus->bus_name, dev->udev->devpath,
			dev->driver_desc);
		goto done_nopm;
	}
	
	/* start any status interrupt transfer */
	if (dev->interrupt) {
		retval = usb_submit_urb (dev->interrupt, GFP_KERNEL);
		if (retval < 0) {
			if (netif_msg_ifup (dev))
				deverr (dev, "intr submit %d", retval);
			goto done;
		}
	}
	
	netif_start_queue (net);

	// delay posting reads until we're fully open
	tasklet_schedule (&dev->bh);
	return retval;
done:
	usb_autopm_put_interface(dev->intf);
done_nopm:
	return retval;
}

/*-------------------------------------------------------------------------*/

/* ethtool methods; minidrivers may need to add some more, but
 * they'll probably want to use this base set.
 */

int lc_get_settings (struct net_device *net, struct ethtool_cmd *cmd)
{
	struct lc_cdc_net *dev = netdev_priv(net);

	if (!dev->mii.mdio_read)
		return -EOPNOTSUPP;

	return mii_ethtool_gset(&dev->mii, cmd);
}
EXPORT_SYMBOL_GPL(lc_get_settings);

int lc_set_settings (struct net_device *net, struct ethtool_cmd *cmd)
{
	struct lc_cdc_net *dev = netdev_priv(net);
	int retval;

	if (!dev->mii.mdio_write)
		return -EOPNOTSUPP;

	retval = mii_ethtool_sset(&dev->mii, cmd);

	return retval;

}
EXPORT_SYMBOL_GPL(lc_set_settings);

u32 lc_get_link (struct net_device *net)
{
	struct lc_cdc_net *dev = netdev_priv(net);

	/* if the device has mii operations, use those */
	if (dev->mii.mdio_read)
		return mii_link_ok(&dev->mii);

	/* Otherwise, say we're up (to avoid breaking scripts) */
	return 1;
}
EXPORT_SYMBOL_GPL(lc_get_link);

int lc_nway_reset(struct net_device *net)
{
	struct lc_cdc_net *dev = netdev_priv(net);

	if (!dev->mii.mdio_write)
		return -EOPNOTSUPP;

	return mii_nway_restart(&dev->mii);
}
EXPORT_SYMBOL_GPL(lc_nway_reset);

void lc_get_drvinfo (struct net_device *net, struct ethtool_drvinfo *info)
{
	struct lc_cdc_net *dev = netdev_priv(net);

	strncpy (info->driver, dev->driver_name, sizeof info->driver);
	strncpy (info->version, DRIVER_VERSION, sizeof info->version);
	strncpy (info->fw_version, dev->driver_desc,
		sizeof info->fw_version);
	usb_make_path (dev->udev, info->bus_info, sizeof info->bus_info);
}
EXPORT_SYMBOL_GPL(lc_get_drvinfo);

u32 lc_get_msglevel (struct net_device *net)
{
	struct lc_cdc_net *dev = netdev_priv(net);

	return dev->msg_enable;
}
EXPORT_SYMBOL_GPL(lc_get_msglevel);

void lc_set_msglevel (struct net_device *net, u32 level)
{
	struct lc_cdc_net *dev = netdev_priv(net);

	dev->msg_enable = level;
}
EXPORT_SYMBOL_GPL(lc_set_msglevel);

/* drivers may override default ethtool_ops in their bind() routine */
static struct ethtool_ops lc_ethtool_ops = {
	.get_settings		= lc_get_settings,
	.set_settings		= lc_set_settings,
	.get_link		= lc_get_link,
	.nway_reset		= lc_nway_reset,
	.get_drvinfo		= lc_get_drvinfo,
	.get_msglevel		= lc_get_msglevel,
	.set_msglevel		= lc_set_msglevel,
};

/*-------------------------------------------------------------------------*/

/* work that cannot be done in interrupt context uses keventd.
 *
 * NOTE:  with 2.5 we could do more of this using completion callbacks,
 * especially now that control transfers can be queued.
 */
static void
kevent (struct work_struct *work)
{
	struct lc_cdc_net		*dev =
		container_of(work, struct lc_cdc_net, kevent);
	int			status;

	/* usb_clear_halt() needs a thread context */
	devdbg(dev,"%s start.....",__func__);
	if (test_bit (EVENT_TX_HALT, &dev->flags)) {
		unlink_urbs (dev, &dev->txq);
		status = usb_clear_halt (dev->udev, dev->out);
		if (status < 0
				&& status != -EPIPE
				&& status != -ESHUTDOWN) {
			if (netif_msg_tx_err (dev))
				deverr (dev, "can't clear tx halt, status %d",
					status);
		} else {
			clear_bit (EVENT_TX_HALT, &dev->flags);
			if (status != -ESHUTDOWN)
				netif_wake_queue (dev->net);
		}
	}
	if (test_bit (EVENT_RX_HALT, &dev->flags)) {
		unlink_urbs (dev, &dev->rxq);
		status = usb_clear_halt (dev->udev, dev->in);
		if (status < 0
				&& status != -EPIPE
				&& status != -ESHUTDOWN) {
			if (netif_msg_rx_err (dev))
				deverr (dev, "can't clear rx halt, status %d",
					status);
		} else {
			clear_bit (EVENT_RX_HALT, &dev->flags);
			tasklet_schedule (&dev->bh);
		}
	}

	/* tasklet could resubmit itself forever if memory is tight */
	if (test_bit (EVENT_RX_MEMORY, &dev->flags)) {
		struct urb	*urb = NULL;

		if (netif_running (dev->net))
			urb = usb_alloc_urb (0, GFP_KERNEL);
		else
			clear_bit (EVENT_RX_MEMORY, &dev->flags);
		if (urb != NULL) {
			clear_bit (EVENT_RX_MEMORY, &dev->flags);
			rx_submit (dev, urb, GFP_KERNEL);
			tasklet_schedule (&dev->bh);
		}
	}

	if (test_bit (EVENT_LINK_RESET, &dev->flags)) {
		clear_bit (EVENT_LINK_RESET, &dev->flags);
	}

	if (dev->flags)
		devdbg (dev, "kevent done, flags = 0x%lx",
			dev->flags);
}

/*-------------------------------------------------------------------------*/

static void tx_complete (struct urb *urb)
{
	struct sk_buff		*skb = (struct sk_buff *) urb->context;
	struct skb_data		*entry = (struct skb_data *) skb->cb;
	struct lc_cdc_net		*dev = entry->dev;

    devdbg(dev,"tx_complete,status:%d,len:%d, *********time:%ld-%ld.",urb->status,entry->length,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);

	if (urb->status == 0) {
		dev->stats.tx_packets++;
		dev->stats.tx_bytes += entry->length;
	} else {
		dev->stats.tx_errors++;
		devdbg(dev,"%s:%d.",__func__,__LINE__);
		switch (urb->status) {
		case -EPIPE:
			lc_defer_kevent (dev, EVENT_TX_HALT);
			devdbg(dev,"%s:%d.",__func__,__LINE__);
			break;

		/* software-driven interface shutdown */
		case -ECONNRESET:		// async unlink
		case -ESHUTDOWN:		// hardware gone
			devdbg(dev,"%s:%d.",__func__,__LINE__);
			break;

		// like rx, tx gets controller i/o faults during khubd delays
		// and so it uses the same throttling mechanism.
		case -EPROTO:
		case -ETIME:
		case -EILSEQ:
			if (!timer_pending (&dev->delay)) {
				mod_timer (&dev->delay,
					jiffies + THROTTLE_JIFFIES);
				if (netif_msg_link (dev))
					devdbg(dev, "tx throttle %d",
							urb->status);
			}
			netif_stop_queue (dev->net);
			devdbg(dev,"%s:%d.",__func__,__LINE__);
			break;
		default:
			if (netif_msg_tx_err (dev))
				devdbg(dev,"tx err %d.", entry->urb->status);
			break;
		}
	}

	urb->dev = NULL;
	entry->state = tx_done;
	tx_defer_bh(dev, skb, &dev->txq);
}

/*-------------------------------------------------------------------------*/

static void lc_tx_timeout (struct net_device *net)
{
	struct lc_cdc_net		*dev = netdev_priv(net);

	unlink_urbs (dev, &dev->txq);
	tasklet_schedule (&dev->bh);

	// FIXME: device recovery -- reset?
}

/*-------------------------------------------------------------------------*/

static int lc_start_xmit (struct sk_buff *skb, struct net_device *net)
{
	struct lc_cdc_net		*dev = netdev_priv(net);
	int			length;
	int			retval = NET_XMIT_SUCCESS;
	struct urb		*urb = NULL;
	struct skb_data		*entry;
	unsigned long		flags;
  //  bool process_arp = false;
   
	length = skb->len;
#ifdef PROCESS_ARP_PACKAGE	
	if(ndis_status.fIPAddress!=0&&FilterArpPacket(skb->data,skb->len))
	{
		goto drop;
	}
#endif	
	if (!(urb = usb_alloc_urb (0, GFP_ATOMIC))) {
		if (netif_msg_tx_err (dev))
			devdbg (dev, "no urb");
		goto drop;
	}

	entry = (struct skb_data *) skb->cb;
	entry->urb = urb;
	entry->dev = dev;
	entry->state = tx_start;
	entry->length = length;
	
	if(ndis_status.fIPAddress!=0)
	{	
		usb_fill_bulk_urb (urb, dev->udev, dev->out,
			skb->data+14, skb->len-14, tx_complete, skb);
	}else
	{
		usb_fill_bulk_urb (urb, dev->udev, dev->out,
			skb->data, skb->len, tx_complete, skb);
	}

	/* don't assume the hardware handles USB_ZERO_PACKET
	 * NOTE:  strictly conforming cdc-ether devices should expect
	 * the ZLP here, but ignore the one-byte packet.
	 */
	if ((length % dev->maxpacket) == 0) {
		urb->transfer_buffer_length++;
		if (skb_tailroom(skb)) {
			skb->data[skb->len] = 0;
			__skb_put(skb, 1);
		}
	}

	devdbg(dev,"lc_start_xmit ,usb_submit_urb,len:%d, time:%ld-%ld.",skb->len,current_kernel_time().tv_sec,current_kernel_time().tv_nsec);

	spin_lock_irqsave (&dev->txq.lock, flags);

	switch ((retval = usb_submit_urb (urb, GFP_ATOMIC))) {
	case -EPIPE:
		netif_stop_queue (net);
		lc_defer_kevent (dev, EVENT_TX_HALT);
		devdbg(dev,"%s:%d.",__func__,__LINE__);
		break;
	default:
		if (netif_msg_tx_err (dev))
		{
			printk ("tx: submit urb err %d.\n", retval);
		}
		devdbg(dev,"%s:%d.",__func__,__LINE__);
		break;
	case 0:
		net->trans_start = jiffies;
		__skb_queue_tail (&dev->txq, skb);
		if (dev->txq.qlen >= TX_QLEN (dev))
		{
			netif_stop_queue (net);
			devdbg(dev,"%s:%d.",__func__,__LINE__);
		}
	}
	spin_unlock_irqrestore (&dev->txq.lock, flags);

	if (retval) {
		if (netif_msg_tx_err (dev))
			devdbg(dev,"drop, code %d", retval);
drop:
		retval = NET_XMIT_SUCCESS;
		dev->stats.tx_dropped++;
		if (skb)
			dev_kfree_skb_any (skb);
		usb_free_urb (urb);
	} else if (netif_msg_tx_queued (dev)) {
		devdbg (dev, "> tx, len %d, type 0x%x",
			length, skb->protocol);
	}
	return retval;
}


/*-------------------------------------------------------------------------*/

// tasklet (work deferred from completions, in_irq) or timer

static void lc_bh (unsigned long param)
{
	struct lc_cdc_net		*dev = (struct lc_cdc_net *) param;
	struct sk_buff		*skb;
	struct skb_data		*entry;
	
	devdbg(dev,"get into ..........%s.",__func__);
	while ((skb = skb_dequeue (&dev->done))) {
		entry = (struct skb_data *) skb->cb;
		devdbg(dev,"%s:entry->state=%d.",__func__,entry->state);
		switch (entry->state) {
		case rx_done:
			entry->state = rx_cleanup;
			rx_process (dev, skb);
			continue;
		case tx_done:
		case rx_cleanup:
			usb_free_urb (entry->urb);
			dev_kfree_skb (skb);
			continue;
		default:
			printk ("%s:bogus skb state %d.\n", __func__,entry->state);
		}
	}

	// waiting for all pending urbs to complete?
	if (dev->wait) {
		devdbg(dev,"%s.............%d.",__func__,__LINE__);
		if ((dev->txq.qlen + dev->rxq.qlen + dev->done.qlen) == 0) {
			wake_up (dev->wait);
		}

	// or are we maybe short a few urbs?
	} else if (netif_running (dev->net)
			&& netif_device_present (dev->net)
			&& !timer_pending (&dev->delay)
			&& !test_bit (EVENT_RX_HALT, &dev->flags)) {
		int	temp = dev->rxq.qlen;
		int	qlen = RX_QLEN (dev);
		devdbg(dev,"%s.............%d.",__func__,__LINE__);

		if (temp < qlen) {
			struct urb	*urb;
			int		i;
			devdbg(dev,"%s.............%d.",__func__,__LINE__);
			// don't refill the queue all at once
			for (i = 0; i < 10 && dev->rxq.qlen < qlen; i++) {
				devdbg(dev,"%s.............%d.",__func__,__LINE__);
				urb = usb_alloc_urb (0, GFP_ATOMIC);
				if (urb != NULL)
					rx_submit (dev, urb, GFP_ATOMIC);
			}
			if (temp != dev->rxq.qlen && netif_msg_link (dev))
				printk ("rxqlen %d --> %d.\n",
						temp, dev->rxq.qlen);
			if (dev->rxq.qlen < qlen)
				tasklet_schedule (&dev->bh);
		}
		if (dev->txq.qlen < TX_QLEN (dev))
			netif_wake_queue (dev->net);
	}
}


/*-------------------------------------------------------------------------
 *
 * USB Device Driver support
 *
 *-------------------------------------------------------------------------*/

// precondition: never called in_interrupt

void lc_disconnect (struct usb_interface *intf)
{
	struct lc_cdc_net		*dev;
	struct usb_device	*xdev;
	struct net_device	*net;

	dev = usb_get_intfdata(intf);
	usb_set_intfdata(intf, NULL);
	if (!dev)
		return;

	xdev = interface_to_usbdev (intf);

	if (netif_msg_probe (dev))
		devinfo (dev, "unregister '%s' usb-%s-%s, %s",
			intf->dev.driver->name,
			xdev->bus->bus_name, xdev->devpath,
			dev->driver_desc);
	
	ndis_release_client_ID(g_cdc_dev);
#if 0
	cancel_delayed_work_sync(&dev->status_work);
#endif
	net = dev->net;	

	/* we don't hold rtnl here ... */
	flush_scheduled_work ();

	lc_cdc_unbind(dev, intf);
	unregister_netdev (net);
	free_netdev(net);
	usb_put_dev (xdev);
}
EXPORT_SYMBOL_GPL(lc_disconnect);

static int lc_ndis_ioctl(struct net_device *dev,struct ifreq *req, int cmd)
{
	ndis_command_t* pndis_req = NULL;
	int ret = -1;
	//printk("..............................\n");
	devdbg(dev,"get into %s,cmd=%d.",__func__,cmd);
	
	if(!netif_running(dev)||cmd!=SIOCDEVPRIVATE)
		return -EINVAL;
	
	pndis_req = (ndis_command_t*)req->ifr_data;
	switch(pndis_req->cmd)
	{
		case NDIS_CMD_INIT_STATUS:
		{
			lc_get_qmi_status(g_cdc_dev->intf);
			if(g_cdc_dev->qmi_sync==0)
			{
				lc_cdc_check_status_work();
			}
		}
		break;
		case NDIS_CMD_CONNECT_CMD:
			{
				WWAN_CONNECT_PARAMS* parms = NULL;
				
				devdbg(dev,"%s:connect ioctl.",__func__);
				
				parms = (WWAN_CONNECT_PARAMS*)pndis_req->data;
				devdbg(dev,"%s:connect ioctl. connect mode %d",__func__,parms->ConnectIpMode);
				ret = ndis_connect(g_cdc_dev,parms->AccessString,parms->UserName,
							parms->Password,parms->Commpression,parms->ConnectIpMode);	
				//printk("..............\n");
				copy_to_user((void *)req->ifr_data,&(ndis_status.fConnectionError),sizeof(ndis_status.fConnectionError));			
			}				
			break;
		case NDIS_CMD_DISCONN_CMD:
			{
				devdbg(dev,"%s:disconnect ioctl.",__func__);
				ndis_disconnect(g_cdc_dev);
			}
			break;
		case NDIS_CMD_GET_VERSION:
			{
				devdbg(dev,"%s:get version ioctl.",__func__);
				copy_to_user((void *)req->ifr_data,ndis_status.fVersion,sizeof(ndis_status.fVersion));
			}
			break;
		case NDIS_CMD_GET_STATUS:
			{
				WWAN_STATUS  m_status;
				devdbg(dev,"%s:get status ioctl.",__func__);
				lc_get_qmi_status(g_cdc_dev->intf);
				m_status.connection_status = ndis_status.fConnectionStatus;
				m_status.ip_address = ndis_status.fIPAddress;
				copy_to_user((void *)req->ifr_data,&m_status,sizeof(WWAN_STATUS));
			}
			break;
		default:
			devdbg(dev,"unsupport cmmmand%d.",cmd);
	}
	return 0;
}

/*-------------------------------------------------------------------------*/
#if !(LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30))
static int lc_eth_mac_addr(struct net_device *dev, void *p)
{
	memcpy(dev->dev_addr,fEthernetHeader.SrcMacAddress,ETH_LENGTH_OF_ADDRESS);

	return 0;
}
static const struct net_device_ops lc_netdev_ops = {
	.ndo_open = lc_open,
	.ndo_stop = lc_stop,
	.ndo_start_xmit = lc_start_xmit,
	.ndo_tx_timeout = lc_tx_timeout,
	.ndo_change_mtu = lc_change_mtu,
	.ndo_do_ioctl =  lc_ndis_ioctl,
	.ndo_set_mac_address = lc_eth_mac_addr,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_get_stats = lc_get_stats, 
};
#endif

// precondition: never called in_interrupt
int lc_check_conn_status(struct usb_interface *intf);


int
lc_cdc_probe (struct usb_interface *udev, const struct usb_device_id *prod)
{
	struct lc_cdc_net			*dev;
	struct net_device		*net;
	struct usb_host_interface	*interface;
	struct usb_device		*xdev;
	int				status;
	const char			*name;
//	DECLARE_MAC_BUF(mac);
	name = udev->dev.driver->name;
	xdev = interface_to_usbdev (udev);
	interface = udev->cur_altsetting;

	usb_get_dev (xdev);

	status = -ENOMEM;

	// set up our own records
	net = alloc_etherdev(sizeof(*dev));
	if (!net) {
		dbg ("can't kmalloc dev");
		goto out;
	}

	dev = netdev_priv(net);
	g_cdc_dev = dev;
	dev->udev = xdev;
	dev->intf = udev;
/* Add for DTS2011050903736 lxz 20110520 start*/
/* linux kernel > 2.6.37: PowerManager needs disable_depth ==0 */
#ifdef  CONFIG_PM_RUNTIME
        if(LINUX_VERSION37_LATER)
        {
          dev->intf->dev.power.disable_depth = 0;
        }
#endif
/* Add for DTS2011050903736 lxz 20110520 end*/

	dev->driver_name = name;
	dev->driver_desc = "Longcheer Ethernet Device";
	dev->msg_enable = netif_msg_init (msg_level, NETIF_MSG_DRV
				| NETIF_MSG_PROBE | NETIF_MSG_LINK);
	skb_queue_head_init (&dev->rxq);
	skb_queue_head_init (&dev->txq);
	skb_queue_head_init (&dev->done);
	dev->bh.func = lc_bh;
	dev->bh.data = (unsigned long) dev;
	INIT_WORK (&dev->kevent, kevent);
	dev->delay.function = lc_bh;
	dev->delay.data = (unsigned long) dev;
	init_timer (&dev->delay);
	mutex_init (&dev->phy_mutex);

	dev->net = net;
	//strcpy (net->name, "eth%d");
	memcpy (net->dev_addr, node_id, sizeof node_id);

	/* rx and tx sides can use different message sizes;
	 * bind() should set rx_urb_size in that case.
	 */
	dev->hard_mtu = net->mtu + net->hard_header_len;

#if !(LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 30))
	net->netdev_ops = &lc_netdev_ops;
#else
	net->change_mtu = lc_change_mtu;
	net->get_stats = lc_get_stats;
	net->hard_start_xmit = lc_start_xmit;
	net->open = lc_open;
	net->stop = lc_stop;
	net->do_ioctl = lc_ndis_ioctl;
	net->tx_timeout = lc_tx_timeout;
#endif
	net->watchdog_timeo = TX_TIMEOUT_JIFFIES;
	net->ethtool_ops = &lc_ethtool_ops;

	
	status = lc_cdc_bind (dev, udev);
	if (status < 0)
		goto out1;

	
	strcpy (net->name, "wan%d");
	
//	printk("%s........net->hard_header_len=%d",__func__,net->hard_header_len);
	/* maybe the remote can't receive an Ethernet MTU */
	if (net->mtu > (dev->hard_mtu - net->hard_header_len))
		net->mtu = dev->hard_mtu - net->hard_header_len;
		
#ifdef NDIS_INIT_STATUS
	if (status >= 0 && dev->status)
		status = init_status (dev, udev);
	if (status < 0)
		goto out3;
#endif
 
	dev->rx_urb_size = net->mtu;

	dev->maxpacket = usb_maxpacket (dev->udev, dev->out, 1);
	devdbg(dev,"%s:dev->rx_urb_size=%u,dev->maxpacket=%u.\n",__func__,dev->rx_urb_size,dev->maxpacket);
	SET_NETDEV_DEV(net, &udev->dev);
	status = register_netdev (net);
	if (status)
		goto out3;
	// ok, it's ready to go.
	usb_set_intfdata (udev, dev);

	netif_device_attach (net);

	//kernel_thread(lc_check_conn_status, (void *)net, 0);
	
	/*set the carrier off as default*/
	netif_carrier_off(net);
	{
		dev->qmi_sync = 0;
	//	INIT_DELAYED_WORK(&dev->status_work, lc_cdc_check_status_work);
    //		schedule_delayed_work(&dev->status_work, 40*HZ);
	}
	//lc_check_conn_status(udev);
	//
	memset(&ndis_status,0,sizeof(ndis_status));
	ndis_status.fConnectionStatus = LC_DISCONNECTED;	
	spin_lock_init(&qmi_request_lock);	
	return 0;

out3:
	lc_cdc_unbind (dev, udev);
out1:
	free_netdev(net);
out:
	usb_put_dev(xdev);
	return status;
}
EXPORT_SYMBOL_GPL(lc_cdc_probe);

/*-------------------------------------------------------------------------*/

/*
 * suspend the whole driver as soon as the first interface is suspended
 * resume only when the last interface is resumed
 */

int lc_suspend (struct usb_interface *intf, pm_message_t message)
{
	struct lc_cdc_net		*dev = usb_get_intfdata(intf);

	if (!dev->suspend_count++) {
		/*
		 * accelerate emptying of the rx and queues, to avoid
		 * having everything error out.
		 */
		netif_device_detach (dev->net);
		(void) unlink_urbs (dev, &dev->rxq);
		(void) unlink_urbs (dev, &dev->txq);
		/*
		 * reattach so runtime management can use and
		 * wake the device
		 */
		netif_device_attach (dev->net);
	}
	return 0;
}
EXPORT_SYMBOL_GPL(lc_suspend);

int lc_resume (struct usb_interface *intf)
{
	struct lc_cdc_net		*dev = usb_get_intfdata(intf);

	if (!--dev->suspend_count)
		tasklet_schedule (&dev->bh);

	return 0;
}
EXPORT_SYMBOL_GPL(lc_resume);

static int lc_cdc_reset_resume(struct usb_interface *intf)
{
	return lc_resume (intf);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * probes control interface, claims data interface, collects the bulk
 * endpoints, activates data interface (if needed), maybe sets MTU.
 * all pure cdc
 */
//int lc_generic_cdc_bind(struct lc_cdc_net *dev, struct usb_interface *intf)
#define USB_DEVICE_Longcheer_DATA 0xFF
static int lc_cdc_bind(struct lc_cdc_net *dev, struct usb_interface *intf)
{
//	u8				*buf = intf->cur_altsetting->extra;
//	int				len = intf->cur_altsetting->extralen;

	struct lc_dev_state		*info = (void *) &dev->data;
	int				status;

	devdbg(dev,"lc_cdc_bind enter.");
    
	if (sizeof dev->data < sizeof *info)
		return -EDOM;
		
	if (LC_INTERFACE_INDEX != intf->cur_altsetting->desc.bInterfaceNumber)
		return -EDOM;
		
	memcpy(fEthernetHeader.DstMacAddress, fCurMacAddress, ETH_LENGTH_OF_ADDRESS);
	memcpy(fEthernetHeader.SrcMacAddress, fDstMacAddress, ETH_LENGTH_OF_ADDRESS);
	fEthernetHeader.EtherType = htons(ETH_TYPE_IPV4);
	
	memset(info, 0, sizeof *info);

	info->data = usb_ifnum_to_if(dev->udev,intf->cur_altsetting->desc.bInterfaceNumber);
 
	status = lc_get_endpoints(dev, info->data);
	if (status < 0) {
		/* ensure immediate exit from lc_disconnect */
		goto error3;
	}

	return lc_get_ethernet_addr(dev);
    
error3:
	/* ensure immediate exit from cdc_disconnect */
	usb_set_intfdata(info->data, NULL);
	usb_driver_release_interface(driver_of(intf), info->data);
 
    return status; 
    
	devinfo(dev, "bad CDC descriptors\n");
	return -ENODEV;
}

void lc_cdc_unbind(struct lc_cdc_net *dev, struct usb_interface *intf)
{
	struct lc_dev_state		*info = (void *) &dev->data;
	struct usb_driver		*driver = driver_of(intf);

	/* disconnect master --> disconnect slave */
	if (intf == info->control && info->data) {
		/* ensure immediate exit from usbnet_disconnect */
		usb_set_intfdata(info->data, NULL);
		usb_driver_release_interface(driver, info->data);
		info->data = NULL;
	}

	/* and vice versa (just in case) */
	else if (intf == info->data && info->control) {
		/* ensure immediate exit from usbnet_disconnect */
		usb_set_intfdata(info->control, NULL);
		usb_driver_release_interface(driver, info->control);
		info->control = NULL;
	}    
    
}
EXPORT_SYMBOL_GPL(lc_cdc_unbind);


/*-------------------------------------------------------------------------
 *
 * Communications Device Class, Ethernet Control model
 *
 * Takes two interfaces.  The DATA interface is inactive till an altsetting
 * is selected.  Configuration data includes class descriptors.  There's
 * an optional status endpoint on the control interface.
 *
 * This should interop with whatever the 2.4 "CDCEther.c" driver
 * (by Brad Hards) talked with, with more functionality.
 *
 *-------------------------------------------------------------------------*/
#ifdef NDIS_INIT_STATUS
static void dumpspeed(struct lc_cdc_net *dev, __le32 *speeds)
{
	if (netif_msg_timer(dev))
		devinfo(dev, "link speeds: %u kbps up, %u kbps down",
			__le32_to_cpu(speeds[0]) / 1000,
		__le32_to_cpu(speeds[1]) / 1000);
}
#endif
static inline int lc_get_ethernet_addr(struct lc_cdc_net *dev)
{

	//dev->net->dev_addr[0] = 0x00;
	//dev->net->dev_addr[1] = 0x1e;

	//dev->net->dev_addr[2] = 0x10;
	//dev->net->dev_addr[3] = 0x1f;
	//dev->net->dev_addr[4] = 0x00;
	//dev->net->dev_addr[5] = 0x01;/*change 0x04 into 0x01 20100129*/
	
	dev->net->dev_addr[0] = 0x00;
	dev->net->dev_addr[1] = 0xA0;
	dev->net->dev_addr[2] = 0xC6;
	dev->net->dev_addr[3] = 0x00;
	dev->net->dev_addr[4] = 0x00;
	dev->net->dev_addr[5] = 0x00;

	return 0;
}

#define lc_CDC_OK 0
#define lc_CDC_FAIL -1

#define	LONGCHEER_INTERFACE_VDF \
	.bInterfaceClass	= 0xFF, \
	.bInterfaceSubClass	= 0xFF, \
	.bInterfaceProtocol	= 0xFF

#define	LONGCHEER_INTERFACE_RMNET_VDF \
	.bInterfaceClass	= 0xFF, \
	.bInterfaceSubClass	= 0x06, \
	.bInterfaceProtocol	= 0x00

static const struct usb_device_id	lc_products [] = {
	{
		.match_flags	=   USB_DEVICE_ID_MATCH_INT_INFO
			  | USB_DEVICE_ID_MATCH_VENDOR,
		.idVendor		= 0X1C9E,
		LONGCHEER_INTERFACE_VDF,
	},
	{
		.match_flags	=   USB_DEVICE_ID_MATCH_INT_INFO
			  | USB_DEVICE_ID_MATCH_VENDOR,
		.idVendor		= 0X05c6,
		LONGCHEER_INTERFACE_VDF,
	},
	{
		.match_flags	=   USB_DEVICE_ID_MATCH_INT_INFO
			  | USB_DEVICE_ID_MATCH_VENDOR,
		.idVendor		= 0X1C9E,
		LONGCHEER_INTERFACE_RMNET_VDF,
	},
	{ },		// END
};
MODULE_DEVICE_TABLE(usb, lc_products);

static int lc_cdc_reset_resume(struct usb_interface *intf);
static struct usb_driver lc_ether_driver = {
	.name =		"lc_ether",
	.id_table =	lc_products,
	.probe =	lc_cdc_probe,
	.disconnect =	lc_disconnect,
	.suspend =	lc_suspend,
	.resume =	lc_resume,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
	.reset_resume = lc_cdc_reset_resume,
#endif
};
#ifdef NDIS_INIT_STATUS

static void lc_cdc_status(struct lc_cdc_net *dev, struct urb *urb)
{
	struct usb_cdc_notification	*event;

	if (urb->actual_length < sizeof *event)
		return;

	/* SPEED_CHANGE can get split into two 8-byte packets */
	if (test_and_clear_bit(EVENT_STS_SPLIT, &dev->flags)) {
		devdbg(dev, "The speed is changed by status event");
		dumpspeed(dev, (__le32 *) urb->transfer_buffer);
		return;
	}

	event = urb->transfer_buffer;
	switch (event->bNotificationType) {
	case USB_CDC_NOTIFY_NETWORK_CONNECTION:
		if (netif_msg_timer(dev))
			devdbg(dev, "CDC: carrier %s",
					event->wValue ? "on" : "off");
		if (event->wValue)
			netif_carrier_on(dev->net);
		else
			netif_carrier_off(dev->net);

		break;
	case USB_CDC_NOTIFY_SPEED_CHANGE:	/* tx/rx rates */
		if (netif_msg_timer(dev))
			devdbg(dev, "CDC: speed change (len %d)",
					urb->actual_length);
		if (urb->actual_length != (sizeof *event + 8))
			set_bit(EVENT_STS_SPLIT, &dev->flags);
		else
			dumpspeed(dev, (__le32 *) &event[1]);
		break;

	case USB_CDC_NOTIFY_RESPONSE_AVAILABLE:
	{
		break;
	}
		
	default:
		devdbg(dev, "%s: CDC: unexpected notification %02x!", __FUNCTION__,
				 event->bNotificationType);
		break;
	}
}
#endif

static int __init lc_cdc_init(void)
{
	BUG_ON((sizeof(((struct lc_cdc_net *)0)->data)
			< sizeof(struct lc_dev_state)));

 	return usb_register(&lc_ether_driver);
}
fs_initcall(lc_cdc_init);

static int  lc_send_qmi_request(struct usb_interface *intf, UInt8 reqesy_type,
				UInt8 *snd_req, int snd_len, 
				UInt8 *read_resp, int resp_len);

static int ndis_get_client_ID(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[128] = {0};
	int ret = 0;
	int len = 0;
	
	len = QCTL_GetClientID(command_buf,sizeof(command_buf),0x01);
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_GET_CLIENT_ID,command_buf, len, resp_buf, 64);
	}
	
	if (QMI_GET_CLIENT_ID != ret){
		printk(KERN_ERR "%s: Get client ID failed\n", __FUNCTION__);
		return -1;
	}
	return 0;
}
static int ndis_get_version_info(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[128] = {0};
	int ret = 0;
	int len = 0;
	
	memset(resp_buf, 0, sizeof(resp_buf));
	len = QCTL_GetVersionReq(command_buf,sizeof(command_buf));
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_GET_VERSION,command_buf, len, resp_buf, sizeof(resp_buf));
	}
	
	if (0 > ret){
		printk(KERN_ERR "%s: Get version failed\n", __FUNCTION__);
		return -1;
	}
	return 0;
}
static int ndis_set_instance_ID(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[128] = {0};
	int ret = 0;
	int len = 0;
	
	memset(resp_buf, 0, sizeof(resp_buf));
	len = QCTL_SetInstanceId(command_buf,sizeof(command_buf));
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_SET_INSTANCE,command_buf, len, resp_buf, sizeof(resp_buf));
	}
	
	if (0 > ret){
		printk(KERN_ERR "%s: Set Instance ID failed\n", __FUNCTION__);
		return -1;
	}
	return 0;
}
static int ndis_set_data_format(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[128] = {0};
	int ret = 0;
	int len = 0;
		
	//memset(resp_buf, 0, sizeof(resp_buf));
	len = QCTL_SetDataFormatReq(command_buf,sizeof(command_buf));
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_SET_DATA_FORMAT,command_buf, len, resp_buf, sizeof(resp_buf));
	}
	
	if (0 > ret){
		printk(KERN_ERR "%s: set data format failed\n", __func__);
		return -1;
	}
	return 0;
}

static int ndis_release_client_ID(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[128] = {0};
	int ret = 0;
	int len = 0;
		
	//memset(resp_buf, 0, sizeof(resp_buf));
	len = QCTL_ReleaseClientID(command_buf,sizeof(command_buf),&ndis_status);
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_RELEASE_CLIENT_ID,command_buf, len, resp_buf, sizeof(resp_buf));
	}
	
	if (0 > ret){
		printk(KERN_ERR "%s: release client ID failed\n", __FUNCTION__);
		return -1;
	}
	return 0;
}

static int ndis_connect(struct lc_cdc_net *dev,char *accessString, 
					char *userName, 
					char *passWord,
					UInt8 compression,
					UInt32 ConnectIpMode)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[256] = {0};
	int ret = 0;
	int len = 0;
	ndis_status.fConnectionError = -1;

	//ConnectIpMode = 2;		//for debug

	memset(command_buf, 0, sizeof(command_buf));
	len = QC_WDS_ModifyProfile(command_buf,sizeof(command_buf),&ndis_status,
		                     accessString,userName,passWord,compression,
		                     ConnectIpMode);
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_MODIFY_PRO,command_buf, len, resp_buf, sizeof(resp_buf));
	}	

	if( ConnectIpMode == 2)
	{	
		memset(command_buf, 0, 128);
		memset(resp_buf, 0, 256);
		devdbg(dev,"%s:connect ioctl. connect ipv4",__func__);
		len = QC_WDS_Connect(command_buf,sizeof(command_buf),ndis_status.fClientID,
		                     accessString,userName,passWord,compression,IPv4);			//ipv 4
		if(len>0)
		{
			ret = lc_send_qmi_request(dev->intf,QMI_CONNECT,command_buf, len, resp_buf, sizeof(resp_buf));
		}

		memset(command_buf, 0, 128);
		memset(resp_buf, 0, 256);
		devdbg(dev,"%s:connect ioctl. connect ipv6",__func__);
		len = QC_WDS_Connect(command_buf,sizeof(command_buf),ndis_status.fClientID,
		                     accessString,userName,passWord,compression,IPv6);			//ipv 6
		if(len>0)
		{
			ret = lc_send_qmi_request(dev->intf,QMI_CONNECT,command_buf, len, resp_buf, sizeof(resp_buf));
		}
		
	}
	else
	{			
		//memset(command_buf, 0, 128);
		
		devdbg(dev,"%s:connect ioctl. connect ipv4 or ipv6",__func__);
		len = QC_WDS_Connect(command_buf,sizeof(command_buf),ndis_status.fClientID,
		                     accessString,userName,passWord,compression,ConnectIpMode);
		if(len>0)
		{
			ret = lc_send_qmi_request(dev->intf,QMI_CONNECT,command_buf, len, resp_buf, sizeof(resp_buf));
		}		
	}


	if (ret < 0)
	{		
		return -2;
	}
	ret = ndis_get_ip_addr(dev);	
	return ret;
}

static int ndis_get_ip_addr(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[256] = {0};
	int ret = 0;
	int len = 0;
	
	ndis_status.fIPAddress = 0;
	ndis_status.fConnectionError = 0;
	//memset(resp_buf, 0, sizeof(resp_buf));
	len = QC_WDS_GetIPAddress(command_buf,sizeof(command_buf),ndis_status.fClientID);
	if(len>0)
	{
		ret = lc_send_qmi_request(dev->intf,QMI_GET_IP_ADDR,command_buf, len, resp_buf, sizeof(resp_buf));
	}
	
	if (ret < 0)
	{	
		printk("failed to get ip address.\n");	
		return -1;
	}
		
	/////////////////////////enable network....
	printk(KERN_ERR "%s: carrier on\n", __func__);
	netif_carrier_on(dev->net);
	
	return 0;
}
static int ndis_disconnect(struct lc_cdc_net *dev)
{
	UInt8 command_buf[128] = {0};
	UInt8 resp_buf[128] = {0};
	int ret = 0;
	int len = 0;

	if(!ndis_status.fPacketHandle)
	{
		memset(command_buf, 0, sizeof(command_buf));
		memset(resp_buf, 0, sizeof(resp_buf));
		len = QC_WDS_Disconnect(command_buf,sizeof(command_buf),&ndis_status,0);
		if(len>0)
		{
			ret = lc_send_qmi_request(dev->intf,QMI_DISCONNECT,command_buf, len, resp_buf, sizeof(resp_buf));
		}
	}
	if (!ndis_status.fPacketHandle1)
	{
		memset(command_buf, 0, sizeof(command_buf));
		memset(resp_buf, 0, sizeof(resp_buf));
		len = QC_WDS_Disconnect(command_buf,sizeof(command_buf),&ndis_status,1);
		if(len>0)
		{
			ret = lc_send_qmi_request(dev->intf,QMI_DISCONNECT,command_buf, len, resp_buf, sizeof(resp_buf));
		}	
	}
	
	/*if (0 > ret){
		printk(KERN_ERR "%s: set data format failed\n", __FUNCTION__);
		return -1;
	}*/	
	return 0;
}
//int lc_check_conn_status(struct usb_interface *intf)
static void lc_cdc_check_status_work()
{
	//struct lc_cdc_net *net = usb_get_intfdata(intf);
	//usb_device *udev = interface_to_usbdev(intf);
	struct lc_cdc_net *dev = g_cdc_dev;

	int ret = 0;
//	int repeat = 0;
	

	//	status_req[5] = resp_buf[23];
	
	//////////////////////////get version
	ret = ndis_get_version_info(dev);
	if(-1 == ret)
	{
		goto failed;
	}
	msleep(80);
	//////////////////////////set instance ID
	ret = ndis_set_instance_ID(dev);
	if(-1 == ret)
	{
		goto failed;
	}
	msleep(80);
	//////////////////////////set data format
	ret = ndis_set_data_format(dev);
	if(-1 == ret)
	{
		goto failed;
	}
	msleep(80);
	//////////////////////////get client id
	ret = ndis_get_client_ID(dev);
	if(-1 == ret)
	{
		goto failed;
	}
//	printk(KERN_ERR"%s: carrier on\n", __FUNCTION__);
//	netif_carrier_on(dev->net);
failed:
	
	dev->qmi_sync = 1;	
//	cancel_delayed_work(&dev->status_work);
	return;	
}

static int lc_get_qmi_status(struct usb_interface *intf)
{
	UInt8 read_resp[128];
	int resp_len = sizeof(read_resp);
	struct usb_device *udev = interface_to_usbdev(intf);
	int ret = -1;
	int index = 0;
	
	while(index++ < 3)
	{
		ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
			0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
			read_resp, resp_len, 1000);
		devdbg(g_cdc_dev,"get qmi response:");
		
		if (ret <= 0)
		{			
			msleep(10);	
		} else 
		{
			printk_hex(read_resp,resp_len);
			ret = processQMIResponse((void*)read_resp,&ndis_status);				
			if (QMI_CONNECT == ret)
			{				
				if( 0 == ndis_status.fConnectionError)
				{
					devdbg(g_cdc_dev,"connect to internet success.");
					ndis_status.fConnectionStatus = LC_GETTING_IP;	
					ndis_get_ip_addr(g_cdc_dev);										
				}else
				{
					devdbg(g_cdc_dev,"connecting to internet failed.");
					ndis_status.fConnectionStatus = LC_DISCONNECTED;	
				}					
			}
			if (QMI_DISCONNECT == ret)
			{
				devdbg(g_cdc_dev,"disconnected from internet success.");
				ndis_status.fConnectionStatus = LC_DISCONNECTED;	
				return ret;
			}
			if(QMI_GET_IP_ADDR == ret)
			{
				if( 0 == ndis_status.fConnectionError && 0 != ndis_status.fIPAddress)
				{
					devdbg(g_cdc_dev,"connect to internet success.");	
					ndis_status.fConnectionStatus = LC_IP_GETTED;										
				}else
				{
					devdbg(g_cdc_dev,"connecting to internet failed.");
					ndis_status.fConnectionStatus = LC_DISCONNECTED;	
				}		
			}
			ret = 0;
		}
	} 
	if(index==3)
		devdbg(g_cdc_dev,"%s: nothing qmi response ...\n", __func__);
	
	return ret;	
}

static int lc_send_qmi_request(struct usb_interface *intf, UInt8 reqesy_type,
				UInt8 *snd_req, int snd_len, 
				UInt8 *read_resp, int resp_len)
{
	int ret;
	int index = 0;
//	int len = 0;
	unsigned long		flags;
	struct usb_device *udev = interface_to_usbdev(intf);
//	struct lc_cdc_net *net = usb_get_intfdata(intf);
//	spin_lock_irqsave (&qmi_request_lock, flags);
	ret = usb_control_msg(udev, usb_sndctrlpipe(udev, 0), 0x00, 
					0x21, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					snd_req, snd_len, 5000);
	devdbg(g_cdc_dev,"send qmi request: %d ",ret);
	printk_hex(snd_req,snd_len);
	if (ret < 0){
		printk(KERN_ERR"%s: send the qmi request failed\n", __FUNCTION__);
//		spin_unlock_irqrestore (&qmi_request_lock, flags);
		return ret;
	}
	msleep(80);
	switch(reqesy_type)
	{
		case QMI_GET_CLIENT_ID:
			while(index++ < 10)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get qmi response:");
				
				if (ret <= 0)
				{
				//	printk(KERN_ERR"%s: Get response failed.\n", __func__);
					msleep(10);	
				} else 
				{
					printk_hex(read_resp,resp_len);
					ret = processQMIResponse((void*)read_resp,&ndis_status);				
					if (QMI_GET_CLIENT_ID == ret)
					{
						devdbg(g_cdc_dev,"get qmi ClientID response success.clientid=%u:\n",ndis_status.fClientID);
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						return ret;
					}
				}
			} 
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		case QMI_RELEASE_CLIENT_ID:
			while(index++ < 10)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get qmi response:");
				
				if (ret <= 0)
				{
			//		printk(KERN_ERR"%s: Get response failed.", __func__);
					msleep(10);	
				} else 
				{
				//	printk_hex(read_resp,resp_len);
					ret = processQMIResponse((void*)read_resp,&ndis_status);					
					if (QMI_RELEASE_CLIENT_ID == ret)
					{
						devdbg(g_cdc_dev,"Release qmi ClientID success.clientid=%u:",ndis_status.fClientID);
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						return ret;
					}
				}				
			}
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		case QMI_CONNECT:
			ndis_status.fConnectionStatus = LC_CONNECTING;
			while(index++ < 50)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get QMI_CONNECT response,index=%d:",index);
				
				if (ret <= 0)
				{
				//	printk(KERN_ERR"%s: Get QMI_CONNECT response failed\n", __func__);
					msleep(100);	
				} else 
				{
					printk_hex(read_resp,resp_len);
					ret = processQMIResponse((void*)read_resp,&ndis_status);					
					if (QMI_CONNECT == ret)
					{
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						if( 0 == ndis_status.fConnectionError)
						{
							devdbg(g_cdc_dev,"connect to internet success.");	
							ndis_status.fConnectionStatus = LC_GETTING_IP;						
							return 0;						
						}else
						{
							devdbg(g_cdc_dev,"connect to internet failed.");	
							ndis_status.fConnectionStatus = LC_DISCONNECTED;						
							return -1;
						}						
					}
				}				
			}
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		case QMI_DISCONNECT:
			while(index++ < 10)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				printk("get QMI_DISCONNECT response,index=%d:\n",index);
				
				if (ret <= 0)
				{
					printk(KERN_ERR"%s: Get QMI_DISCONNECT response failed\n", __func__);
					msleep(10);	
				} else 
				{
					printk_hex(read_resp,resp_len);
					ret = processQMIResponse((void*)read_resp,&ndis_status);	
					spin_unlock_irqrestore (&qmi_request_lock, flags);				
					if (QMI_DISCONNECT == ret)
					{
						printk("call QMI_DISCONNECT success.\n");
						return ret;
					}
				} 
			}
			ndis_status.fConnectionStatus = LC_DISCONNECTING;
			ret = -2;
			break;
		case QMI_GET_IP_ADDR:
		#if 0
			while(index++ < 20)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 3000);
			//	devdbg(g_cdc_dev,"get QMI_GET_IP_ADDR response,index=%d:",index);
				
				if (ret <= 0)
				{
				//	printk(KERN_ERR"%s: Get QMI_GET_IP_ADDR response failed\n", __func__);
					msleep(100);	
				}else 
				{
				//	printk_hex(read_resp,resp_len);
					ret = processQMIResponse((void*)read_resp,&ndis_status);					
					if (QMI_GET_IP_ADDR == ret )
					{
		//				spin_unlock_irqrestore (&qmi_request_lock, flags);
						if(0 == ndis_status.fConnectionError)
						{
						//	devdbg(g_cdc_dev,"get ipaddress success.ipaddress:%u",ndis_status.fIPAddress);
							//ndis_status.fConnectionStatus = NDIS_CONNECTED;
							return 0;
						}else
						{
						//	devdbg(g_cdc_dev,"get ipaddress failed.error num:%u",ndis_status.fConnectionError);
							//ndis_status.fConnectionStatus = NDIS_DISCONNECTED;
							return -1;
						}						
					}
				}
			}
		//	devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
		#endif
			break;
		case QMI_SET_DATA_FORMAT:
			while(index++ < 10)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get qmi response:");
				printk_hex(read_resp,resp_len);
				if (ret <= 0)
				{				
					msleep(10);	
				} else 
				{
					ret = processQMIResponse((void*)read_resp,&ndis_status);
					if (QMI_SET_DATA_FORMAT == ret)
					{
						devdbg(g_cdc_dev,"get Set data format success.");
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						return ret;					
					}
				}
			}
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		case QMI_MODIFY_PRO:
			while(index++ < 10)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get qmi response:");
				//printk_hex(read_resp,resp_len);

				if (ret < 0)
				{
					printk(KERN_ERR"%s: Set Instance failed. %d", __func__,ret);
					msleep(10);	
				} else 
				{
					ret = processQMIResponse((void*)read_resp,&ndis_status);
					if (QMI_MODIFY_PRO == ret)
					{
						devdbg(g_cdc_dev,"get QMI_MODIFY_PRO  success.");
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						return ret;
					}
				}
			}
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		
		case QMI_SET_INSTANCE:
			while(index++ < 10)
			{
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get qmi response:");
				printk_hex(read_resp,resp_len);
				if (ret <= 0)
				{
					//printk(KERN_ERR"%s: Set Instance failed.", __func__);
					msleep(10);	
				} else 
				{
					ret = processQMIResponse((void*)read_resp,&ndis_status);
					if (QMI_SET_INSTANCE == ret)
					{
						devdbg(g_cdc_dev,"get Set Instance response  success.");
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						return ret;
					}
				}
			}
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		case QMI_GET_VERSION:
		    while(index++ < 10)
			{
//				char version[64]={0};
				ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0), 0x01, 
					0xA1, 0x00, intf->cur_altsetting->desc.bInterfaceNumber, 
					read_resp, resp_len, 1000);
				devdbg(g_cdc_dev,"get qmi response:");
				printk_hex(read_resp,resp_len);
				if (ret <= 0)
				{
					//printk(KERN_ERR"%s: Get version failed\n", __func__);
					msleep(10);	
				} else 
				{
					ret = processQMIResponse((void*)read_resp,&ndis_status);
					if (QMI_GET_VERSION == ret)
					{
						devdbg(g_cdc_dev,"get qmi version success.version:%s.",ndis_status.fVersion);
//						spin_unlock_irqrestore (&qmi_request_lock, flags);
						return ret;					
					}
				}
			}
			devdbg(g_cdc_dev,"%s: Get response timeout.", __func__);
			ret = -2;
			break;
		default:
			devdbg(g_cdc_dev,"unknown qmi request type:");
			break;
	}
	
//	spin_unlock_irqrestore (&qmi_request_lock, flags);
	return ret;
}

static void __exit lc_cdc_exit(void)
{
 	usb_deregister(&lc_ether_driver);
}
module_exit(lc_cdc_exit);


MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");
