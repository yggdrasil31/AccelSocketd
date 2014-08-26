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
#define TRUE	1
#define FALSE	0

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
void* vThreadServer_EXE(void* pArg);


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
		bind(s32ListenSocket, (struct sockaddr*)&lstName, SUN_LEN (&lstName));

		// Listen for connections
		listen(s32ListenSocket, SERVER_MAX_CONNECTIONS_IN_BACKLOG);	
		
		lbResult = TRUE;
	}
	
	syslog(LOG_INFO, "bServer_init returns %d while creating local namespace socket %s",lbResult,ps8ListenSocketName);
	
	return lbResult;
}


void* vThreadServer_EXE(void* pArg)
{
		int			ls32ClientSocket = *((int*)pArg); 
		int			ls32Length = 0; 
		char*		lps8Text = NULL;
		int			ls32Result = 0; 
		
		syslog(LOG_INFO, "vThreadServer_EXE : server started on client socket %u", ls32ClientSocket);
		
		do 
		{
			/* First, read the length of the text message from the socket. If 
			read returns zero, the client closed the connection.  */
			ls32Result = read (ls32ClientSocket, &ls32Length, sizeof (ls32Length));
			if (ls32Result > 0) 
			{
				/* Allocate a buffer to hold the text.  */ 
				lps8Text = (char*) malloc (ls32Length); 
				
				/* Read the text itself, and print it.  */ 
				ls32Result = read (ls32ClientSocket, lps8Text, ls32Length);
				if (ls32Result > 0)
				{ 
					printf ("%s\n", lps8Text); 
					
					syslog(LOG_INFO, "vThreadServer_EXE : read %u bytes on client socket %u", ls32Result, ls32ClientSocket);
					/* If the client sent the message "quit," we're all done.  */ 
					if (strcmp (lps8Text, "quit"))
					{
						syslog(LOG_INFO, "vThreadServer_EXE : server closing requested on client socket %u", ls32Result);
						ls32Result = 0;
					}
				}
				else
				{
					syslog(LOG_INFO, "vThreadServer_EXE : error while reading %u bytes on client socket %u", ls32Result, ls32ClientSocket);
					ls32Result = -1;
				}
			}
			else
			{
				syslog(LOG_INFO, "vThreadServer_EXE : error while reading message size on client socket %u", ls32ClientSocket);
			}			
			
			if (lps8Text != NULL)
			{
				/* Free the buffer.  */ 
				free (lps8Text);
				lps8Text = NULL;
			}
		}
		while (ls32Result > 0);
		
		close(ls32ClientSocket);
		syslog(LOG_INFO, "vThreadServer_EXE : client socket %u closed", ls32ClientSocket);
		
		
		return NULL;
}

void vServer_processListen(void)
{	
	int					ls32ClientSocket; 
	int					ls32Result;
	pthread_t		lthreadServer;	

	// Accept a connection
	ls32ClientSocket = accept (s32ListenSocket, NULL, NULL); 

	// create a new server thread for each accepted connection
	if (ls32ClientSocket >=0)
	{
		ls32Result = pthread_create ( &lthreadServer,NULL, vThreadServer_EXE, (void*)&ls32ClientSocket);
		syslog(LOG_INFO, "vServer_processListen : returns %d while creating vThreadServer_EXE", ls32Result);
	}
	
	
}	 


void vServer_terminate()
{
	if(s32ListenSocket>=0)
	{
		/* Remove the socket file.   */ 
		close (s32ListenSocket); 
		unlink (ps8ListenSocketName); 
		
		s32ListenSocket = -1;
	}
}



