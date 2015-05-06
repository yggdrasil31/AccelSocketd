//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : AccelSocketd_server.c (SOURCE)
//----------------------------------------------------------------------------//
// DESC : the namespaced local socket server source code.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocketd_server.c,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 24/07/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//
  
//****************************************************************************//
// INCLIB
//****************************************************************************//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/un.h> 
#include <syslog.h>


//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include <libAccelSocket.h>
#include "AccelSocketd_server.h"
#include "AccelSocketd_i2c.h"

//****************************************************************************//
// DEFINITION
//****************************************************************************// 

//****************************************************************************//
// MACRO
//****************************************************************************//
  
//****************************************************************************//
// TYPEDEF
//****************************************************************************//

//****************************************************************************//
// CONST
//****************************************************************************//

//****************************************************************************//
// EXPORT
//****************************************************************************//

//****************************************************************************//
// INTERN
//****************************************************************************//
int									s32ServerSocket = -1;
struct sockaddr_un	stServerAddress;
socklen_t						s32AddressLength;

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//


elibAccelSocketBool bServer_init(void)
{	
	elibAccelSocketBool lvbResult = FALSE;
	s32AddressLength = sizeof(struct sockaddr_un);
	
	if((s32ServerSocket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0)
	{
		memset(&stServerAddress, 0, sizeof(stServerAddress));
		stServerAddress.sun_family = AF_UNIX;
		strcpy(stServerAddress.sun_path, SERVER_SOCKET_NAME);
		
		unlink(SERVER_SOCKET_NAME);
		
		if(bind(s32ServerSocket, (const struct sockaddr *) &stServerAddress, sizeof(stServerAddress)) < 0)
		{
			// error while binding
			vServer_terminate();
		}
		else
		{
			lvbResult = TRUE;
		}
	}
	
	syslog(LOG_INFO, "bServer_init : returns %d while creating local namespace socket %u",lvbResult,s32ServerSocket);
	
	return lvbResult;
}


void vServer_processListen(void)
{	
	int												lvs32BytesReceived;
	int												lvs32BytesSent;
	TstLibAccelSocketFrame		lts8Request = {0};
	TstLibAccelSocketFrame		lts8Reply = {0};
	struct sockaddr_un				lstClientAddress;
	uint8_t										lvu8Data;
	TstAccel_XYZ							lstAccel = {0};
		
	lvs32BytesReceived = recvfrom(s32ServerSocket,
																lts8Request,
																SERVER_SOCKET_MAX_FRAME_SIZE,
																0, 
																(struct sockaddr *) &(lstClientAddress),
																&s32AddressLength);
	
	syslog(LOG_INFO, "vServer_processListen : %d bytes received",lvs32BytesReceived);
	
	if (	(lvs32BytesReceived>=1)
			&&(lvs32BytesReceived<=SERVER_SOCKET_MAX_FRAME_SIZE)	)
	{
		lts8Reply[SERVER_OFFSET_CMD] = lts8Request[SERVER_OFFSET_CMD];
		switch (lts8Request[SERVER_OFFSET_CMD])
		{
			case SERVER_CMD_SET_DATA_RATE:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_SET_DATA_RATE received");
				if (I2c_bSetDataRate(lts8Request[SERVER_OFFSET_PARAM1])== FALSE)
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}				 
				break;
				
			case SERVER_CMD_GET_DATA_RATE:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_GET_DATA_RATE received");
				if (I2c_bGetDataRate(&lvu8Data))
				{
					lts8Reply[SERVER_OFFSET_PARAM1] = lvu8Data;
				}
				else
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}
				break;
				
			case SERVER_CMD_SET_SCALE_RANGE:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_SET_SCALE_RANGE received");
				if (I2c_bSetScaleRange(lts8Request[SERVER_OFFSET_PARAM1]) == FALSE)
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}
				break;
				
			case SERVER_CMD_GET_SCALE_RANGE:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_GET_SCALE_RANGE received");
				if (I2c_bGetScaleRange(&lvu8Data))
				{
					lts8Reply[SERVER_OFFSET_PARAM1] = lvu8Data;
				}
				else
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}
				break;
				
			case SERVER_CMD_SET_SELFTEST_MODE:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_SET_SELFTEST_MODE received");				
				if (I2c_bSetSelfTestMode(lts8Request[SERVER_OFFSET_PARAM1]) == FALSE)
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}
				break;
			
			/*
			case SERVER_CMD_SET_INTERRUPT:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_SET_INTERRUPT received");
				break;
				
			case SERVER_CMD_CLEAR_INTERRUPT:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_CLEAR_INTERRUPT received");
				break;
			*/
				
			case SERVER_CMD_GET_XYZ:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_GET_XYZ received");
				if (I2c_bGetXYZ(&lstAccel))
				{
					memcpy(&lts8Reply[SERVER_OFFSET_PARAM1],&lstAccel,sizeof(TstAccel_XYZ));					
				}
				else
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}				
				break;
				
			case SERVER_CMD_READ_REGISTER:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_READ_REGISTER received");
				if(I2c_bReadRegister(lts8Request[SERVER_OFFSET_PARAM1], &lvu8Data))
				{
					lts8Reply[SERVER_OFFSET_PARAM1] = lvu8Data;
				}
				else
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}
				break;
				
			case SERVER_CMD_WRITE_REGISTER:
				syslog(LOG_INFO, "vServer_processListen : SERVER_CMD_WRITE_REGISTER received");
				if(I2c_bWriteRegister(lts8Request[SERVER_OFFSET_PARAM1], lts8Request[SERVER_OFFSET_PARAM2]) == FALSE)
				{
					lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_ERROR;
				}
				break;
				
			default:
				syslog(LOG_INFO, "vServer_processListen : unsupported request received");
				lts8Reply[SERVER_OFFSET_CMD] = SERVER_CMD_UNSUPPORTED;
				break;
		}
		
		
		
		// Sending reply to the client
		lvs32BytesSent = sendto(s32ServerSocket,
														lts8Reply,
														SERVER_SOCKET_MAX_FRAME_SIZE,
														0,
														(struct sockaddr *) &(lstClientAddress), 
														s32AddressLength);
		syslog(LOG_INFO, "vServer_processListen : %u bytes sent",lvs32BytesSent);
	}

}	 


void vServer_terminate()
{
	if(s32ServerSocket>=0)
	{
		syslog(LOG_INFO, "vServer_terminate : closing local namespace socket %u",s32ServerSocket);
		close (s32ServerSocket); 
		s32ServerSocket = -1;
		
		// Remove the socket file
		unlink (SERVER_SOCKET_NAME); 		
	}
	else
	{		
		syslog(LOG_INFO, "vServer_terminate : local namespace socket already closed\n");
	}
}



