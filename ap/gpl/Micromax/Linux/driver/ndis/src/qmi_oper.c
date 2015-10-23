/*
 * This file is developed based on USBModemECMControl.cpp 
 * which is used by mac NDIS dail 2012.01.05 
 * this file only process the qmi data package,so hope for porting to other system easily.
 * Copyright (C) 2012 by qiaoyongkang (longcheer Ltd.)
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
#include <linux/ctype.h>
#include <linux/kernel.h>
#include "qmi_oper.h"
#include "qmi_header.h"
#define bzero(b,len) memset((b),0,(len))

static UInt8    fQMITransactionId = 0;
//#define HEX_DEBUG
#ifdef DEBUG
#define lc_kdbg(fmt, arg...) 	do{ \
	printk(KERN_ERR "######: " fmt , ## arg); \
	}while(0)
#else
#define lc_kdbg(fmt, arg...) 	do{ \
	}while(0)
#endif
/********************************************************************
 *
 *         Name:  printk_hex
 *  Description:  print data buffer with hex which is lined by 16 bytes
 *        Input:  buf:the data buffer to be print of
 *                buf_size:the data buffer length.
 *       Return:  null
 *        Notes:  
 ********************************************************************/
void printk_hex(UInt8 *buf,int buf_size)
{
#ifdef HEX_DEBUG
    int i,j;
    int total_size = 0;
    int line_cout =  buf_size>>4;
    if((buf_size&0x0000000f)!=0)
        line_cout++;

    for(i=0;i<line_cout;i++)
    {
        printk("\n%d:",total_size);
        for(j=0;j<16;j++)
        {
            total_size++;
            printk("%02X ",buf[(i<<4)+j]);
            if(((i<<4)+1+j)==buf_size)
            {
                printk("\n");
                return;
            }
        }
    }
#endif    
}
/********************************************************************
 *
 *         Name:  QCTL_GetClientID
 *  Description:  get a qmi client ID
 *        Input:  qmiType:the type of qmi,there are QMUX_TYPE_WDS
 *                length:the pMsgBuff max length.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int  QCTL_GetClientID(UInt8* pMsgBuff,UInt32 length,UInt8 qmiType)
{
    UInt16 msgLength;
//	void *pMsgBuff = NULL;
	QCQMI* qmi = NULL;
	QMICTL_GET_CLIENT_ID_REQ_MSG* pReq = NULL;
	
	lc_kdbg("QCTL_GetClientID : entry\n");

	msgLength = sizeof(QCQMI_HDR) + sizeof(QMICTL_GET_CLIENT_ID_REQ_MSG);
	lc_kdbg("%s:msgLength=%d,%d,%d\n",__func__, msgLength,sizeof(QCQMI_HDR),sizeof(QMICTL_GET_CLIENT_ID_REQ_MSG) );
	
	if(msgLength>length||pMsgBuff==NULL)
	{
		lc_kdbg("QCTL_GetClientID : length=%d < msgLength=%d OR pMsgBuff==NULL\n",msgLength,length);
		return -1;
	}
	
	bzero(pMsgBuff, length);
	
	qmi = (QCQMI*)pMsgBuff;
	pReq = (QMICTL_GET_CLIENT_ID_REQ_MSG*)&(qmi->SDU);
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	qmi->CtlFlags  = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_CTL;
	qmi->ClientId = 0x00;
	
	pReq->CtlFlags = QMICTL_CTL_FLAG_REQUEST;
	++fQMITransactionId;
	pReq->TransactionId = fQMITransactionId;
	pReq->QMICTLType    = QMICTL_GET_CLIENT_ID_REQ;
	pReq->Length        = sizeof(QMICTL_GET_CLIENT_ID_REQ_MSG) - sizeof(QCQMICTL_MSG_HDR);
	pReq->TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
	pReq->TLVLength     = 1;
	pReq->QMIType       = qmiType;
	
	//lc_kdbg("QCTL_GetClientID : leave");
	lc_kdbg("%s : leave\n",__func__);
	return msgLength;
}
/********************************************************************
 *
 *         Name:  QCTL_ReleaseClientID
 *  Description:  relase a qmi client ID
 *        Input:  length:the pMsgBuff max length.
 *                pndis_status : the input global parameters.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QCTL_ReleaseClientID(UInt8* pMsgBuff,UInt32 length,NDIS_QMI_STATUS* pndis_status)
{
	UInt16 msgLength;
//	void *pMsgBuff = NULL;
	QCQMI* qmi = NULL;
	QMICTL_RELEASE_CLIENT_ID_REQ_MSG* pReq = NULL;
	
	lc_kdbg("QCTL_ReleaseClientID : entry\n");
	msgLength = sizeof(QCQMI_HDR) + sizeof(QMICTL_RELEASE_CLIENT_ID_REQ_MSG);	
	
	if(msgLength>length||pMsgBuff==NULL)
	{
		lc_kdbg("QCTL_ReleaseClientID : length=%d < msgLength=%d OR pMsgBuff==NULL\n",msgLength,length);
		return -1;
	}
	bzero(pMsgBuff, length);	
	
	qmi = (QCQMI*)pMsgBuff;
	pReq = (QMICTL_RELEASE_CLIENT_ID_REQ_MSG*)&(qmi->SDU);
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	qmi->CtlFlags  = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_CTL;
	qmi->ClientId = 0x00;
	
	pReq->CtlFlags = QMICTL_CTL_FLAG_REQUEST;
	++fQMITransactionId;
	pReq->TransactionId = fQMITransactionId;
	pReq->QMICTLType    = QMICTL_RELEASE_CLIENT_ID_REQ;
	pReq->Length        = sizeof(QMICTL_RELEASE_CLIENT_ID_REQ_MSG) - sizeof(QCQMICTL_MSG_HDR);
	pReq->TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
	pReq->TLVLength     = 2;
	pReq->QMIType       = pndis_status->fQMIType;
	pReq->ClientId      = pndis_status->fClientID;
	
	//lc_kdbg("QCTL_ReleaseClientID : leave");
	lc_kdbg("%s : leave\n",__func__);
    return msgLength;
}
/********************************************************************
 *
 *         Name:  QCTL_GetVersionReq
 *  Description:  GET a qmi version info
 *        Input:  length:the pMsgBuff max length.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QCTL_GetVersionReq(UInt8* pMsgBuff,UInt32 length)
{
	UInt16 msgLength;
//	void *pMsgBuff = NULL;
	QCQMI* qmi = NULL;
	QMICTL_GET_VERSION_REQ_MSG* pReq = NULL;
	
	lc_kdbg("QCTL_GetVersionReq : entry\n");
	
	msgLength = sizeof(QCQMI_HDR) + sizeof(QMICTL_GET_VERSION_REQ_MSG);
	
	if (msgLength>length||pMsgBuff==NULL)
	{
	    lc_kdbg("QCTL_GetVersionReq : length=%d < msgLength=%d OR pMsgBuff==NULL\n",msgLength,length);
		return -1;
	}
	
	bzero(pMsgBuff, length);	
	
	qmi = (QCQMI*)pMsgBuff;
	pReq = (QMICTL_GET_VERSION_REQ_MSG*)&(qmi->SDU);
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	qmi->CtlFlags  = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_CTL;
	qmi->ClientId = 0x00;
	
	pReq->CtlFlags = QMICTL_CTL_FLAG_REQUEST;
	++fQMITransactionId;
	pReq->TransactionId = fQMITransactionId;
	pReq->QMICTLType    = QMICTL_GET_VERSION_REQ;
	pReq->Length        = sizeof(QMICTL_GET_VERSION_REQ_MSG) - sizeof(QCQMICTL_MSG_HDR);
	pReq->TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
	pReq->TLVLength     = 0x0001;
	pReq->QMIType       = QMUX_TYPE_ALL;
	
	//lc_kdbg("QCTL_GetVersionReq : leave");
	lc_kdbg("%s : leave\n",__func__);
    return msgLength;  
}
/********************************************************************
 *
 *         Name:  QCTL_SetDataFormatReq
 *  Description:  set a qmi data format 
 *        Input:  length:the pMsgBuff max length.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QCTL_SetDataFormatReq(UInt8* pMsgBuff,UInt32 length)
{
	UInt16 msgLength;
//	void *pMsgBuff = NULL;
	QCQMI* qmi = NULL;
	QMICTL_SET_DATA_FORMAT_REQ_MSG* pReq = NULL;
	
	lc_kdbg("QCTL_SetDataFormatReq : entry\n");
	
	msgLength = sizeof(QCQMI_HDR) + sizeof(QMICTL_SET_DATA_FORMAT_REQ_MSG);
	
//	pMsgBuff = IOMalloc(msgLength);
	
	if (msgLength>length||pMsgBuff==NULL)
	{
	    lc_kdbg("QCTL_SetDataFormatReq : length=%d < msgLength=%d OR pMsgBuff==NULL\n",msgLength,length);
		return -1;
	}
	
	bzero(pMsgBuff, length);	
	
	qmi = (QCQMI*)pMsgBuff;
	pReq = (QMICTL_SET_DATA_FORMAT_REQ_MSG*)&(qmi->SDU);
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	qmi->CtlFlags  = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_CTL;
	qmi->ClientId = 0x00;
	
	pReq->CtlFlags = QMICTL_CTL_FLAG_REQUEST;
	++fQMITransactionId;
	pReq->TransactionId = fQMITransactionId;
	pReq->QMICTLType    = QMICTL_SET_DATA_FORMAT_REQ;
	pReq->Length        = sizeof(QMICTL_SET_DATA_FORMAT_REQ_MSG) - sizeof(QCQMICTL_MSG_HDR);
	pReq->TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
	pReq->TLVLength     = 0x0001;
	pReq->DataFormat    = 0; // disable qos
	pReq->Proto.TLVType = SET_DATA_FORMAT_TLV_TYPE_LINK_PROTO;
	pReq->Proto.TLVLength = 0x0002;
	pReq->Proto.LinkProt  = (SET_DATA_FORMAT_LINK_PROTO_ETH | SET_DATA_FORMAT_LINK_PROTO_IP);
	
	//lc_kdbg("QCTL_SetDataFormatReq : leave");
	lc_kdbg("%s : leave\n",__func__);
    return msgLength;   
}
/********************************************************************
 *
 *         Name:  QCTL_SetInstanceId
 *  Description:  set a qmi Instance ID 
 *        Input:  length:the pMsgBuff max length.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QCTL_SetInstanceId(UInt8* pMsgBuff,UInt32 length)
{
	UInt16 msgLength;
//	void *pMsgBuff = NULL;
	QCQMI* qmi = NULL;
	QMICTL_SET_INSTANCE_ID_REQ_MSG* pSet = NULL;
	
	//lc_kdbg("QCTL_SetInstanceId : entry");
	lc_kdbg("%s : entry\n",__func__);
	
	msgLength = sizeof(QCQMI_HDR) + sizeof(QMICTL_SET_INSTANCE_ID_REQ_MSG);
	
//	pMsgBuff = IOMalloc(msgLength);	
	if (msgLength>length||pMsgBuff==NULL)
	{
	    lc_kdbg("%s : length=%d < msgLength=%d OR pMsgBuff==NULL\n",__func__,msgLength,length);
		return -1;
	}
	
	bzero(pMsgBuff, length);	
	
	qmi = (QCQMI*)pMsgBuff;
	pSet = (QMICTL_SET_INSTANCE_ID_REQ_MSG*)&(qmi->SDU);
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	qmi->CtlFlags  = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_CTL;
	qmi->ClientId = 0x00;
	
	pSet->CtlFlags = QMICTL_CTL_FLAG_REQUEST;
	++fQMITransactionId;
	pSet->TransactionId = fQMITransactionId;
	pSet->QMICTLType    = QMICTL_SET_INSTANCE_ID_REQ;
	pSet->Length        = sizeof(QMICTL_SET_INSTANCE_ID_REQ_MSG) - sizeof(QCQMICTL_MSG_HDR);
	pSet->TLVType       = QCTLV_TYPE_REQUIRED_PARAMETER;
	pSet->TLVLength     = 1;
	pSet->Value         = 1; // will fail?
	
	lc_kdbg("%s : leave\n",__func__);
	
    return msgLength;   	
}
/********************************************************************
 *
 *         Name:  QC_WDS_GetIPAddress
 *  Description:  get  qmi ip address 
 *        Input:  length:the pMsgBuff max length.
 *                fClientID:the current clientID 
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QC_WDS_GetIPAddress(UInt8* pMsgBuff,UInt32 length,UInt8 fClientID)
{
	UInt16 msgLength;
	QCQMI* qmi = NULL;
	QCQMUX* pQCQMUX = NULL;
	QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG* pGetMsg = NULL;
	
	lc_kdbg("%s : entry\n",__func__);
	msgLength = sizeof(QCQMI_HDR) + sizeof(QCQMUX_HDR) + sizeof(QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG);
	if (msgLength>length||pMsgBuff==NULL)
	{
	    lc_kdbg("%s : length=%d < msgLength=%d OR pMsgBuff==NULL\n",__func__,msgLength,length);
		return -1;
	}
	qmi = (QCQMI*)pMsgBuff;	
	bzero(pMsgBuff, length);	
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->CtlFlags = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_WDS;
	qmi->ClientId = fClientID;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	
	pQCQMUX = (QCQMUX*)&(qmi->SDU);
	
	pQCQMUX->CtlFlags = QMUX_CTL_FLAG_SINGLE_MSG | QMUX_CTL_FLAG_TYPE_CMD;
	++fQMITransactionId;
	pQCQMUX->TransactionId = fQMITransactionId;
	
	pGetMsg = (QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG*)&(pQCQMUX->Message);
	pGetMsg->Type = QMIWDS_GET_RUNTIME_SETTINGS_REQ;
	pGetMsg->Length  = sizeof(QMIWDS_GET_RUNTIME_SETTINGS_REQ_MSG) - sizeof(QCQMUX_MSG_HDR);
	pGetMsg->TLVType =  0x10;
	pGetMsg->TLVLength = 0x04;
	pGetMsg->Mask = QMIWDS_GET_RUNTIME_SETTINGS_MASK_IPV4_ADDR;
	
	lc_kdbg("%s : leave\n",__func__);
	
    return msgLength;	
}

/********************************************************************
 *
 *         Name:  QC_WDS_ModifyProfile
 *  Description:  use apn dail to internet through qmi 
 *        Input:  length:the pMsgBuff max length;
 *                fClientID:the current clientID 
 *                accessString:apn;
 *                userName: user name;
 *                passWord: password;
 *                compression: auth type.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QC_WDS_ModifyProfile(UInt8* pMsgBuff,
					UInt32 length,
					NDIS_QMI_STATUS*  pndis_status,//fClientID
					char *accessString, 
					char *userName, 
					char *passWord,
					UInt8 compression,
					UInt32 ConnectIpMode)
{
	UInt16 msgLength;
	QCQMI* qmi = NULL;
	UInt8  *pMsg = NULL;
	QCQMUX* pQCQMUX = NULL;
	QCQMUX_MSG_HDR* qmuxMsg = NULL;

//	if(ConnectIpMode == 0)
//		return -1;
	lc_kdbg("%s : entry\n",__func__);
	qmi = (QCQMI*)pMsgBuff;
	
	bzero(pMsgBuff, length);	
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->CtlFlags = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_WDS;
	qmi->ClientId = pndis_status->fClientID;
	
	msgLength = sizeof(QCQMI_HDR);
	pMsg = (UInt8 *)pMsgBuff;
	
	pMsg += msgLength;
	
	pQCQMUX = (QCQMUX*)&(qmi->SDU);
	
	//lc_kdbg_hex(pMsgBuff,32);
	
	pQCQMUX->CtlFlags = QMUX_CTL_FLAG_SINGLE_MSG | QMUX_CTL_FLAG_TYPE_CMD;
	++fQMITransactionId;
	pQCQMUX->TransactionId = cpu_to_le16(fQMITransactionId);
	//pndis_status->fConnectionTransId = cpu_to_le16(pQCQMUX->TransactionId);
	//lc_kdbg_hex(pMsgBuff,32);
	//pQCQMUX->CtlFlags = QMUX_CTL_FLAG_TYPE_RSP | QMUX_CTL_FLAG_TYPE_CMD;
	//lc_kdbg_hex(pMsgBuff,32);
	
	
	qmuxMsg = (QCQMUX_MSG_HDR*)&(pQCQMUX->Message);
	qmuxMsg->Type = cpu_to_le16(QMI_WDS_MODIFY_PROFILE_SETTINGS_REQ);
	//lc_kdbg_hex(pMsgBuff,32);
	pMsg = (UInt8 *)&(qmuxMsg->Length);
	
	pMsg += sizeof(qmuxMsg->Length);
	
	// APN
	//if (accessString && (strlen(accessString) > 0))
	{
		*pMsg = QCTLV_TYPE_APN;
		++pMsg;
		// length
		*(UInt16 *)pMsg = cpu_to_le16(strlen(accessString));
		pMsg += sizeof(UInt16);
		
		strcpy((char *)pMsg, accessString);
		
		pMsg += strlen(accessString);
	}
	
	// Auth
	switch (compression)
	{
		case WwanAuthProtocolNone:
		{
		    break;
		}
		
		case WwanAuthProtocolPap:
		{
			*pMsg = 0x1d;
			++pMsg;
			*(UInt16 *)pMsg = 1;
			pMsg += sizeof(UInt16);
			*pMsg = 1;
			++pMsg;
			break;
		}
			
		case WwanAuthProtocolChap:
		case WwanAuthProtocolMax:
		{
			*pMsg = 0x1d;
			++pMsg;
			*(UInt16 *)pMsg = 1;
			pMsg += sizeof(UInt16);
			*pMsg = 2;
			++pMsg;
			break;
		}
			
		default:
		{
		    break;
		}
	}
	
	// user name
	if (userName)
	{
	   if (strlen(userName) > 0)
	   {
		   *pMsg = 0x1b;
		   ++pMsg;
		   *(UInt16 *)pMsg = strlen(userName);
		   pMsg += sizeof(UInt16);
		   strcpy((char *)pMsg, userName);
		   pMsg += strlen(userName);
	   }
	}
	
	// password
	if (passWord)
	{
		if (strlen(passWord) > 0)
		{
			*pMsg = 0x1c;
			++pMsg;
			*(UInt16 *)pMsg = strlen(passWord);
			pMsg += sizeof(UInt16);
			strcpy((char *)pMsg, passWord);
			pMsg += strlen(passWord);
		}	
	}

	switch(ConnectIpMode)
	{
		case IPv4:			//ipv 4
			{
				*pMsg = 0x1d;
				++pMsg;
				// length
				*(UInt16 *)pMsg = 1;
				pMsg += sizeof(UInt16);
				
				*(char *)pMsg = 4;
				pMsg += sizeof(UInt8);
			}
			break;		
		case IPv6:		//ipv 6
			{
				*pMsg = 0x1d;
				++pMsg;
				// length
				*(UInt16 *)pMsg = 1;
				pMsg += sizeof(UInt16);
				
				*(char *)pMsg = 6;
				pMsg += sizeof(UInt8);
				
			}
			break;
	}
	

	// ProfileType & ProfileIndex
	{
		*pMsg = 0x01;
		++pMsg;
		// length
		*(UInt16 *)pMsg = 2;
		pMsg += sizeof(UInt16);
		
		if(ConnectIpMode == IPv6)
		{
			*(char *)pMsg = 0;
			pMsg += sizeof(UInt8);
			*(char *)pMsg = 2;
		}
		else if (ConnectIpMode == IPv4v6)
		{
			*(char *)pMsg = 0;
			pMsg += sizeof(UInt8);
			*(char *)pMsg = 2;			
		}
		
		pMsg += sizeof(UInt8);
	}
	
	// PdpType
	{
		*pMsg = 0x11;
		++pMsg;
		// length
		*(UInt16 *)pMsg = 1;
		pMsg += sizeof(UInt16);		
		
		if(ConnectIpMode == IPv6)
		{
			*(char *)pMsg = 2;
		}
		else if(ConnectIpMode == IPv4v6)
		{
			*(char *)pMsg = 3;
		}
		
		pMsg += sizeof(UInt8);
	}
	


	
	// set the length
	qmuxMsg->Length = cpu_to_le16((UInt8 *)pMsg - (UInt8 *)&(qmuxMsg->Length) - sizeof(UInt16));
	qmi->Length     = cpu_to_le16((UInt8 *)pMsg - (UInt8 *)&(qmi->Length));
	msgLength       = (UInt8 *)pMsg - (UInt8 *)qmi;
	
	lc_kdbg("%s: Connect dump data:\n",__func__);
	printk_hex(pMsgBuff,msgLength);
	
	lc_kdbg("%s : leave\n",__func__);
	
    return msgLength;
}

/********************************************************************
 *
 *         Name:  QC_WDS_Connect
 *  Description:  use apn dail to internet through qmi 
 *        Input:  length:the pMsgBuff max length;
 *                fClientID:the current clientID 
 *                accessString:apn;
 *                userName: user name;
 *                passWord: password;
 *                compression: auth type.
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/
int QC_WDS_Connect(UInt8* pMsgBuff,
					UInt32 length,
					UInt8  fClientID,
					char *accessString, 
					char *userName, 
					char *passWord,
					UInt8 compression,
					UInt32 ConnectIpMode)
{
	UInt16 msgLength;
	QCQMI* qmi = NULL;
	UInt8  *pMsg = NULL;
	QCQMUX* pQCQMUX = NULL;
	QCQMUX_MSG_HDR* qmuxMsg = NULL;

	if(ConnectIpMode == 2)
		return -1;
	lc_kdbg("%s : entry\n",__func__);
	qmi = (QCQMI*)pMsgBuff;
	
	bzero(pMsgBuff, length);	
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->CtlFlags = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_WDS;
	qmi->ClientId = fClientID;
	
	msgLength = sizeof(QCQMI_HDR);
	pMsg = (UInt8 *)pMsgBuff;
	
	pMsg += msgLength;
	
	pQCQMUX = (QCQMUX*)&(qmi->SDU);
	
	//lc_kdbg_hex(pMsgBuff,32);
	
	pQCQMUX->CtlFlags = QMUX_CTL_FLAG_SINGLE_MSG | QMUX_CTL_FLAG_TYPE_CMD;
	++fQMITransactionId;
	pQCQMUX->TransactionId = fQMITransactionId;
	//lc_kdbg_hex(pMsgBuff,32);
	//pQCQMUX->CtlFlags = QMUX_CTL_FLAG_TYPE_RSP | QMUX_CTL_FLAG_TYPE_CMD;
	//lc_kdbg_hex(pMsgBuff,32);
	
	
	qmuxMsg = (QCQMUX_MSG_HDR*)&(pQCQMUX->Message);
	qmuxMsg->Type = QMIWDS_START_NETWORK_INTERFACE_REQ;
	//lc_kdbg_hex(pMsgBuff,32);
	pMsg = (UInt8 *)&(qmuxMsg->Length);
	
	pMsg += sizeof(qmuxMsg->Length);
	
	// APN
	//if (accessString && (strlen(accessString) > 0))
	{
		*pMsg = QCTLV_TYPE_APN;
		++pMsg;
		// length
		*(UInt16 *)pMsg = strlen(accessString);
		pMsg += sizeof(UInt16);
		
		strcpy((char *)pMsg, accessString);
		
		pMsg += strlen(accessString);
	}
	
	// Auth
	switch (compression)
	{
		case WwanAuthProtocolNone:
		{
		    break;
		}
		
		case WwanAuthProtocolPap:
		{
			*pMsg = QCTLV_TYPE_AUTH_TYPE;
			++pMsg;
			*(UInt16 *)pMsg = 1;
			pMsg += sizeof(UInt16);
			*pMsg = 1;
			++pMsg;
			break;
		}
			
		case WwanAuthProtocolChap:
		{
			*pMsg = QCTLV_TYPE_AUTH_TYPE;
			++pMsg;
			*(UInt16 *)pMsg = 1;
			pMsg += sizeof(UInt16);
			*pMsg = 2;
			++pMsg;
			break;
		}
			
		default:
		{
		    break;
		}
	}
	
	// user name
	if (userName)
	{
	   if (strlen(userName) > 0)
	   {
		   *pMsg = QCTLV_TYPE_USER_NAME;
		   ++pMsg;
		   *(UInt16 *)pMsg = strlen(userName);
		   pMsg += sizeof(UInt16);
		   strcpy((char *)pMsg, userName);
		   pMsg += strlen(userName);
	   }
	}
	
	// password
	if (passWord)
	{
		if (strlen(passWord) > 0)
		{
			*pMsg = QCTLV_TYPE_PASSWORD;
			++pMsg;
			*(UInt16 *)pMsg = strlen(passWord);
			pMsg += sizeof(UInt16);
			strcpy((char *)pMsg, passWord);
			pMsg += strlen(passWord);
		}	
	}

	switch(ConnectIpMode)
	{
		case IPv4:			//ipv 4
			{
				*pMsg = 0x19;
				++pMsg;
				// length
				*(UInt16 *)pMsg = 1;
				pMsg += sizeof(UInt16);
				
				*(char *)pMsg = 4;
				pMsg += sizeof(UInt8);
			}
			break;		
		case IPv6:		//ipv 6
			{
				*pMsg = 0x19;
				++pMsg;
				// length
				*(UInt16 *)pMsg = 1;
				pMsg += sizeof(UInt16);
				
				*(char *)pMsg = 6;
				pMsg += sizeof(UInt8);

				*pMsg = 0x31;
				++pMsg;
				// length
				*(UInt16 *)pMsg = 1;
				pMsg += sizeof(UInt16);
				
				*(char *)pMsg = 2;
				pMsg += sizeof(UInt8);
				
			}
			break;
	}
	
	// set the length
	qmuxMsg->Length = (UInt8 *)pMsg - (UInt8 *)&(qmuxMsg->Length) - sizeof(UInt16);
	qmi->Length     = (UInt8 *)pMsg - (UInt8 *)&(qmi->Length);
	msgLength       = (UInt8 *)pMsg - (UInt8 *)qmi;
	
	lc_kdbg("%s: Connect dump data:\n",__func__);
	printk_hex(pMsgBuff,msgLength);
	
	lc_kdbg("%s : leave\n",__func__);
	
    return msgLength;
}
/********************************************************************
 *
 *         Name:  QC_WDS_Disconnect
 *  Description:  disconnect from  qmi internet.
 *        Input:  length:the pMsgBuff max length.
 *                pndis_status:ndis global status parameters. 
 *       Output:  pMsgBuff : the buf used to generate a package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/	
int QC_WDS_Disconnect(UInt8* pMsgBuff,UInt32 length,NDIS_QMI_STATUS* pndis_status,UInt8 flag)
{
	UInt16 msgLength;
	QCQMI* qmi = NULL;
	QCQMUX* pQCQMUX = NULL;
	QMIWDS_STOP_NETWORK_INTERFACE_REQ_MSG* pStopMsg = NULL;
	
	lc_kdbg("%s : entry\n",__func__);
	
	msgLength = sizeof(QCQMI_HDR) + sizeof(QCQMUX_HDR) + sizeof(QMIWDS_STOP_NETWORK_INTERFACE_REQ_MSG);
	if (msgLength>length||pMsgBuff==NULL)
	{
	    lc_kdbg("%s : length=%d < msgLength=%d OR pMsgBuff==NULL\n",__func__,msgLength,length);
		return -1;
	}
	
	qmi = (QCQMI*)pMsgBuff;
	
	bzero(pMsgBuff, length);	
	
	qmi->IFType   = USB_CTL_MSG_TYPE_QMI;
	qmi->CtlFlags = QMICTL_CTL_FLAG_CMD;
	qmi->QMIType  = QMUX_TYPE_WDS;
	qmi->ClientId = pndis_status->fClientID;
	qmi->Length   = msgLength - sizeof(qmi->IFType);
	
	pQCQMUX = (QCQMUX*)&(qmi->SDU);
	
	pQCQMUX->CtlFlags = QMUX_CTL_FLAG_SINGLE_MSG | QMUX_CTL_FLAG_TYPE_CMD;
	++fQMITransactionId;
	pQCQMUX->TransactionId = fQMITransactionId;
	
	pStopMsg = (QMIWDS_STOP_NETWORK_INTERFACE_REQ_MSG*)&(pQCQMUX->Message);
	pStopMsg->Type = QMIWDS_STOP_NETWORK_INTERFACE_REQ;
	pStopMsg->Length  = sizeof(QMIWDS_STOP_NETWORK_INTERFACE_REQ_MSG) - sizeof(QCQMUX_MSG_HDR);
	pStopMsg->TLVType =  QCTLV_TYPE_REQUIRED_PARAMETER;
	pStopMsg->TLVLength = 0x04;
	if(flag)
		pStopMsg->Packet_Handle = cpu_to_le32(pndis_status->fPacketHandle);	
	else
		pStopMsg->Packet_Handle = cpu_to_le32(pndis_status->fPacketHandle1);	

	lc_kdbg("%s : leave\n",__func__);
	
    return msgLength;

	lc_kdbg("%s : leave\n",__func__);
	
    return msgLength;
}
/********************************************************************
 *
 *         Name:  QCTL_Procecess_GetClient_ID_Response
 *  Description:  QCTL Procecess GetClient ID Response
 *        Input:  pQmi:the package message get from qmi server.
 *       Output:  fClientID : client ID getted from the package.
 *                fQMIType  : fQMIType getted from the package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/	
int QCTL_Procecess_GetClient_ID_Response(QCQMI* pQmi,UInt8* fClientID,UInt8* fQMIType)
{
	QMICTL_GET_CLIENT_ID_RESP_MSG* pMsg = (QMICTL_GET_CLIENT_ID_RESP_MSG*)&(pQmi->SDU);
	
	lc_kdbg("%s : entry\n",__func__);
	
	if (pMsg->QMIResult == QMI_RESULT_SUCCESS)
	{
	    *fClientID = pMsg->ClientId;
		*fQMIType  = pMsg->QMIType;
		
		lc_kdbg("%s: get client id.\n",__func__);
	}
	else 
	{
		*fClientID = 0;
		*fQMIType = 0;
		lc_kdbg("%s: failed to get client id.\n",__func__);
		return -1;
	}
	
	lc_kdbg("%s : leave\n",__func__);
	
	return 0;
}
/********************************************************************
 *
 *         Name:  QCTL_Procecess_SetDataFormat_Response
 *  Description:  QCTL Procecess SetDataFormat Response.
 *        Input:  pQmi:the package message get from qmi server.
 *       Output:  pndis_status : qmi global status parameters.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/	
int QCTL_Procecess_SetDataFormat_Response(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status)
{
    QMICTL_SET_DATA_FORMAT_RESP_MSG* pMsg = (QMICTL_SET_DATA_FORMAT_RESP_MSG*)&(pQmi->SDU);
	
	lc_kdbg("%s : entry\n",__func__);
	
	if (pMsg->QMIResult == QMI_RESULT_SUCCESS)
	{
	    if (pndis_status->fIPPacketSupport)
		{
		    UInt8 *pMsgStart = (UInt8 *) &(pMsg->TLVType);
			UInt8 *pMsgPtr = (UInt8 *)&(pMsg->QMIError);
			UInt32 remaningBytes;
			pMsgPtr += sizeof(UInt16);			
			
			remaningBytes = pMsg->Length - (pMsgPtr -pMsgStart);
			
			if (remaningBytes >= sizeof(QMICTL_SET_DATA_FORMAT_TLV_LINK_PROT))
			{
				if (pMsg->Proto.TLVType == SET_DATA_FORMAT_TLV_TYPE_LINK_PROTO)
				{
					lc_kdbg("%s: link protocol received.pMsg->Proto.LinkProt=%u\n",__func__,pMsg->Proto.LinkProt);
					pndis_status->fIPModeEnabled = (pMsg->Proto.LinkProt == SET_DATA_FORMAT_LINK_PROTO_IP);
					if (pndis_status->fIPModeEnabled)
					{
					    lc_kdbg("IP Mode is enabled!\n");
					}
				}
			}
		}
	}
	else
	{
	    lc_kdbg("%s: failed to set data format.QMIError=%u,QMIResult=%u\n",__func__, pMsg->QMIError, pMsg->QMIResult);
		return -1;
	}
	
/*	if (fSetDataFormating)
	{
	    fSetDataFormating = false;
		fCommandGate->commandWakeup(NULL, false);
		XTRACE(this, 0, 0, "QCTL_Procecess_SetDataFormat_Response: now wake up command gate");
	}*/
	lc_kdbg("%s : leave\n",__func__);
	
	return 0;	
}
/********************************************************************
 *
 *         Name:  QCTL_Procecess_SetInstanceId_Response
 *  Description:  QCTL Procecess SetInstanceId Response.
 *        Input:  pQmi:the package message get from qmi server.
 *       Output:  fSetInstanceIdFail : if get qmi id return -1,else return 1.
 *                fQMI_ID : QMI_ID
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/	
int QCTL_Procecess_SetInstanceId_Response(QCQMI* pQmi,int* fSetInstanceIdFail,UInt32* fQMI_ID)
{
	QMICTL_SET_INSTANCE_ID_RESP_MSG* pMsg = (QMICTL_SET_INSTANCE_ID_RESP_MSG*)&(pQmi->SDU);
	
	lc_kdbg("%s : entry\n",__func__);
	
	if (pMsg->QMIResult == QMI_RESULT_SUCCESS)
	{
	    *fQMI_ID = pMsg->QMI_ID;
		*fSetInstanceIdFail = -1;
		lc_kdbg("%s: get qmi id,QMI_ID=%u\n",__func__,pMsg->QMI_ID);
	}
	else 
	{
		*fSetInstanceIdFail = 1;
		*fQMI_ID = MP_INVALID_QMI_ID;
		lc_kdbg("%s: failed to get qmi id,QMIResult=%u,QMIError=%u.\n",
				__func__,pMsg->QMIResult, pMsg->QMIError);
		return -1;
	}

/*	if (fSetInstanceIding)
	{
	    fSetInstanceIding = false;
		fCommandGate->commandWakeup(NULL, false);
		XTRACE(this, 0, 0, "QCTL_Procecess_SetInstanceId_Response: now wake up command gate");
	}*/
	lc_kdbg("%s : leave\n",__func__);
	
	return 0;	
}
/********************************************************************
 *
 *         Name:  QCTL_Procecess_ReleaseClient_ID_Response
 *  Description:  QCTL Procecess ReleaseClient ID Response.
 *        Input:  pQmi:the package message get from qmi server.
 *       Output:  fClientID : client ID getted from the package.
 *                fQMIType  : fQMIType getted from the package.
 *       Return:  -1 : call faled.
 *                >0  : call success,the package length used.
 *        Notes:  
 ********************************************************************/	
int QCTL_Procecess_ReleaseClient_ID_Response(QCQMI* pQmi,UInt8* fClientID,UInt8* fQMIType)
{
	QMICTL_RELEASE_CLIENT_ID_RESP_MSG* pMsg = (QMICTL_RELEASE_CLIENT_ID_RESP_MSG*)&(pQmi->SDU);
	
	lc_kdbg("%s : entry\n",__func__);
	
	if (pMsg->QMIResult == QMI_RESULT_SUCCESS)
	{
	    lc_kdbg( "%s: release client id.ClientId=%u,QMIType=%u.\n", __func__,pMsg->ClientId, pMsg->QMIType);
		
		if (*fClientID == pMsg->ClientId && *fQMIType == pMsg->QMIType)
		{
		    *fClientID = 0;
			*fQMIType  = 0;
		}
	}
	else 
	{
		lc_kdbg( "%s: failed to release client id,QMIError=%u,QMIResult=%u.\n",__func__,pMsg->QMIError, pMsg->QMIResult);
		return -1;
	}
	
/*	if (fReleaseingID)
	{
	    fReleaseingID = false;
		fCommandGate->commandWakeup(NULL, false);
		XTRACE(this, 0, 0, "QCTL_Procecess_ReleaseClient_ID_Response: now wake up command gate");
	}*/
	lc_kdbg("%s : leave\n",__func__);
	
	return 0;
}
/********************************************************************
 *
 *         Name:  QCTL_Procecess_GetVersion_Respons
 *  Description:  QCTL Procecess GetVersion Respons.
 *        Input:  pQmi:the package message get from qmi server.
 *       Output:  pndis_status : qmi global status parameters.
 *       Return:  -1 : call faled.
 *                0  : call success
 *        Notes:  
 ********************************************************************/	
int QCTL_Procecess_GetVersion_Respons(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status)
{
	QMICTL_GET_VERSION_RESP_MSG* pMsg = (QMICTL_GET_VERSION_RESP_MSG*) &(pQmi->SDU);
	QMUX_TYPE_VERSION_STRUCT* pVersion = NULL;
	UInt32 msgLen;
	UInt8 *pMsgStart;
	int i;
	msgLen = pMsg->Length;
	pMsgStart = (UInt8 *) &(pMsg->TLVType);
	
	
	lc_kdbg("%s : entry\n",__func__);
	
	if (pMsg->QMIResult != QMI_RESULT_SUCCESS)
	{
		lc_kdbg( "%s: failed to get version.QMIError=%u,QMIResult=%u.\n",__func__,pMsg->QMIError, pMsg->QMIResult);
		return -1;
	}
	else 
	{
		// judge message length
		msgLen -= sizeof(QMI_TLV_HDR);
		pMsgStart += sizeof(QMI_TLV_HDR);
		msgLen -= pMsg->TLVLength;
		pMsgStart += pMsg->TLVLength;
		msgLen -= sizeof(QMI_TLV_HDR);
		pMsgStart += sizeof(QMI_TLV_HDR);
		msgLen -= pMsg->TLV2Length;
		pMsgStart += pMsg->TLV2Length;
		
		pVersion = (QMUX_TYPE_VERSION_STRUCT*) &(pMsg->TypeVersion);
		
		for ( i = 0; i < pMsg->NumElements; ++i)
		{
			if (pVersion->QMUXType == QMUX_TYPE_CTL)
			{
			    lc_kdbg( "%s: get qmi control version,MajorVersion=%u,MinorVersion=%u.\n",
						__func__,pVersion->MajorVersion,pVersion->MinorVersion);
				
				if (pVersion->MajorVersion >= 1 && pVersion->MinorVersion >= 3)
				{
				    pndis_status->fIPPacketSupport = true;
					lc_kdbg("%s: IP Supported,MajorVersion=%u,MinorVersion=%u.\n",
							__func__,pVersion->MajorVersion, pVersion->MinorVersion);
					sprintf(pndis_status->fVersion,"%u.%u",pVersion->MajorVersion, pVersion->MinorVersion);
				}
			}
			
			++pVersion;
		}
		
		// extra length
		while (msgLen >= sizeof(QMI_TLV_HDR))
		{            
			QMI_TLV_HDR* pHdr = (QMI_TLV_HDR*) pMsgStart;
			UInt8 *pCurMsg = NULL;
			UInt8 numInstances;
			
			lc_kdbg("%s: extra tlv process.",__func__);
			msgLen -= sizeof(QMI_TLV_HDR);
			pMsgStart += sizeof(QMI_TLV_HDR);
			pCurMsg = pMsgStart;
			msgLen -= pHdr->TLVLength;
			pMsgStart += pHdr->TLVLength;
			
			if (msgLen < 0)
			{
			    break;
			}
			
			switch (pHdr->TLVType)
		    {
		        case QMICTL_GETVERSION_RSP_TLV_TYPE_ADD_VERSION:
				{
					// length
					pCurMsg += *pCurMsg;
					pCurMsg += 1;
					
					numInstances = *pCurMsg;
					pCurMsg += 1;
					
					pVersion = (QMUX_TYPE_VERSION_STRUCT*) &(pCurMsg);
					
					for ( i = 0; i < numInstances; ++i)
					{
						if (pVersion->QMUXType == QMUX_TYPE_CTL)
						{
							lc_kdbg( "%s: get qmi control version,MajorVersion=%u,MinorVersion=%u.\n",
									__func__,pVersion->MajorVersion, pVersion->MinorVersion);
							
							if (pVersion->MajorVersion >= 1 && pVersion->MinorVersion >= 3)
							{
								pndis_status->fIPPacketSupport = true;
								lc_kdbg( "%s: IP Supported,MajorVersion=%u,MinorVersion=%u.\n",
										__func__, pVersion->MajorVersion, pVersion->MinorVersion);
								sprintf(pndis_status->fVersion,"%u.%u",pVersion->MajorVersion, pVersion->MinorVersion);
							}
						}
						
						++pVersion;    
					}					
				    break;
				}
				default:
				{
				    break;
				}
			}
			
		}
	}
	
/*	if (fGetVersioning)
	{
		fGetVersioning = false;
		fCommandGate->commandWakeup(NULL, false);
		XTRACE(this, 0, 0, "QCTL_Procecess_GetVersion_Respons: now wake up command gate");
	}*/
	lc_kdbg("%s : leave\n",__func__);
	
    return 0;
}
/********************************************************************
 *
 *         Name:  QCQMI_ProcessQMUXWDSResponse
 *  Description:  QCQMI Process QMUX WDS Response
 *        Input:  pQmi:the data getted from qmi server
 *        Output: pndis_status: ndis global status parameters. 
 *       Return:  -1 : call faled.
 *                0  : call success
 *        Notes:  
 ********************************************************************/
int QCQMI_ProcessQMUXWDSResponse(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status)
{
	QCQMUX* pQmux = NULL;
	QCQMUX_MSG_HDR* pQmuxMsg = NULL; 
	UInt16 totalLength = 0;
	bool bCompound = false;
	bool bDone     = false;
	int ret = -1;
	
	lc_kdbg("%s : entry\n",__func__);
	
	pQmux = (QCQMUX*)&(pQmi->SDU);
	pQmuxMsg = (QCQMUX_MSG_HDR*)&(pQmux->Message);
	
	// message length
	totalLength = pQmi->Length;
	
	totalLength -= sizeof(QCQMI_HDR);
	totalLength += sizeof(pQmi->IFType);
	
	totalLength -=sizeof(QCQMUX);
	totalLength += sizeof(pQmux->Message);	
	
	if (pQmux->CtlFlags & QMUX_CTL_FLAG_MASK_COMPOUND)
	{
	    bCompound = true;
		lc_kdbg("%s:compounded message.\n",__func__);
	}
	
	while (bDone == false)
	{
	    switch (pQmuxMsg->Type)
		{
		    case QMIWDS_START_NETWORK_INTERFACE_RESP:
			{
				QMIWDS_START_NETWORK_INTERFACE_RESP_MSG* pStartMsg = (QMIWDS_START_NETWORK_INTERFACE_RESP_MSG*)pQmuxMsg;
				
				lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMIWDS_START_NETWORK_INTERFACE_RESP.\n");				
				ret = QMI_CONNECT;
				if (pStartMsg->QMUXResult != QMI_RESULT_SUCCESS)
				{
					// to do
				    lc_kdbg("QCQMI_ProcessQMUXWDSResponse:Connection Failed,QMUXError=%u.\n",pStartMsg->QMUXError);
					pndis_status->fPacketHandle    = 0;
					//if((pndis_status->fConnectionError == -1) && (pndis_status->fConnectionError != 0))
					if(pndis_status->fConnecting)
					{
						pndis_status->fConnectionError = pStartMsg->QMUXError; 
						lc_kdbg("kaibo : QCQMI_ProcessQMUXWDSResponse:No connect has ok.\n");
					}
					else
					{
						lc_kdbg("kaibo : QCQMI_ProcessQMUXWDSResponse:One connect has ok.Iogren it\n");
											}
					// release commandgate
					if (pndis_status->fConnecting)
					{
						pndis_status->fConnecting = false;
					//	fCommandGate->commandWakeup(NULL, false);
						lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMIWDS_START_NETWORK_INTERFACE_RESP command wake up.\n");				   
					}
				}
				else 
				{
					if(pndis_status->fPacketHandle)
						pndis_status->fPacketHandle = le32_to_cpu(pStartMsg->Pkt_Data_Handle);
					if(!pndis_status->fPacketHandle)
						pndis_status->fPacketHandle1 = le32_to_cpu(pStartMsg->Pkt_Data_Handle);
					pndis_status->fConnectionError = 0;
					lc_kdbg("QCQMI_ProcessQMUXWDSResponse:Connection Succeeded,Pkt_Data_Handle=%u!\n", pStartMsg->Pkt_Data_Handle);
					
					// release commandgate
					if (pndis_status->fConnecting && !pndis_status->fIPModeEnabled)
					{
						pndis_status->fConnecting = false;
					//	fCommandGate->commandWakeup(NULL, false);
						lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMIWDS_START_NETWORK_INTERFACE_RESP command wake up.\n");				   
					}
				}				
			    break;
			}
			case QMIWDS_STOP_NETWORK_INTERFACE_RESP:
			{				
				QMIWDS_RESP_MSG_HEADER* pMsgHeader = (QMIWDS_RESP_MSG_HEADER*) pQmuxMsg;
				
				lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMIWDS_STOP_NETWORK_INTERFACE_RESP");
				ret = QMI_DISCONNECT;
				if (pMsgHeader->QMUXResult != QMI_RESULT_SUCCESS)
				{
					pndis_status->fConnectionError = pMsgHeader->QMUXError;
					lc_kdbg("QCQMI_ProcessQMUXWDSResponse:Disconnection Failed,QMUXError=%u.\n",pMsgHeader->QMUXError);
				}
				else 
				{
				    pndis_status->fPacketHandle    = 0;	
					pndis_status->fConnectionError = 0;
				}
			
				if (pndis_status->fDisconnecting)
				{
					pndis_status->fDisconnecting = false;
					//fCommandGate->commandWakeup(NULL, false);
					lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMIWDS_STOP_NETWORK_INTERFACE_RESP command wake up.\n");					    
				} 
					
				break;
			}
			
			case QMIWDS_GET_RUNTIME_SETTINGS_RESP:
			{
				lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMIWDS_GET_RUNTIME_SETTINGS_RESP.\n");
			    QC_WDS_Process_GetRunTimeSettings_Respons(pQmuxMsg,pndis_status);
			    ret = QMI_GET_IP_ADDR;
				break;
			}
		
			
			case QMI_WDS_MODIFY_PROFILE_SETTINGS_RESP:
			{
				lc_kdbg("QCQMI_ProcessQMUXWDSResponse:QMI_WDS_MODIFY_PROFILE_SETTINGS_RESP.\n");
			    ret = QMI_MODIFY_PRO;
				break;
			}
		    default:
			{
			    break;
			}
		}
		
		// no need to process again
		if (!bCompound)
		{
		    break;
		}
		
		totalLength -= sizeof(QCQMUX_MSG_HDR);
		totalLength -= pQmuxMsg->Length;
		
		if (totalLength > sizeof(PQCQMUX_MSG_HDR))//PQCQMUX_MSG_HDR
		{
		    bDone = false;
		}
		else 
		{
		    bDone = true;	
		}
		
		// move pointer
		pQmuxMsg = (PQCQMUX_MSG_HDR)((UInt8 *)pQmuxMsg + sizeof(QCQMUX_MSG_HDR) + pQmuxMsg->Length);
	}
	lc_kdbg("%s : leave\n",__func__);
	
	return ret;
}
/********************************************************************
 *
 *         Name:  QC_WDS_Process_GetRunTimeSettings_Respons
 *  Description:  QC WDS Process Get RunTime Settings Respons
 *        Input:  pQMuxMsg:the data getted from qmi server
 *        Output: pndis_status: ndis global status parameters. 
 *       Return:  -1 : call faled.
 *                0  : call success
 *        Notes:  
 ********************************************************************/
int  QC_WDS_Process_GetRunTimeSettings_Respons(QCQMUX_MSG_HDR* pQMuxMsg,NDIS_QMI_STATUS* pndis_status)
{
    QMIWDS_GET_RUNTIME_SETTINGS_RESP_MSG* pGetMsg = (QMIWDS_GET_RUNTIME_SETTINGS_RESP_MSG*)pQMuxMsg;
	int ret = 0;
	
	lc_kdbg("%s : entry\n",__func__);
	
	if (pGetMsg->QMUXResult != QMI_RESULT_SUCCESS)
	{
	    pndis_status->fIPAddress = 0;
	    pndis_status->fConnectionError = pGetMsg->QMUXError;
		lc_kdbg("%s: failed to get ip address,QMUXError=%u,QMUXResult=%u.\n",__func__,
				pGetMsg->QMUXError, pGetMsg->QMUXResult);
		ret = -1;
	}
	else 
	{
		UInt32 remainingBytes = 0;
	    UInt8 *pMsgStart = (UInt8 *)&(pGetMsg->TLVType);
		UInt8 *pMsgPtr   = (UInt8 *)&(pGetMsg->QMUXError);
		pMsgPtr += sizeof(UInt16);
		
		remainingBytes = pQMuxMsg->Length - (pMsgStart - pMsgPtr);
		
		if (remainingBytes >= sizeof(QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV4_ADDR))
		{
			QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV4_ADDR* pAddr = (QMIWDS_GET_RUNTIME_SETTINGS_TLV_IPV4_ADDR*)pMsgPtr;
			
			if (pAddr->TLVType == QMIWDS_GET_RUNTIME_SETTINGS_TLV_TYPE_IPV4)
			{
				pndis_status->fIPAddress = ntohl(pAddr->IPV4Address);
				
				// linkup net card
				if (pndis_status->fIPPacketSupport && pndis_status->fIPModeEnabled && pndis_status->fPacketHandle != 0)
				{
				//	IONetworkMedium	*medium;
				//	IOMediumType mediumType = kIOMediumEthernet10BaseT | kIOMediumOptionFullDuplex;
				//	medium = IONetworkMedium::getMediumWithType(pndis_status->fMediumDict, mediumType);
				//	setLinkStatus(kIONetworkLinkActive | kIONetworkLinkValid, medium, 10 * 1000000);
					pndis_status->fLinkStatus = 1;
				}			
				
				lc_kdbg("%s: successfully get ip address:IPV4Address=%u,fIPAddress=%u.\n",__func__,
				      pAddr->IPV4Address, pndis_status->fIPAddress);
				ret = 0;
			}
			else
			{
			    lc_kdbg("QC_WDS_Process_GetRunTimeSettings_Respons: invalid tlv type=%u.\n",pAddr->TLVType);
			    ret = -1;
			}
		}
	}
#if 0
	if (fGetIpAddressing)
	{
	    fGetIpAddressing = false;
		fCommandGate->commandWakeup(NULL, false);
		XTRACE(this, 0, 0, "QC_WDS_Process_GetRunTimeSettings_Respons: now wake up command gate");
	}
#endif
	lc_kdbg("%s : leave\n",__func__);
	
	return ret;
}
/********************************************************************
 *
 *         Name:  processQMIResponse
 *  Description:  process QMI Respons
 *        Input:  buffer:the data getted from qmi server
 *        Output: pndis_status: ndis global status parameters. 
 *       Return:  -1 : call faled.
 *                QMI_GET_CLIENT_ID--QMI_GET_VERSION  : call success
 *        Notes:  
 ********************************************************************/
int processQMIResponse(void *buffer,NDIS_QMI_STATUS* pndis_status)
{
    QCQMI* qmi = (QCQMI*)buffer;
    int    ret = -1;
	
	lc_kdbg("%s : entry.\n",__func__);
	lc_kdbg("qmi buffer is %02x, %04x, %02x, %02x, %02x\n", qmi->IFType, qmi->Length, qmi->CtlFlags, qmi->QMIType, qmi->ClientId);
	
	if (qmi->CtlFlags != QCQMI_CTL_FLAG_SERVICE)
	{
	    lc_kdbg("%s:Unexpected QMI control flags,IFType=%u,CtlFlags=%u.\n",__func__,qmi->IFType, qmi->CtlFlags);
		return -1;
	}
	
	if (QMUX_TYPE_CTL == qmi->QMIType)
	{
	    // handle set_Instance_id and getclientid and release client id
		QCQMICTL_MSG_HDR* pCtlMsgHdr = (QCQMICTL_MSG_HDR*)&(qmi->SDU);
		UInt8 controlFlag;
		
		controlFlag = pCtlMsgHdr->CtlFlag & 0x03;
		
		switch (controlFlag)
	    {
			// response
			case QMICTL_CTL_FLAG_RSP:
			{
				switch (pCtlMsgHdr->QMICTLType)
				{
					case QMICTL_SET_INSTANCE_ID_RESP:
					{
						lc_kdbg("processQMIResponse:QMICTL_SET_INSTANCE_ID_RESP.\n");
						QCTL_Procecess_SetInstanceId_Response(qmi,&(pndis_status->fSetInstanceIdFail),&(pndis_status->fQMI_ID));
						ret = QMI_SET_INSTANCE;
						break;
					}
						
					case QMICTL_GET_CLIENT_ID_RESP:
					{
						lc_kdbg("processQMIResponse:QMICTL_GET_CLIENT_ID_RESP.\n");
						QCTL_Procecess_GetClient_ID_Response(qmi,&(pndis_status->fClientID),&(pndis_status->fQMIType));
						ret = QMI_GET_CLIENT_ID;
						break;
					}
						
					case QMICTL_RELEASE_CLIENT_ID_RESP:
					{
						lc_kdbg("processQMIResponse:QMICTL_RELEASE_CLIENT_ID_RESP.\n");
						QCTL_Procecess_ReleaseClient_ID_Response(qmi,&(pndis_status->fClientID),&(pndis_status->fQMIType));
						ret = QMI_RELEASE_CLIENT_ID;
						break;
					}
					case QMICTL_GET_VERSION_RESP:
					{
						lc_kdbg("processQMIResponse:QMICTL_GET_VERSION_RESP.\n");
						QCTL_Procecess_GetVersion_Respons(qmi,pndis_status);
						ret = QMI_GET_VERSION;
					    break;
					}
					case QMICTL_SET_DATA_FORMAT_RESP:
					{
					    lc_kdbg("processQMIResponse:QMICTL_SET_DATA_FORMAT_RESP.\n");
						QCTL_Procecess_SetDataFormat_Response(qmi,pndis_status);
						ret = QMI_SET_DATA_FORMAT;
						break;
					}
					default:
					{
						lc_kdbg("processQMIResponse:unkown control message,QMICTLType=%u.\n",pCtlMsgHdr->QMICTLType);
						return -1;
					}
				}
				
				break;
			}
			
			// Indication
			case QMICTL_CTL_FLAG_IND:
			{
			    switch (pCtlMsgHdr->QMICTLType)
				{
				    case QMICTL_REVOKE_CLIENT_IDIND:
					{
						lc_kdbg("processQMIResponse:QMICTL_REVOKE_CLIENT_IDIND.\n");
						// should reacquire id
						pndis_status->fClientID = 0;
						pndis_status->fQMIType  = 0;
					    break;
					}
					case QMICTL_INVALID_CLIENT_ID_IND:
					{
					    lc_kdbg("processQMIResponse:QMICTL_INVALID_CLIENT_ID_IND.\n");
					    break;
					}
					default:
					{
					    lc_kdbg("%s:unkown QMICTLType=%u.\n",__func__,pCtlMsgHdr->QMICTLType);
						return -1;
					}
				}
			}
			
			default:
			{
				lc_kdbg("%s:unkown controlFlag=%u.\n",__func__,controlFlag);
			    return -1;
			}
		}
	}
	else 
	{
	    UInt8 controlFlag = 0;
		QCQMUX* pQmux = NULL;
		QCQMUX_MSG_HDR* pQmuxMsg = NULL; 
	    
		pQmux = (QCQMUX*)&(qmi->SDU);
		pQmuxMsg = (QCQMUX_MSG_HDR*)&(pQmux->Message);
		
		controlFlag = pQmux->CtlFlags & QMUX_CTL_FLAG_MASK_TYPE;
		
		switch (controlFlag)
	    {
		    case QMUX_CTL_FLAG_TYPE_RSP:
			{
			    switch (qmi->QMIType)
			    {
				    case QMUX_TYPE_WDS:
					{
						lc_kdbg("processQMIResponse:QMUX_CTL_FLAG_TYPE_RSP-QMUX_TYPE_WDS.\n");
						ret = QCQMI_ProcessQMUXWDSResponse(qmi, pndis_status);						
					    break;
					}
					case QMUX_TYPE_DMS:
					{
						lc_kdbg("processQMIResponse:QMUX_CTL_FLAG_TYPE_RSP-QMUX_TYPE_DMS.\n");
					    break;
					}
					default:
					{
						lc_kdbg("processQMIResponse:QMUX_CTL_FLAG_TYPE_RSP-QMUXUnknown,QMIType=%u.\n", qmi->QMIType);
					    break;
					}
				}
				
				break;
			}
			case QMUX_CTL_FLAG_TYPE_IND:
			{   
				switch (qmi->QMIType)
			    {
			        case QMUX_TYPE_WDS:
					{
						lc_kdbg("processQMIResponse:QMUX_CTL_FLAG_TYPE_IND-QMUX_TYPE_WDS.\n");
						ret = QCQMI_ProcessQMUXWDSIndication(qmi, pndis_status);						
					    break;
					}
				    default:
					{
						lc_kdbg("processQMIResponse:QMUX_CTL_FLAG_TYPE_IND-QMUXUnknown,QMIType=%u.\n", qmi->QMIType);
					    break;
					}
			    }
				
			    break;
			}
		    default:
			{
			    lc_kdbg("%s:unkown controlFlag=%u.\n",__func__,controlFlag);
			    return -1;
			}
		}		
	}
	
	lc_kdbg("%s : leave\n",__func__);
	
	return ret;
}
/********************************************************************
 *
 *         Name:  QCQMI_ProcessQMUXWDSIndication
 *  Description:  QCQMI Process QMUX WDS Indication
 *        Input:  pQmi:the data getted from qmi server
 *        Output: pndis_status: ndis global status parameters. 
 *       Return:  -1 : call faled.
 *                0  : call success
 *        Notes:  
 ********************************************************************/
int QCQMI_ProcessQMUXWDSIndication(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status)
{
	QCQMUX* pQmux = NULL;
	QCQMUX_MSG_HDR* pQmuxMsg = NULL; 
	UInt16 totalLength = 0;
	bool bCompound = false;
	bool bDone     = false;
	
	lc_kdbg("%s : entry.\n",__func__);
	
	pQmux = (QCQMUX*)&(pQmi->SDU);
	pQmuxMsg = (QCQMUX_MSG_HDR*)&(pQmux->Message);
	
	// message length
	totalLength = pQmi->Length;
	
	totalLength -= sizeof(QCQMI_HDR);
	totalLength += sizeof(pQmi->IFType);
	
	totalLength -=sizeof(QCQMUX);
	totalLength += sizeof(pQmux->Message);
	
	
	
	if (pQmux->CtlFlags & QMUX_CTL_FLAG_MASK_COMPOUND)
	{
	    bCompound = true;
	}
	
	while (bDone == false)
	{
	    switch (pQmuxMsg->Type)
		{
		    case QMIWDS_GET_PKT_SRVC_STATUS_IND:
			{				
				QMIWDS_GET_PKT_SRVC_STATUS_IND_MSG* pPktInd = (QMIWDS_GET_PKT_SRVC_STATUS_IND_MSG*)pQmuxMsg;
		        
		        lc_kdbg("QCQMI_ProcessQMUXWDSIndication:QMIWDS_GET_PKT_SRVC_STATUS_IND.\n");
		        
				if (pPktInd->ConnectionStatus == QWDS_PKT_DATA_CONNECTED)
				{
				    if (pPktInd->ReconfigRequired == 0)
					{
						if (!pndis_status->fIPModeEnabled)
						{
/* 						    IONetworkMedium	*medium;
						    IOMediumType mediumType = kIOMediumEthernet10BaseT | kIOMediumOptionFullDuplex;
						    medium = IONetworkMedium::getMediumWithType(fMediumDict, mediumType);
						    setLinkStatus(kIONetworkLinkActive | kIONetworkLinkValid, medium, 10 * 1000000); */
							pndis_status->fLinkStatus = 1;
						}
						lc_kdbg("QCQMI_ProcessQMUXWDSIndication:QMIWDS_GET_PKT_SRVC_STATUS_IND connected.\n");
					}
					else 
					{
					    // make disconnection	
						//setLinkStatus(kIONetworkLinkValid, 0); 
						//ResetPacketCount();
						pndis_status->fLinkStatus = 0;
						pndis_status->fPacketHandle    = 0;
						pndis_status->fIPAddress = 0;
						lc_kdbg("QCQMI_ProcessQMUXWDSIndication:QMIWDS_GET_PKT_SRVC_STATUS_IND disconnected.\n");
					}

				}
				else 
				{
				    // make disconnection	
					//setLinkStatus(kIONetworkLinkValid, 0);
					pndis_status->fLinkStatus = 0;
					pndis_status->fPacketHandle    = 0;
					pndis_status->fIPAddress = 0;
					lc_kdbg("QCQMI_ProcessQMUXWDSIndication:QMIWDS_GET_PKT_SRVC_STATUS_IND disconnected.\n");
					//ResetPacketCount();
					pndis_status->fLinkStatus = 0;
					
				}
				
				// release commandgate when ip enabled
				if (pndis_status->fConnecting)
				{
					pndis_status->fConnecting = false;
				//	pndis_status->fCommandGate->commandWakeup(NULL, false);
					lc_kdbg("QCQMI_ProcessQMUXWDSIndication:QMIWDS_GET_PKT_SRVC_STATUS_IND command wake up.\n");				   
				}
				
			    break;
			}
		    default:
			{
				break;
			}
		}
		
		// no need to process again
		if (!bCompound)
		{
		    break;
		}
		
		totalLength -= sizeof(QCQMUX_MSG_HDR);
		totalLength -= pQmuxMsg->Length;
		
		if (totalLength > sizeof(PQCQMUX_MSG_HDR))//PQCQMUX_MSG_HDR
		{
		    bDone = false;
		}
		else 
		{
		    bDone = true;	
		}
		
		// move pointer
		pQmuxMsg = (PQCQMUX_MSG_HDR)((UInt8 *)pQmuxMsg + sizeof(QCQMUX_MSG_HDR) + pQmuxMsg->Length);
	}
	
	lc_kdbg("%s : leave\n",__func__);
	
	return 0;
}
