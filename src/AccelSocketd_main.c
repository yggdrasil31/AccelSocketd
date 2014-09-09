//############################################################################//
// HEAD : (c) ACTIA 2014 : " Any copie and re-utilisation of this file without
//                           the written consent of ACTIA is forbidden "
//        --------------------------------------------------------------------
//        Classification :  (-)No   (X)Confident. ACTIA   (X)Confident. Client
//        --------------------------------------------------------------------
//        AccelSocketd : Module Name
//##########################7#################################################//
// FILE : AccelSocketd_main.c (SOURCE)
//----------------------------------------------------------------------------//
// DESC : the AccelSocketd daemon source code.
//----------------------------------------------------------------------------//
// HIST : $Log: AccelSocketd.c,v $
// HIST : Version |   Date   | Author | Description                           
//        --------------------------------------------------------------------
//         01.00  | 24/07/14 |  JTou  | Initial version
//                |          |        |
//############################################################################//
  
//****************************************************************************//
// INCLIB
//****************************************************************************//
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>
#include <sched.h>


//****************************************************************************//
// INCUSER
//****************************************************************************// 
#include "AccelSocketd.h"
#include "AccelSocketd_server.h"
#include "AccelSocketd_i2c.h"

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
static volatile int	s_iExitFlag = 0;	// see http://www.linuxprogrammingblog.com/all-about-linux-signals?page=show


//****************************************************************************//
// REG
//****************************************************************************//
  
//****************************************************************************//
// PROTO
//****************************************************************************//
static void s_vProcess(void);
static void s_vHandleSig(int sig);



//============================================================================//
// NAME : accel ioctl
//============================================================================//
// ROLE :
//----------------------------------------------------------------------------//
// INPUT :
//
// OUTPUT :
//
// RETURN:
//----------------------------------------------------------------------------//
//============================================================================//
/*
int accel_lis3dh_Ioctl(int req, void * argp)
{


	if(stAccel_lis3dh_Infos.iIsOpen == true)
	{

		switch(req)
		{
			case ACCEL_SET_DATARATE:
				return(accel_lis3dh_SetDataRate((TAccel_DataRate)argp));
			case ACCEL_GET_DATARATE:
				return(accel_lis3dh_GetDataRate((TAccel_DataRate*)argp));
			case ACCEL_SET_SCALERANGE:
				return(accel_lis3dh_SetScaleRange((TAccel_Accel)argp));
			case ACCEL_GET_SCALERANGE:
				return(accel_lis3dh_GetScaleRange((TAccel_Accel*)argp));
#if ( defined(LIS3DH_INT1_OPEN) || defined(LIS3DH_INT2_OPEN) )
			case ACCEL_SET_INTERRUPT:
				return(accel_lis3dh_SetInterrupt((TpfcAccel_Callback)argp));
			case ACCEL_CLR_INTERRUPT:
				return(accel_lis3dh_ClrInterrupt());
#endif	//#if ( defined(LIS3DH_INT1_OPEN) || defined(LIS3DH_INT2_OPEN) )
			case ACCEL_GET_XYZ:
				return(accel_lis3dh_GetXYZ((TstAccel_XYZ*)argp));
			case ACCEL_READ_REGISTER:
				return(accel_lis3dh_ReadReg((TstAccel_RegAccess*)argp));
			case ACCEL_WRITE_REGISTER:
				return(accel_lis3dh_WriteReg((TstAccel_RegAccess*)argp));
#ifdef LIS3DH_INT1_OPEN
			case ACCEL_ENABLE_MOTION_DETECT_INT1:
				return accel_lis3dh_EnableMotionDetect((TstAccel_MotionDetectParam *)argp);
			case ACCEL_DISABLE_MOTION_DETECT_INT1:
				return accel_lis3dh_DisableMotionDetect();
			case ACCEL_ENABLE_DRDY_INT1:
				return accel_lis3dh_EnableDrdyIt();
			case ACCEL_DISABLE_DRDY_INT1:
				return accel_lis3dh_DisableDrdyIt();
#endif	//#ifdef LIS3DH_INT1_OPEN
			default:
			break;
		}
	}
	else
		return(LOWLEVEL_ERROR_ACCEL_DRIVERNOTOPEN);
		
	return(LOWLEVEL_NO_ERROR);
}
*/


static void s_vHandleSig (int sig)
{
	s_iExitFlag = 1;
}


static void s_vProcess(void)
{
	syslog (LOG_NOTICE, "... still running ...");
	vServer_processListen();	
}


int main(int argc, char **argv)
{
	pid_t							pid, sid;
	struct sigaction	stAct;
	char							tempStr[48] = {0};
	
	// Skeletton for daemon code from http://shahmirj.com/blog/beginners-guide-to-creating-a-daemon-in-linux
		
	// Set our Logging Mask and open the Log
	//setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog(ACCELSOCKETD_STRING_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

	syslog(LOG_INFO, "Starting %s %s",ACCELSOCKETD_STRING_NAME,ACCELSOCKETD_STRING_VERSION);

	// Fork the Parent Process
	pid = fork();
	if (pid < 0)
	{
		exit(EXIT_FAILURE);
	}

	// We got a good pid, Close the Parent Process
	if (pid > 0)
	{
		exit(EXIT_SUCCESS);
	}

	// Change File Mask
	umask(0);

	// Create a new Signature Id for our child
	sid = setsid();
	if (sid < 0)
	{
		exit(EXIT_FAILURE);
	}

	
	// Change Directory
	// If we cant find the directory we exit with failure.
	if ((chdir("/")) < 0)
	{
		exit(EXIT_FAILURE);
	}

	// Close Standard File Descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Daemon-specific initialization goes here
	
	// Install signal handler
	memset (&stAct, '\0', sizeof(stAct));
	stAct.sa_handler = &s_vHandleSig;
	if (sigaction(SIGTERM, &stAct, NULL) < 0)
	{
		syslog(LOG_INFO, "Can't install signal handler %s",tempStr);
		exit(EXIT_FAILURE);
	}
		
	// Open I2C-dev
	if (!I2c_bOpen())
	{
		I2c_vClose();
		exit(EXIT_FAILURE);
	}
	
	// Init the local namespaced socket server
	if (!bServer_init())
	{
		I2c_vClose();
		vServer_terminate();
		exit(EXIT_FAILURE);
	}
	
	syslog(LOG_INFO, "Successfully started");
	
	//----------------
	// Main Process
	//----------------
	while(!s_iExitFlag)
	{
		s_vProcess();		// Run our Process
		sched_yield();
	}
	
	// Terminate server
	vServer_terminate();

	// Close I2C-dev
	I2c_vClose();
	
	// Close the log
	syslog(LOG_INFO, "Terminated");
	closelog();
	
	return 0;
}

