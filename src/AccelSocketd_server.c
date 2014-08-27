//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : AccelSocketd_UdpServer.c (SOURCE)
//----------------------------------------------------------------------------//
// DESC : the namespaced local socket server source code.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocketd.c,v $
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
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <sys/un.h> 
#include <syslog.h>


//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include "AccelSocketd_server.h"

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


int bServer_init(void)
{	
	int lvs32Result = FALSE;
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
			lvs32Result = TRUE;
		}
	}
	
	syslog(LOG_INFO, "bServer_init : returns %d while creating local namespace socket %u",lvs32Result,s32ServerSocket);
	
	return lvs32Result;
}


void vServer_processListen(void)
{	
	int									lvs32BytesReceived;
	int									lvs32BytesSent;
	char								las8Frame[SERVER_MAX_FRAME_SIZE] = {0};
	char								las8Reply[SERVER_MAX_FRAME_SIZE] = {0};
	struct sockaddr_un	stClientAddress;
		
	lvs32BytesReceived = recvfrom(s32ServerSocket,
																las8Frame,
																SERVER_MAX_FRAME_SIZE,
																0, 
																(struct sockaddr *) &(stClientAddress),
																&s32AddressLength);
	
	syslog(LOG_INFO, "vServer_processListen : %u bytes received",lvs32BytesReceived);
	
	if (lvs32BytesReceived>=1)
	{
		switch (las8Frame[0])
		{
			case SERVER_PROTOCOL_SET_DATA_RATE:
				break;
				
			case SERVER_PROTOCOL_GET_DATA_RATE:
				break;
				
			case SERVER_PROTOCOL_SET_SCALE_RANGE:
				break;
				
			case SERVER_PROTOCOL_GET_SCALE_RANGE:
				break;
				
			case SERVER_PROTOCOL_SET_SELFTEST_MODE:
				break;
				
			case SERVER_PROTOCOL_SET_INTERRUPT:
				break;
				
			case SERVER_PROTOCOL_CLEAR_INTERRUPT:
				break;
				
			case SERVER_PROTOCOL_GET_XYZ:
				break;
				
			case SERVER_PROTOCOL_READ_REGISTER:
				break;
				
			case SERVER_PROTOCOL_WRITE_REGISTER:
				break;
				
			default:
			case SERVER_PROTOCOL_QUIT:
				break;
		}
		
		// Sending reply to the client
		lvs32BytesSent = sendto(s32ServerSocket,
														las8Reply,
														SERVER_MAX_FRAME_SIZE,
														0,
														(struct sockaddr *) &(stClientAddress), 
														s32AddressLength);	
		syslog(LOG_INFO, "vServer_processListen : %u bytes sent",lvs32BytesSent);
	}

}	 


void vServer_terminate()
{
	if(s32ServerSocket>=0)
	{
		close (s32ServerSocket); 
		s32ServerSocket = -1;
		
		// Remove the socket file
		unlink (SERVER_SOCKET_NAME); 
		
		syslog(LOG_INFO, "vServer_terminate : local namespace socket %u closed",s32ServerSocket);
	}
}



