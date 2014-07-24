//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : lis3dh.h (HEADER)
//----------------------------------------------------------------------------//
// DESC : the lis3dh accelerometer header.
//----------------------------------------------------------------------------//
// HIST : $Log: lis3dh.h,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 24/07/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//

#ifndef ACCEL_LIS3DH_H
#define ACCEL_LIS3DH_H

//****************************************************************************//
// DEFINITION
//****************************************************************************//
#define LIS3DH_RESERVED1_L		0x00
#define LIS3DH_RESERVED1_H		0x06
#define LIS3DH_STATUS_REG_AUX	0x07
#define LIS3DH_OUT_ADC1_L		0x08
#define LIS3DH_OUT_ADC1_H		0x09
#define LIS3DH_OUT_ADC2_L		0x0a
#define LIS3DH_OUT_ADC2_H		0x0b
#define LIS3DH_OUT_ADC3_L		0x0c
#define LIS3DH_OUT_ADC3_H		0x0d
#define LIS3DH_INT_COUNTER_REG 	0x0e
#define LIS3DH_WHO_AM_I			0x0f
#define LIS3DH_RESERVED2_L		0x10
#define LIS3DH_RESERVED2_H		0x1e
#define LIS3DH_TEMP_CFG_REG		0x1f
#define LIS3DH_CTRL_REG1		0x20
#define LIS3DH_CTRL_REG2		0x21
#define LIS3DH_CTRL_REG3		0x22
#define LIS3DH_CTRL_REG4		0x23
#define LIS3DH_CTRL_REG5		0x24
#define LIS3DH_CTRL_REG6		0x25
#define LIS3DH_REFERENCE		0x26
#define LIS3DH_STATUS_REG2		0x27
#define LIS3DH_OUT_X_L			0x28
#define LIS3DH_OUT_X_H			0x29
#define LIS3DH_OUT_Y_L			0x2a
#define LIS3DH_OUT_Y_H			0x2b
#define LIS3DH_OUT_Z_L			0x2c
#define LIS3DH_OUT_Z_H			0x2d
#define LIS3DH_FIFO_CTRL_REG	0x2e
#define LIS3DH_FIFO_SRC_REG		0x2f
#define LIS3DH_INT1_CFG			0x30
#define LIS3DH_INT1_SRC			0x31
#define LIS3DH_INT1_THS			0x32
#define LIS3DH_INT1_DURATION	0x33
#define LIS3DH_RESERVED3_L		0x34
#define LIS3DH_RESERVED3_H		0x37
#define LIS3DH_CLICK_CFG		0x38
#define LIS3DH_CLICK_SRC		0x39
#define LIS3DH_CLICK_THS		0x3a
#define LIS3DH_TIME_LIMIT		0x3b
#define LIS3DH_TIME_LATENCY		0x3c
#define LIS3DH_TIME_WINDOW		0x3d
#define LIS3DH_RESERVED4_L		0x3e

#define LIS3DH_STATUSREG_XYZDA	(1<<3)

#define LIS3DH_WHO_AM_I_VAL		0x33

/* CTRL_REG1 definitions */
#define LIS3DH_HPM		(0x03<<6)
#define LIS3DH_HPIS1	(1<<0)

/* CTRL_REG3 definitions */
#define LIS3DH_I1_INT1	(1<<6)

/* INT1_CFG definitions */
#define LIS3DH_AOI	(1<<7)
#define LIS3DH_6D	(1<<6)
#define LIS3DH_ZHIE	(1<<5)
#define LIS3DH_ZLIE	(1<<4)
#define LIS3DH_YHIE	(1<<3)
#define LIS3DH_YLIE	(1<<2)
#define LIS3DH_XHIE	(1<<1)
#define LIS3DH_XLIE	(1<<0)

#define LIS3DH_MAX_DATA_RATE	5000
#define LIS3DH_MAX_FULL_SCALE	16000000
#define LIS3DH_MIN_FULL_SCALE	2000000


//****************************************************************************//
// MACRO
//****************************************************************************//


//****************************************************************************//
// TYPEDEF
//****************************************************************************//





//****************************************************************************//
// EXPORT
//****************************************************************************//


//****************************************************************************//
// PROTO
//****************************************************************************//


#endif // ACCEL_LIS3DH_H
