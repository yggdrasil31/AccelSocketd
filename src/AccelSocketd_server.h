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
// DESC : the namespaced local socket server header.
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


//****************************************************************************//
// INCUSER
//****************************************************************************// 


//****************************************************************************//
// DEFINITION
//****************************************************************************// 
#define SERVER_MAX_FRAME_SIZE								256
#define SERVER_MAX_CONNECTIONS_IN_BACKLOG		5

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
int bServer_init(void* aps8ListenSocketName);
void vServer_processListen(void);
void vServer_terminate();




