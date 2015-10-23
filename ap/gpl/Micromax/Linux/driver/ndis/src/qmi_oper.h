#ifndef __QMI_OPER_H__
#define __QMI_OPER_H__

#include "qmi_header.h"
#define DEBUG

enum
{
    QMI_GET_CLIENT_ID			= 0,
    QMI_RELEASE_CLIENT_ID,
    QMI_CONNECT,
    QMI_DISCONNECT,
    QMI_GET_IP_ADDR,
    QMI_SET_DATA_FORMAT,
    QMI_SET_INSTANCE,
    QMI_GET_VERSION,
    QMI_MODIFY_PRO,
};

typedef struct _NDIS_QMI_STATUS{	
	UInt32           fPacketHandle;
	UInt32           fPacketHandle1;
	bool             fIPPacketSupport;
	bool             fDisconnecting;
	bool             fConnecting;
	bool             fGetVersioning;
	bool             fIPModeEnabled;	
	int              fConnectionStatus;
	int              fSetInstanceIdFail;
	UInt32           fQMI_ID;
	UInt32           fIPAddress;
	UInt16           fConnectionError;	
	UInt8    		 fQMIType;
	UInt8    		 fClientID;			
	UInt8			 fLinkStatus;	
	char             fVersion[64];	
} NDIS_QMI_STATUS;
/********************************************************************
 *
 *         Name:  printk_hex
 *  Description:  print data buffer with hex which is lined by 16 bytes
 *        Input:  buf:the data buffer to be print of
 *                buf_size:the data buffer length.
 *       Return:  null
 *        Notes:  
 ********************************************************************/
void printk_hex(UInt8 *buf,int buf_size);
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
int  QCTL_GetClientID(UInt8 * pMsgBuff,UInt32 length,UInt8 qmiType);
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
int QCTL_ReleaseClientID(UInt8* pMsgBuff,UInt32 length,NDIS_QMI_STATUS* pndis_status);
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
int QCTL_GetVersionReq(UInt8* pMsgBuff,UInt32 length);
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
int QCTL_SetDataFormatReq(UInt8* pMsgBuff,UInt32 length);
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
int QCTL_SetInstanceId(UInt8* pMsgBuff,UInt32 length);
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
int QC_WDS_GetIPAddress(UInt8* pMsgBuff,UInt32 length,UInt8 fClientID);

/********************************************************************
 *
 *         Name:  QC_WDS_ModifyProfile
 *  Description:  modify  profile for support ipv6 adn dulIP
 *        Input:  length:the pMsgBuff max length;
 *                fClientID:the current clientID 
 *               char ConnectIpMode			0 for ipv4 1 for ipv6 2 for dul
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
					UInt32 ConnectIpMode);


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
					UInt32 ConnectIpMode);					
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
int QC_WDS_Disconnect(UInt8* pMsgBuff,UInt32 length,NDIS_QMI_STATUS* pndis_status,UInt8 flag);
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
int QCTL_Procecess_GetClient_ID_Response(QCQMI * pQmi,UInt8* fClientID,UInt8* fQMIType);
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
int QCTL_Procecess_SetDataFormat_Response(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status);
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
int QCTL_Procecess_SetInstanceId_Response(QCQMI* pQmi,int* fSetInstanceIdFail,UInt32* fQMI_ID);
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
int QCTL_Procecess_ReleaseClient_ID_Response(QCQMI* pQmi,UInt8* fClientID,UInt8* fQMIType);
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
int QCTL_Procecess_GetVersion_Respons(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status);
 /********************************************************************
 *
 *         Name:  QCQMI_ProcessQMUXWDSResponse
 *  Description:  QCQMI Process QMUX WDS Response
 *        Input:  pQmi:the data buffer to be print of
 *        Output: pndis_status: ndis global status parameters. 
 *       Return:  null
 *        Notes:  
 ********************************************************************/
int QCQMI_ProcessQMUXWDSResponse(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status);
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
 int  QC_WDS_Process_GetRunTimeSettings_Respons(QCQMUX_MSG_HDR* pQMuxMsg,NDIS_QMI_STATUS* pndis_status);
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
int processQMIResponse(void *buffer,NDIS_QMI_STATUS* pndis_status);
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
int QCQMI_ProcessQMUXWDSIndication(QCQMI* pQmi,NDIS_QMI_STATUS* pndis_status);
#endif					
