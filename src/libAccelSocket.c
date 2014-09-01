//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : libAccelSocket.c (SOURCE)
//----------------------------------------------------------------------------//
// DESC : the lib that connects application to the AccelSocket Daemon
//----------------------------------------------------------------------------//
// HIST : $Log: libAccelSocket.c,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 27/08/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//
  
//****************************************************************************//
// INCLIB
//****************************************************************************//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/un.h> 
#include <syslog.h>


//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include "AccelSocketd_server.h"
#include "libAccelSocket.h"

//****************************************************************************//
// DEFINITION
//****************************************************************************// 
#define CLIENT_SOCKET_NAME "/tmp/myAccelSocketClient"

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
int									s32ClientSocket = -1;
struct sockaddr_un	stClientAddress;
struct sockaddr_un 	stServerAddress;


//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//
elibAccelSocketBool libAccelSocket_bComServer(TYPE_LibAccelSocketFrame ats8Request, TYPE_LibAccelSocketFrame ats8Reply,uint32_t* apu32Size);



elibAccelSocketBool libAccelSocket_bOpen(void)
{	
	elibAccelSocketBool lvbResult = FALSE;
			
	if((s32ClientSocket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0)
	{
		memset(&stClientAddress, 0, sizeof(struct sockaddr_un));
		stClientAddress.sun_family = AF_UNIX;
		strcpy(stClientAddress.sun_path, CLIENT_SOCKET_NAME);
		
		unlink(CLIENT_SOCKET_NAME);
		if(bind(s32ClientSocket, (const struct sockaddr *) &stClientAddress, sizeof(struct sockaddr_un))>=0)
		{
			lvbResult = TRUE;
			
			memset(&stServerAddress, 0, sizeof(struct sockaddr_un));
			stServerAddress.sun_family = AF_UNIX;
			strcpy(stServerAddress.sun_path, SERVER_SOCKET_NAME);
		}		
	}
	
#ifdef DEBUG
	printf("libAccelSocket_bOpen : returns %d while creating local namespace socket %u\n",lvbResult,s32ClientSocket);
#endif
	
	return lvbResult;
}


void libAccelSocket_vClose(void)
{
	if(s32ClientSocket>=0)
	{
#ifdef DEBUG
		printf("libAccelSocket_vClose : closing local namespace socket %u\n",s32ClientSocket);
#endif
		
		close (s32ClientSocket); 
		s32ClientSocket = -1;
		
		// Remove the socket file
		unlink (CLIENT_SOCKET_NAME);
	}
	else
	{
#ifdef DEBUG
		printf("libAccelSocket_vClose : local namespace socket already closed\n");
#endif		
	}
}


elibAccelSocketBool libAccelSocket_bSetDataRate(TYPE_LibAccelSocketFrame ats8Reply,uint32_t* apu32Size)
{
	elibAccelSocketBool				lvbResult = FALSE;
	TYPE_LibAccelSocketFrame	lts8Request = {0};
	
	
	// Prepare the request frame
	lts8Request[0] = SERVER_PROTOCOL_SET_DATA_RATE;
	
	// Send it to the server
	lvbResult = libAccelSocket_bComServer(lts8Request, ats8Reply, apu32Size);
	
	return lvbResult;
}


elibAccelSocketBool libAccelSocket_bGetDataRate(TYPE_LibAccelSocketFrame ats8Reply,uint32_t* apu32Size)
{
	elibAccelSocketBool				lvbResult = FALSE;
	TYPE_LibAccelSocketFrame	lts8Request = {0};
	
	
	// Prepare the request frame
	lts8Request[0] = SERVER_PROTOCOL_GET_DATA_RATE;
	
	// Send it to the server
	lvbResult = libAccelSocket_bComServer(lts8Request, ats8Reply, apu32Size);
		
	return lvbResult;
}


/*
#define SERVER_PROTOCOL_GET_DATA_RATE				1
#define SERVER_PROTOCOL_SET_SCALE_RANGE			2
#define SERVER_PROTOCOL_GET_SCALE_RANGE			3
#define SERVER_PROTOCOL_SET_SELFTEST_MODE		4
#define SERVER_PROTOCOL_SET_INTERRUPT				5
#define SERVER_PROTOCOL_CLEAR_INTERRUPT			6
#define SERVER_PROTOCOL_GET_XYZ							7
#define SERVER_PROTOCOL_READ_REGISTER				8
#define SERVER_PROTOCOL_WRITE_REGISTER			9
*/


elibAccelSocketBool libAccelSocket_bComServer(TYPE_LibAccelSocketFrame ats8Request, TYPE_LibAccelSocketFrame ats8Reply,uint32_t* apu32Size)
{
	//socklen_t									lvs32AddressLength;
	int32_t										lvs32BytesSent = 0;
	elibAccelSocketBool				lvbResult = FALSE;
	
	// Try to send it	
	*apu32Size = 0;	
	if (s32ClientSocket>=0)
	{
#ifdef DEBUG
		printf("libAccelSocket_bComServer : trying to send 0x%02X\n", ats8Request[0]);
#endif

		lvs32BytesSent = sendto(s32ClientSocket, (char *) ats8Request, LIBACCELSOCKET_MAX_FRAME_SIZE, 0, (struct sockaddr *) &stServerAddress, sizeof(struct sockaddr_un));

#ifdef DEBUG
		printf("libAccelSocket_bComServer : %d bytes sent\n", lvs32BytesSent);
#endif

		if (lvs32BytesSent>0)
		{		
			//lvs32AddressLength = sizeof(struct sockaddr_un);
			//*apu32Size = recvfrom(s32ClientSocket, (char *) ats8Reply, LIBACCELSOCKET_MAX_FRAME_SIZE, 0, (struct sockaddr *) &(stServerAddress), &lvs32AddressLength);
			*apu32Size = read(s32ClientSocket, (char *) ats8Reply, LIBACCELSOCKET_MAX_FRAME_SIZE);
			lvbResult = TRUE;
			
#ifdef DEBUG
			printf("libAccelSocket_bComServer : %d bytes received from server\n",*apu32Size);
#endif

		}
		else
		{
#ifdef DEBUG
			printf("libAccelSocket_bComServer : could not send data to the server\n");
#endif
		}
	}
	else
	{
#ifdef DEBUG
		printf("libAccelSocket_bComServer : client socket not opened\n");
#endif
	}
		
	return lvbResult;
}
