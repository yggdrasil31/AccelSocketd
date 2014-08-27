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

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//



int main(int argc, char **argv)
{	
	int lu32Choice;
	int	ls32Result;
	int ls32Quit = FALSE;
	
	
	ls32Result = libAccelSocket_bOpen();
	if (ls32Result != FALSE)
	{	
		do
		{
			printf ("AccelSocket Client :\r\n ");
			printf (" 1- SET_DATA_RATE\n");
			printf (" 2- GET_DATA_RATE\n");
			printf (" 3- SET_SCALE_RANGE\n");
			printf (" 4- GET_SCALE_RANGE\n");
			printf (" 5- SET_SELFTEST_MODE\n");
			printf (" 6- SET_INTERRUPT\n");
			printf (" 7- CLEAR_INTERRUPT\n");
			printf (" 8- GET_XYZ\n");
			printf (" 9- READ_REGISTER\n");
			printf ("10- WRITE_REGISTER\n");
			printf ("11- QUIT\n");
			
			scanf("your choice : %d", &lu32Choice);
			
			switch (lu32Choice-1)
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
					ls32Quit = TRUE;
					break;
			}
		}
		while (ls32Quit == FALSE);
			
		libAccelSocket_vClose();
	}
	
	return 0;
}

