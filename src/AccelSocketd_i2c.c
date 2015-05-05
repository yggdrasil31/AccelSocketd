//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : AccelSocketd_i2c.c (SOURCE)
//----------------------------------------------------------------------------//
// DESC : The i2c management source for lis3dh accelerometer.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocketd_i2c.c,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 01/09/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//
  
//****************************************************************************//
// INCLIB
//****************************************************************************//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <syslog.h>
#include <sys/types.h>
#include <fcntl.h>

//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include "libAccelSocket.h"
#include "lis3dh.h"
#include "AccelSocketd_i2c.h"

//****************************************************************************//
// DEFINITION
//****************************************************************************// 
#define LIS3DH_MULTIPLEBYTE_MASK	0x80

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
static int					s_iAccelFd = -1;

//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//
elibAccelSocketBool I2c_bSetSlaveAddress(int avs32Address);
elibAccelSocketBool I2c_bReadPairedRegister(uint8_t avu8Register, uint16_t* apu16Value);
elibAccelSocketBool I2c_bWritePairedRegister(uint8_t avu8Register, uint16_t avu16Value);


elibAccelSocketBool I2c_bOpen(void)
{
	elibAccelSocketBool	lvbResult = FALSE;
	char								lts8TempStr[48] = {0};
	uint8_t							lvu8RegValue = 0;	
	
	
	snprintf(lts8TempStr, 19, "/dev/i2c-%d", ACCELSOCKETD_I2C_ADAPTER);	
	s_iAccelFd = open(lts8TempStr, O_RDWR);
	if (s_iAccelFd < 0)
	{
		// ERROR HANDLING; you can check errno to see what went wrong
		syslog(LOG_INFO, "I2c_bOpen : Can't open I2C device %s",lts8TempStr);		
	}
	else
	{
		if(I2c_bSetSlaveAddress(ACCELSOCKETD_I2C_ADDRESS))
		{
			syslog(LOG_INFO, "I2c_bOpen : I2C slave address set");		
			if (I2c_bReadRegister(LIS3DH_WHO_AM_I,&lvu8RegValue))
			{
				syslog(LOG_INFO, "I2c_bOpen : I2C slave has sent its ID : %d", lvu8RegValue);
				if (lvu8RegValue == 0x33)
				{
					if (	(I2c_bWriteRegister(LIS3DH_CTRL_REG1,0x47))						// 50 Hz, Normal Mode, X/Y/Z enabled
						&&	(I2c_bWriteRegister(LIS3DH_CTRL_REG2,0x00))						// No filtering
						&&	(I2c_bWriteRegister(LIS3DH_CTRL_REG3,0x00))						// IT Disabled;
						&&	(I2c_bWriteRegister(LIS3DH_CTRL_REG4,0x88))						// Blockupdate + HR + Default value (+/- 2G, No selftest)
						&&	(I2c_bWriteRegister(LIS3DH_INT1_CFG,0x00))						// No IT
						&&	(I2c_bReadRegister(LIS3DH_INT1_SRC,&lvu8RegValue))	)	// Clear IT
					{
						lvbResult = TRUE;
					}
					else
					{
						syslog(LOG_INFO, "I2c_bOpen : Cannnot access registers from I2C slave");
					}
				}
				else
				{
					syslog(LOG_INFO, "I2c_bOpen : This is not a LIS3DH");
				}
			}
			else
			{
				syslog(LOG_INFO, "I2c_bOpen : No reply from I2C slave");
			}
		}
	}
	
	syslog(LOG_INFO, "I2c_bOpen : returns %d while opening I2C device %d",lvbResult,s_iAccelFd);
	
	return lvbResult;
}


void I2c_vClose(void)
{
	if (s_iAccelFd>=0)
	{
		(void)I2c_bWriteRegister(LIS3DH_CTRL_REG1,0x00);
		
		syslog(LOG_INFO, "I2c_bClose : closing I2C device %d",s_iAccelFd);
		close(s_iAccelFd);
		s_iAccelFd = -1;		
	}
	else
	{
		syslog(LOG_INFO, "I2c_bClose : I2C device already closed");
	}
}


// set the I2C slave address for all subsequent I2C device transfers
elibAccelSocketBool I2c_bSetSlaveAddress(int avs32Address)
{
	elibAccelSocketBool lvbResult = FALSE;
	
	if (ioctl(s_iAccelFd, I2C_SLAVE, avs32Address) < 0)
	{
		syslog(LOG_INFO, "I2c_SetSlaveAddress : impossible to set the slave address");
	}
	else
	{
		lvbResult = TRUE;
	}
	
	return lvbResult;
}


elibAccelSocketBool I2c_bReadRegister(uint8_t avu8Register, uint8_t* apu8Value)
{
	elibAccelSocketBool	lvbResult = FALSE;
	
	if (write(s_iAccelFd, &avu8Register, 1) == 1)
	{
		if (read(s_iAccelFd, apu8Value, 1) != 1)
		{
			syslog(LOG_INFO, "I2c_ReadRegister : Error while reading");
		}
		else
		{			
			syslog(LOG_INFO, "I2c_ReadRegister : Read 0x%02X in register 0x%02X",*apu8Value, avu8Register);
			lvbResult = TRUE;
		}
	}
	else
	{
		syslog(LOG_INFO, "I2c_ReadRegister : Writing register 0x%02X NOK",avu8Register);
	}
	
	return lvbResult;
}

elibAccelSocketBool I2c_bReadPairedRegister(uint8_t avu8Register, uint16_t* apu16Value)
{
	elibAccelSocketBool	lvbResult = FALSE;
	uint8_t							ltu8Buff[2] = {0};
	
	if (I2c_bReadRegister(avu8Register, &ltu8Buff[0]))
	{
		if (I2c_bReadRegister(avu8Register+1, &ltu8Buff[1]))
		{
			*apu16Value = ((uint16_t)ltu8Buff[1]) << 8;
			*apu16Value |= ltu8Buff[0];
			syslog(LOG_INFO, "I2c_bReadPairedRegister : Read 0x%04X in register 0x%02X",*apu16Value, avu8Register);
			lvbResult = TRUE;
		}
	}
	
	return lvbResult;
}


elibAccelSocketBool I2c_bWriteRegister(uint8_t avu8Register, uint8_t avu8Value)
{
	elibAccelSocketBool	lvbResult = FALSE;
	uint8_t							ltu8Buff[2] = {0};
	
	ltu8Buff[0] = avu8Register;
	ltu8Buff[1] = avu8Value;
	
	if (write(s_iAccelFd, ltu8Buff, 2) == 2)
	{	
		syslog(LOG_INFO, "I2c_WriteRegister : Write 0x%02X in register 0x%02X OK",avu8Value, avu8Register);
		lvbResult = TRUE;
	}
	else
	{
		syslog(LOG_INFO, "I2c_WriteRegister : Write 0x%02X in register 0x%02X NOK",avu8Value, avu8Register);
	}
	
	return lvbResult;
}


elibAccelSocketBool I2c_bWritePairedRegister(uint8_t avu8Register, uint16_t avu16Value)
{
	elibAccelSocketBool	lvbResult = FALSE;
	uint8_t							ltu8Buff[3] = {0};
	
	ltu8Buff[0] = avu8Register;
	ltu8Buff[1] = avu16Value & 0x00FF;
	ltu8Buff[2] = (avu16Value >> 8) & 0x00FF;
	
	if (write(s_iAccelFd, ltu8Buff, 3) == 3)
	{
		syslog(LOG_INFO, "I2c_bWritePairedRegister : Write 0x%04X in register 0x%02X OK",avu16Value, avu8Register);
		lvbResult = TRUE;	
	}
	else
	{
		syslog(LOG_INFO, "I2c_bWritePairedRegister : Write 0x%04X in register 0x%02X NOK",avu16Value, avu8Register);
		
	}
	
	return lvbResult;
}

elibAccelSocketBool I2c_bSetDataRate(uint8_t avu8DataRate)
{

	elibAccelSocketBool	lvbRet = FALSE;
	unsigned char				lvu8RegValue;
	
	if(avu8DataRate <= 9)
	{		
		if (I2c_bReadRegister(LIS3DH_CTRL_REG1,&lvu8RegValue))
		{
			lvu8RegValue &= 0x0F;
			lvu8RegValue |= (((unsigned char)avu8DataRate)<<4);
			if (I2c_bWriteRegister(LIS3DH_CTRL_REG1,lvu8RegValue))
			{
				lvbRet = TRUE;
			}
		}
	}	
	
	return lvbRet;
}


elibAccelSocketBool I2c_bGetDataRate(uint8_t* apu8DataRate)
{
	elibAccelSocketBool	lvbRet = FALSE;
		
	if (I2c_bReadRegister(LIS3DH_CTRL_REG1,apu8DataRate))
	{
		*apu8DataRate = ((*apu8DataRate) & 0xF0)>>4;
		lvbRet = TRUE;
	}
	
	return lvbRet;	
}


elibAccelSocketBool I2c_bSetScaleRange(uint8_t avu8Scale)
{

	elibAccelSocketBool	lvbRet = FALSE;
	unsigned char				lvu8RegValue;
	
	if(avu8Scale <= 3)
	{		
		if (I2c_bReadRegister(LIS3DH_CTRL_REG4,&lvu8RegValue))
		{			
			lvu8RegValue = lvu8RegValue & 0xCF;
			lvu8RegValue |= (avu8Scale<< 4);
			if (I2c_bWriteRegister(LIS3DH_CTRL_REG4,lvu8RegValue))
			{
				lvbRet = TRUE;
			}
		}
	}	
	
	return lvbRet;
}


elibAccelSocketBool I2c_bGetScaleRange(uint8_t* apu8Scale)
{
	elibAccelSocketBool	lvbRet = FALSE;
		
	if (I2c_bReadRegister(LIS3DH_CTRL_REG4,apu8Scale))
	{
		*apu8Scale = (*apu8Scale) & 0xCF;
		*apu8Scale = *apu8Scale >> 4;
		lvbRet = TRUE;
	}
	
	return lvbRet;	
}


elibAccelSocketBool I2c_bSetSelfTestMode(uint8_t avu8Mode)
{

	elibAccelSocketBool	lvbRet = FALSE;
	unsigned char				lvu8RegValue;
	
	if(avu8Mode <= 1)
	{		
		if (I2c_bReadRegister(LIS3DH_CTRL_REG4,&lvu8RegValue))
		{			
			lvu8RegValue = lvu8RegValue & 0xF9;
			lvu8RegValue |= (((unsigned char)avu8Mode)<< 1);
			if (I2c_bWriteRegister(LIS3DH_CTRL_REG4,lvu8RegValue))
			{
				lvbRet = TRUE;
			}
		}
	}	
	
	return lvbRet;
}


elibAccelSocketBool I2c_bGetXYZ(TstAccel_XYZ* apstAccel)
{
	elibAccelSocketBool	lvbRet = FALSE;
	unsigned char 			lvu8Register;
	
	uint8_t							lvu8Val = 0;
	uint16_t						lvu16Val = 0;
	
	if (I2c_bReadRegister(LIS3DH_STATUS_REG,&lvu8Val))
	{
		// new data available ?
		if (lvu8Val & LIS3DH_STATUSREG_XYZDA) 
		{
			if (I2c_bReadPairedRegister(LIS3DH_OUT_X_L,&lvu16Val))
			{
				apstAccel->x = lvu16Val;
				if (I2c_bReadPairedRegister(LIS3DH_OUT_Y_L,&lvu16Val))
				{
					apstAccel->y = lvu16Val;
					if (I2c_bReadPairedRegister(LIS3DH_OUT_Z_L,&lvu16Val))
					{
						apstAccel->z = lvu16Val;
						lvbRet = TRUE;
					}
				}
			}
		}
	}
	
	
	/*
	lvu8Register = LIS3DH_OUT_X_L;
	lvu8Register |= LIS3DH_MULTIPLEBYTE_MASK;
	
	if (write(s_iAccelFd, &lvu8Register, 1) == 1)
	{
		if (read(s_iAccelFd, (char*)apstAccel, 6) != 6)
		{
			syslog(LOG_INFO, "I2c_bGetXYZ : Error while reading");
		}
		else
		{			
			syslog(LOG_INFO, "I2c_bGetXYZ : Read successful %02X %02X %02X %02X %02X %02X", ((char*)apstAccel)[0], ((char*)apstAccel)[1], ((char*)apstAccel)[2], ((char*)apstAccel)[3], ((char*)apstAccel)[4], ((char*)apstAccel)[5] );
			lvbRet = TRUE;
		}
	}
	*/
	
	return lvbRet;
}

