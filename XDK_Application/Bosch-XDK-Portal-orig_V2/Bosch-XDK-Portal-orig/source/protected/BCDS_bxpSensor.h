/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	BCDS_bxpSensor.h
**
**	DESCRIPTION:	Public Interface Header for bxpSensor.c source file
**
**	PURPOSE:        Contains the public macro, typedef, variables and function
**	                definitions for the SENSOR module
**
**	AUTHOR(S):      Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**	2015             unknown    BCDS         Initial Release (DMS_demoSensorMonitor)
**  2016.Jan         crk        BCDS         Consolidation/Cleanup of Code
**
*******************************************************************************/

/* header definition ******************************************************** */
#ifndef _BCDS_BXP_SENSOR_H_
#define _BCDS_BXP_SENSOR_H_

/* public interface declaration ********************************************* */
#include "FreeRTOS.h"
#include "timers.h"

/* public type and macro definitions */
#define SENSOR_DATA_BUF_SIZE    1536

typedef struct {
	uint32_t length;
	char data[SENSOR_DATA_BUF_SIZE];
} DataBuffer;

/* public function prototype declarations */

/**
 * @brief The function to get and print the sensors streaming data
 *
 * @param[in] pvParameters Rtos task should be defined with the type void *(as argument)
 */
void Sensor_sendStreamDataTimerHandler(xTimerHandle pvParameters);

/**
 * @brief The function to print the sensors report data
 */
void Sensor_sendReportData(void);

/**
 * @brief The function to print the configuration data
 *
 * @param[in] pvParameters Rtos task should be defined with the type void *(as argument)
 */
void Sensor_sendConfigDataTimerHandler(xTimerHandle pvParameters);

/**
 * @brief Reset the Report Data for all Sensors
 */
void Sensor_resetReportData(void);

/**
 * @brief Init the Sensor module
 */
void Sensor_init(void);

/**
 * @brief Uninit the Sensor module
 */
void Sensor_deinit(void);

/* public global variable declarations */

// Data Buffers
extern DataBuffer g_sensorStreamBuffer;
extern DataBuffer g_sensorReportBuffer;
extern DataBuffer g_sensorConfigBuffer;

/* inline function definitions */

#endif /* _BCDS_BXP_SENSOR_H_ */

/** ************************************************************************* */
