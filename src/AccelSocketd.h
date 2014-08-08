//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : AccelSocketd.h (HEADER)
//----------------------------------------------------------------------------//
// DESC : the AccelSocketd daemon header.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocketd.h,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 24/07/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//

#ifndef __ACCELSOCKETD_H__
#define __ACCELSOCKETD_H__
  
//****************************************************************************//
// INCLIB
//****************************************************************************//

//****************************************************************************//
// INCUSER
//****************************************************************************// 

//****************************************************************************//
// DEFINITION
//****************************************************************************// 

// AccelSocketd version number
#define ACCELSOCKETD_STRING_NAME			"AccelSocketd"
#define ACCELSOCKETD_STRING_VERSION		"V01.00.00"

// I2C setup
#define ACCELSOCKETD_I2C_ADAPTER			((char)0x00)
#define ACCELSOCKETD_I2C_ADDRESS			((char)0x00)

// UDP setup
#define ACCELSOCKETD_UDP_LISTEN_PORT	1337
#define ACCELSOCKETD_UDP_ADDRESS			"127.0.0.1"


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
// REG
//****************************************************************************//
  
//****************************************************************************//
// EXPORT
//****************************************************************************//


#endif