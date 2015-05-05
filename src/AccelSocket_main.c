//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocket : Module Name
//##########################7#################################################//
// FILE : AccelSocket_main.c (SOURCE)
//----------------------------------------------------------------------------//
// DESC : the AccelSocket client source code.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocket.c,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 26/08/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//
  
//****************************************************************************//
// INCLIB
//****************************************************************************//
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include "libAccelSocket.h"
#include "AccelSocketd_server.h"
#include "lis3dh.h"


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

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//



int main(int argc, char **argv)
{		
	uint32_t									ls32Quit = FALSE;
	uint32_t									lu32Choice;
	elibAccelSocketBool				lbResult;
	uint32_t									lvu32Value;
	TstAccel_XYZ							lstAccel;
	uint8_t										lvu8Register;
	
	
	lbResult = libAccelSocket_bOpen(CLIENT_SOCKET_NAME);
	if (lbResult == TRUE)
	{	
		do
		{
			printf ("AccelSocket Client using lib %s:\r\n",libAccelSocket_ps8GetVersion());
			printf (" 0- SET_DATA_RATE\n");
			printf (" 1- GET_DATA_RATE\n");
			printf (" 2- SET_SCALE_RANGE\n");
			printf (" 3- GET_SCALE_RANGE\n");
			printf (" 4- SET_SELFTEST_MODE\n");
			/*
			printf (" 5- SET_INTERRUPT\n");
			printf (" 6- CLEAR_INTERRUPT\n");
			*/
			printf (" 7- GET_XYZ\n");
			printf (" 8- READ_REGISTER\n");
			printf (" 9- WRITE_REGISTER\n");
			printf ("10- QUIT\n");
			
			scanf("%d", &lu32Choice);
			
			printf ("executing %d\n",lu32Choice);
			switch (lu32Choice)
			{
				case SERVER_CMD_SET_DATA_RATE:
					libAccelSocket_bSetDataRate(9);
					break;
					
				case SERVER_CMD_GET_DATA_RATE:
					if(libAccelSocket_bGetDataRate(&lvu32Value))
					{
						printf("Data rate is %d\n",lvu32Value);
					}
					break;
					
				case SERVER_CMD_SET_SCALE_RANGE:
					libAccelSocket_bSetScaleRange(1);
					break;
					
				case SERVER_CMD_GET_SCALE_RANGE:
					
					if(libAccelSocket_bGetScaleRange(&lvu32Value))
					{
						printf("Scale range is %d\n",lvu32Value);
					}
					
					break;
					
				case SERVER_CMD_SET_SELFTEST_MODE:
					libAccelSocket_bSetSelftestMode(1);
					break;
				
				/*	
				case SERVER_CMD_SET_INTERRUPT:
					libAccelSocket_bSetInterrupt(ltReply,&lu32Size);
					break;
					
				case SERVER_CMD_CLEAR_INTERRUPT:
					libAccelSocket_bClearInterrupt(ltReply,&lu32Size);
					break;
				*/
					
				case SERVER_CMD_GET_XYZ:
					libAccelSocket_bGetXYZ(&lstAccel);
					{
						printf("x is %d\n",lstAccel.x);
						printf("y is %d\n",lstAccel.y);
						printf("z is %d\n",lstAccel.z);
					}
					break;
					
				case SERVER_CMD_READ_REGISTER:
					lvu8Register = LIS3DH_WHO_AM_I;
					libAccelSocket_bReadRegister(lvu8Register, (char*) &lvu32Value);
					printf("register LIS3DH_WHO_AM_I holds %02X\n",lvu32Value);
					break;
					
				case SERVER_CMD_WRITE_REGISTER:
					lvu8Register = LIS3DH_CTRL_REG3;
					libAccelSocket_bWriteRegister(lvu8Register, 0);
					break;
					
				default:
					ls32Quit = TRUE;
					break;
			}
		}
		while (ls32Quit == FALSE);
			
		libAccelSocket_vClose();
	}
	
	return 0;
}

