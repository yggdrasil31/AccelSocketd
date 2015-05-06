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
#define LIBACCELSOCKET_VERSION "V01.00.02"


//****************************************************************************//
// MACRO
//****************************************************************************//


//****************************************************************************//
// TYPEDEF
//****************************************************************************//
typedef enum {FALSE = 0, TRUE = 1} elibAccelSocketBool;


typedef enum
{
	LIBACCELSOCKET_SCALE_PLUSORMINUS2G = 0,
	LIBACCELSOCKET_SCALE_PLUSORMINUS4G,
	LIBACCELSOCKET_SCALE_PLUSORMINUS8G,
	LIBACCELSOCKET_SCALE_PLUSORMINUS16G
}	elibAccelSocketScale;


typedef enum
{
	LIBACCELSOCKET_RATE_POWERDOWN = 0,
	LIBACCELSOCKET_RATE_1HZ,
	LIBACCELSOCKET_RATE_10HZ,
	LIBACCELSOCKET_RATE_25HZ,
	LIBACCELSOCKET_RATE_50HZ,
	LIBACCELSOCKET_RATE_100HZ,
	LIBACCELSOCKET_RATE_200HZ,
	LIBACCELSOCKET_RATE_400HZ,
	LIBACCELSOCKET_RATE_LOW1600HZ,
	LIBACCELSOCKET_RATE_NORMAL1250HZ_LOW5000HZ
}	elibAccelSocketRate;


typedef enum
{
	LIBACCELSOCKET_SELFTESTMODE_OFF = 0,
	LIBACCELSOCKET_SELFTESTMODE_0,
	LIBACCELSOCKET_SELFTESTMODE_1
}	elibAccelSocketSelfTestMode;

typedef struct
{
	/** @brief x: x acceleration value*/
	uint16_t x;
	/** @brief y: y acceleration value*/
	uint16_t y;
	/** @brief z: z acceleration value*/
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

char*								libAccelSocket_ps8GetVersion(void);

elibAccelSocketBool libAccelSocket_bSetDataRate(elibAccelSocketRate avu32DataRateConfiguration);
elibAccelSocketBool libAccelSocket_bGetDataRate(elibAccelSocketRate* apu32DataRateConfiguration);
elibAccelSocketBool libAccelSocket_bSetScaleRange(elibAccelSocketScale avu32ScaleRangeConfiguration);
elibAccelSocketBool libAccelSocket_bGetScaleRange(elibAccelSocketScale* apu32ScaleRangeConfiguration);
elibAccelSocketBool libAccelSocket_bSetSelftestMode(elibAccelSocketSelfTestMode avu8Mode);
elibAccelSocketBool libAccelSocket_bGetXYZ(TstAccel_XYZ* apstAccel);
elibAccelSocketBool libAccelSocket_bReadRegister(uint8_t avu8Register, uint8_t* apu8Value);
elibAccelSocketBool libAccelSocket_bWriteRegister(uint8_t avu8Register, uint8_t avu8Value);

#endif





