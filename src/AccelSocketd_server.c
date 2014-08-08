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
// HIST : $Log: AccelSocketd.h,v $
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
#include <string.h> 
#include <sys/socket.h> 
#include <sys/un.h> 
#include <unistd.h> 
#include <sys/types.h> 
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
int							s32ListenSocket = -1;
const char* 		ps8ListenSocketName = NULL;

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//

int bServer_init(void* aps8ListenSocketName)
{	
	struct sockaddr_un	lstName; 
	int			 						lbResult = FALSE;
	
	ps8ListenSocketName = aps8ListenSocketName;
	
	// Create the socket
	s32ListenSocket = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (s32ListenSocket>=0)
	{	
		// Indicate that this is a server
		lstName.sun_family = AF_LOCAL;
		strcpy (lstName.sun_path, ps8ListenSocketName);
		bind(s32ListenSocket, &lstName, SUN_LEN (&lstName));

		// Listen for connections
		listen(s32ListenSocket, SERVER_MAX_CONNECTIONS_IN_BACKLOG);	
		
		lbResult = TRUE;
	}
	
	syslog(LOG_INFO, "bServer_init returns %d while creating local namespace socket %s",lbResult,lps8ListenSocketName);
	
	return lbResult;
}


void vServer_processListen(void)
{	
	struct sockaddr_un	lstClientName; 
	socklen_t						ls32ClientNameLen; 
	int 								ls32ClientSocket; 

	// Accept a connection
	ls32ClientSocket = accept (s32ListenSocket, &lstClientName, &ls32ClientNameLen); 

	// create a new server thread for each accepted connection
	if (ls32ClientSocket >=0)
	{
		
	}
	
	
}	 


void vServer_terminate()
{
	if(s16ListenSocket>=0)
	{
		/* Remove the socket file.   */ 
		close (s32ListenSocket); 
		unlink (ps8ListenSocketName); 
		
		s16ListenSocket = -1;
	}
}



