//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : AccelSocketd_Server.h (SOURCE)
//----------------------------------------------------------------------------//
// DESC : the namespaced local socket server header.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocketd_server.h,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 24/07/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//

#ifndef __ACCELSOCKETD_SERVER_H__
#define __ACCELSOCKETD_SERVER_H__

//****************************************************************************//
// INCLIB
//****************************************************************************//


//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include <libAccelSocket.h>

//****************************************************************************//
// DEFINITION
//****************************************************************************// 

#define SERVER_SOCKET_NAME									"/tmp/myAccelSocketServer"		// Unix Domain Socket name
#define SERVER_SOCKET_MAX_FRAME_SIZE				16

#define SERVER_OFFSET_CMD										0
#define SERVER_OFFSET_PARAM1								1
#define SERVER_OFFSET_PARAM2								2


#define SERVER_CMD_SET_DATA_RATE				0
#define SERVER_CMD_GET_DATA_RATE				1
#define SERVER_CMD_SET_SCALE_RANGE			2
#define SERVER_CMD_GET_SCALE_RANGE			3
#define SERVER_CMD_SET_SELFTEST_MODE		4
/*
#define SERVER_CMD_SET_INTERRUPT				5
#define SERVER_CMD_CLEAR_INTERRUPT			6
*/
#define SERVER_CMD_GET_XYZ							7
#define SERVER_CMD_READ_REGISTER				8
#define SERVER_CMD_WRITE_REGISTER				9
#define SERVER_CMD_UNSUPPORTED					10
#define SERVER_CMD_ERROR								11

//****************************************************************************//
// MACRO
//****************************************************************************//
  
//****************************************************************************//
// TYPEDEF
//****************************************************************************//
typedef char TstLibAccelSocketFrame[SERVER_SOCKET_MAX_FRAME_SIZE];

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
elibAccelSocketBool bServer_init(void);
void vServer_processListen(void);
void vServer_terminate();

#endif





