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
#define LIBACCELSOCKET_MAX_SOCKETNAME_SIZE					64


//****************************************************************************//
// MACRO
//****************************************************************************//
  
//****************************************************************************//
// TYPEDEF
//****************************************************************************//

//****************************************************************************//
// CONST
//****************************************************************************//
const char ps8libAccelVersion[] = LIBACCELSOCKET_VERSION;


//****************************************************************************//
// EXPORT
//****************************************************************************//

//****************************************************************************//
// INTERN
//****************************************************************************//
int									s32ClientSocket = -1;
struct sockaddr_un	stClientAddress;
struct sockaddr_un 	stServerAddress;
char 								ts8ClientSocketName[LIBACCELSOCKET_MAX_SOCKETNAME_SIZE];

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//
elibAccelSocketBool libAccelSocket_bComServer(TstLibAccelSocketFrame ats8Request, TstLibAccelSocketFrame ats8Reply);



elibAccelSocketBool libAccelSocket_bComServer(TstLibAccelSocketFrame ats8Request, TstLibAccelSocketFrame ats8Reply)
{	
	int32_t										lvs32BytesSent = 0;
	uint32_t 									lvu32Size = 0;
	elibAccelSocketBool				lvbResult = FALSE;
	
	
	// Try to send it		
	if (s32ClientSocket>=0)
	{
#ifdef DEBUG
		printf("libAccelSocket_bComServer : trying to send 0x%02X\n", ats8Request[0]);
#endif

		lvs32BytesSent = sendto(s32ClientSocket, (char *) ats8Request, SERVER_SOCKET_MAX_FRAME_SIZE, 0, (struct sockaddr *) &stServerAddress, sizeof(struct sockaddr_un));

#ifdef DEBUG
		printf("libAccelSocket_bComServer : %d bytes sent\n", lvs32BytesSent);
#endif

		if (lvs32BytesSent == SERVER_SOCKET_MAX_FRAME_SIZE)
		{				
			lvu32Size = read(s32ClientSocket, (char *) ats8Reply, SERVER_SOCKET_MAX_FRAME_SIZE);
			if (lvu32Size == SERVER_SOCKET_MAX_FRAME_SIZE)
			{
				lvbResult = TRUE;			
#ifdef DEBUG
				printf("libAccelSocket_bComServer : %d bytes received from server\n",lvu32Size);
#endif
			}
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


char* libAccelSocket_ps8GetVersion(void)
{
	return ps8libAccelVersion;
}


elibAccelSocketBool libAccelSocket_bOpen(char* aps8ClientSocketName)
{	
	elibAccelSocketBool lvbResult = FALSE;
			
	if (strlen(aps8ClientSocketName)<LIBACCELSOCKET_MAX_SOCKETNAME_SIZE)
	{
		if((s32ClientSocket = socket(AF_UNIX, SOCK_DGRAM, 0)) >= 0)
		{
			strcpy(ts8ClientSocketName,aps8ClientSocketName);
		
			memset(&stClientAddress, 0, sizeof(struct sockaddr_un));
			stClientAddress.sun_family = AF_UNIX;
			strcpy(stClientAddress.sun_path, ts8ClientSocketName);
			
			unlink(ts8ClientSocketName);
			if(bind(s32ClientSocket, (const struct sockaddr *) &stClientAddress, sizeof(struct sockaddr_un))>=0)
			{
				memset(&stServerAddress, 0, sizeof(struct sockaddr_un));
				stServerAddress.sun_family = AF_UNIX;
				strcpy(stServerAddress.sun_path, SERVER_SOCKET_NAME);
				
				lvbResult = TRUE;
			}
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
		unlink (ts8ClientSocketName);
	}
	else
	{
#ifdef DEBUG
		printf("libAccelSocket_vClose : local namespace socket already closed\n");
#endif		
	}
}


elibAccelSocketBool libAccelSocket_bSetDataRate(elibAccelSocketRate avu8DataRateConfiguration)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
	
	
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_SET_DATA_RATE;
	lts8Request[SERVER_OFFSET_PARAM1] = avu8DataRateConfiguration;
	
	// Send it to the server
	if(libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		// Get the result
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}


elibAccelSocketBool libAccelSocket_bGetDataRate(elibAccelSocketRate* apu8DataRateConfiguration)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
	
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_GET_DATA_RATE;
	
	// Send it to the server
	if (libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			*apu8DataRateConfiguration = lts8Reply[SERVER_OFFSET_PARAM1];
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}

elibAccelSocketBool libAccelSocket_bSetScaleRange(elibAccelSocketScale avu8ScaleRangeConfiguration)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
	
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_SET_SCALE_RANGE;
	lts8Request[SERVER_OFFSET_PARAM1] = avu8ScaleRangeConfiguration;
	
	// Send it to the server
	if(libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		// Get the result
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}	


elibAccelSocketBool libAccelSocket_bGetScaleRange(elibAccelSocketScale* apu8ScaleRangeConfiguration)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
	
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_GET_SCALE_RANGE;
	
	// Send it to the server
	if (libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			*apu8ScaleRangeConfiguration = lts8Reply[SERVER_OFFSET_PARAM1];
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}	


elibAccelSocketBool libAccelSocket_bSetSelftestMode(elibAccelSocketSelfTestMode avu8Mode)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
	
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_SET_SELFTEST_MODE;
	lts8Request[SERVER_OFFSET_PARAM1] = avu8Mode;
	
	// Send it to the server
	if(libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		// Get the result
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}

/*
elibAccelSocketBool libAccelSocket_bSetInterrupt(TstLibAccelSocketFrame ats8Reply, uint32_t* apu32Size)
{
	elibAccelSocketBool				lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	
	
	// Prepare the request frame
	lts8Request[0] = SERVER_CMD_SET_INTERRUPT;
	
	// Send it to the server
	lvbResult = libAccelSocket_bComServer(lts8Request, ats8Reply);
		
	return lvbResult;
}	


elibAccelSocketBool libAccelSocket_bClearInterrupt(TstLibAccelSocketFrame ats8Reply, uint32_t* apu32Size)
{
	elibAccelSocketBool				lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	
	
	// Prepare the request frame
	lts8Request[0] = SERVER_CMD_CLEAR_INTERRUPT;
	
	// Send it to the server
	lvbResult = libAccelSocket_bComServer(lts8Request, ats8Reply);
		
	return lvbResult;
}
*/


elibAccelSocketBool libAccelSocket_bGetXYZ(TstAccel_XYZ* apstAccel)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
	
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_GET_XYZ;
	
	// Send it to the server
	if (libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			memcpy((char*)apstAccel, &lts8Reply[SERVER_OFFSET_PARAM1], sizeof(TstAccel_XYZ) );
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}


elibAccelSocketBool libAccelSocket_bReadRegister(uint8_t avu8Register, uint8_t* apu8Value)
{
	elibAccelSocketBool			lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
		
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_READ_REGISTER;
	lts8Request[SERVER_OFFSET_PARAM1] = avu8Register;
	
	// Send it to the server
	if(libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			*apu8Value = lts8Reply[SERVER_OFFSET_PARAM1];
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}	


elibAccelSocketBool libAccelSocket_bWriteRegister(uint8_t avu8Register, uint8_t avu8Value)
{
	elibAccelSocketBool				lvbResult = FALSE;
	TstLibAccelSocketFrame	lts8Request = {0};
	TstLibAccelSocketFrame	lts8Reply = {0};
		
	// Prepare the request frame
	lts8Request[SERVER_OFFSET_CMD] = SERVER_CMD_WRITE_REGISTER;
	lts8Request[SERVER_OFFSET_PARAM1] = avu8Register;
	lts8Request[SERVER_OFFSET_PARAM2] = avu8Value;
	
	// Send it to the server
	if (libAccelSocket_bComServer(lts8Request, lts8Reply))
	{
		if (lts8Reply[SERVER_OFFSET_CMD] == lts8Request[SERVER_OFFSET_CMD])
		{
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}	


