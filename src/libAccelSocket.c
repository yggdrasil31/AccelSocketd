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
#include <sys/types.h>
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
#define CLIENT_SOCKET_NAME "./AccelSocketClient"

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
socklen_t						s32AddressLength;

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//


int libAccelSocket_bOpen(void)
{	
	int lvs32Result = FALSE;
	s32AddressLength = sizeof(struct sockaddr_un);
		
	if((s32ClientSocket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0)
	{
		memset(&stClientAddress, 0, sizeof(struct sockaddr_un));
		stClientAddress.sun_family = AF_UNIX;
		strcpy(stClientAddress.sun_path, CLIENT_SOCKET_NAME);
		
		unlink(CLIENT_SOCKET_NAME);
		if(bind(s32ClientSocket, (const struct sockaddr *) &stClientAddress, sizeof(struct sockaddr_un))>=0)
		{
			lvs32Result = TRUE;
		}		
	}
	
#ifdef DEBUG
	printf("libAccelSocket_bOpen : returns %d while creating local namespace socket %u",lvs32Result,s32ClientSocket);
#endif
	
	return lvs32Result;
}


void libAccelSocket_vClose(void)
{
	if(s32ClientSocket>=0)
	{
		close (s32ClientSocket); 
		s32ClientSocket = -1;
		
		// Remove the socket file
		unlink (CLIENT_SOCKET_NAME);
		
#ifdef DEBUG
		printf("libAccelSocket_vClose : local namespace socket %u closed",s32ClientSocket);
#endif
	}
}



