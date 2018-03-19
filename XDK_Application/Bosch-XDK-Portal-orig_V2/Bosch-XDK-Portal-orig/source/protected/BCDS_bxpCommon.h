/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	BCDS_bxpCommon.h
**
**	DESCRIPTION:	Configuration header for the MQTT Paho Client
**
**	PURPOSE:        Contains the common macro, typedef, variables and function
**	                definitions for the entire project
**
**	AUTHOR(S):      Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**  2016.Apr         crk        BCDS         Initial Release
**
*******************************************************************************/

/* header definition ******************************************************** */
#ifndef _BCDS_BXP_COMMON_H_
#define _BCDS_BXP_COMMON_H_

/* Config interface declaration ********************************************** */

/* Config type and macro definitions */
#define XDK_BXP_REVISION       "0.1.0"              /**< Bosch IoT Suite Revision */

#define NUMBER_UINT8_ZERO		     UINT8_C(0)     /**< Zero value */
#define NUMBER_UINT32_ZERO 		     UINT32_C(0)    /**< Zero value */
#define NUMBER_UINT16_ZERO 		     UINT16_C(0)    /**< Zero value */
#define NUMBER_INT16_ZERO 		     INT16_C(0)     /**< Zero value */

#define POINTER_NULL 			     NULL          /**< ZERO value for pointers */

#define TIMER_AUTORELOAD_ON          1             /**< Auto reload timer */
#define TIMER_AUTORELOAD_OFF         0             /**< One Shot Timer */

#define CMN_ENABLED         1                          /**< enabled */
#define CMN_DISABLED        0                          /**< disabled */

#define CMN_TRUE            1                          /**< true */
#define CMN_FALSE           0                          /**< false */

#define WDG_FREQ            1000                   /**< Watchdog Frequency */
#define WDG_TIMEOUT         200                    /**< Watchdog Timeout */

#define FASTEST_TRANSMISSION_SPEED	 100                /**< The minimum outgoing transmission speed */


/* global function prototype declarations */

/* global variable declarations */

/* global inline function definitions */

#endif /* _BCDS_BXP_COMMON_H_ */

/** ************************************************************************* */
