//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : libAccelSocket.h (HEADER)
//----------------------------------------------------------------------------//
// DESC : the lib that connects application to the AccelSocket Daemon.
//----------------------------------------------------------------------------//
// HIST : $Log: libAccelSocket.h,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 24/07/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//

#ifndef __LIBACCELSOCKET_H__
#define __LIBACCELSOCKET_H__

//****************************************************************************//
// INCLIB
//****************************************************************************//
#include <stdint.h>
#include <sys/types.h>

//****************************************************************************//
// INCUSER
//****************************************************************************// 


//****************************************************************************//
// DEFINITION
//****************************************************************************// 
#define LIBACCELSOCKET_MAX_SOCKETNAME_SIZE					64
#define LIBACCELSOCKET_MAX_FRAME_SIZE								16

//****************************************************************************//
// MACRO
//****************************************************************************//
  
//****************************************************************************//
// TYPEDEF
//****************************************************************************//
typedef enum {FALSE = 0, TRUE = 1} elibAccelSocketBool;
typedef char TstLibAccelSocketFrame[LIBACCELSOCKET_MAX_FRAME_SIZE];

typedef struct{
	/** @brief x: x value*/
	uint16_t x;
	/** @brief y: y value*/
	uint16_t y;
	/** @brief z: z value*/
	uint16_t z;
}TstAccel_XYZ;

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
elibAccelSocketBool libAccelSocket_bOpen(char* aps8ClientSocketName);
void								libAccelSocket_vClose(void);

elibAccelSocketBool libAccelSocket_bSetDataRate(uint8_t avu8DataRateConfiguration); // a value of 9 is the normal data rate according to LIS3DH datasheet
elibAccelSocketBool libAccelSocket_bGetDataRate(uint8_t* apu8DataRateConfiguration);
elibAccelSocketBool libAccelSocket_bSetScaleRange(uint8_t avu8ScaleRangeConfiguration);
elibAccelSocketBool libAccelSocket_bGetScaleRange(uint8_t* apu8ScaleRangeConfiguration);
elibAccelSocketBool libAccelSocket_bSetSelftestMode(uint8_t avu8Mode);
/*
elibAccelSocketBool libAccelSocket_bSetInterrupt(TstLibAccelSocketFrame ats8Reply, uint32_t* apu32Size);
elibAccelSocketBool libAccelSocket_bClearInterrupt(TstLibAccelSocketFrame ats8Reply, uint32_t* apu32Size);
*/
elibAccelSocketBool libAccelSocket_bGetXYZ(TstAccel_XYZ* apstAccel);
elibAccelSocketBool libAccelSocket_bReadRegister(uint8_t avu8Register, uint8_t* apu8Value);
elibAccelSocketBool libAccelSocket_bWriteRegister(uint8_t avu8Register, uint8_t avu8Value);

#endif





