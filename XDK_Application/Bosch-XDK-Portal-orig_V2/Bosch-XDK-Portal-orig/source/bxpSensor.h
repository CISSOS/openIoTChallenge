/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	bxpSensor.h
**
**	DESCRIPTION:	Local Header for SENSOR_send_cc.c source file
**
**	PURPOSE:        Contains the local macro, typedef, variables and function
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
#ifndef _BXP_SENSOR_H_
#define _BXP_SENSOR_H_

/* local interface declaration ********************************************** */

/* local type and macro definitions */
#define SENSOR_ROUNDING_VAR                        0.5
#define SENSOR_INIT_ONE                            1

// Report Data Structure
typedef struct {
	int32_t CurrentData;
	int32_t Avg;
	int32_t Max;
	int32_t Min;
	int32_t Sum;
} ReportData;

/* local module variable declarations */

// Data Variables
static uint32_t              s_sensorRunAvgCounter             = NUMBER_UINT32_ZERO;       /**< Running Average Counter */
static const ReportData      ResetDataBuf                   = { 0, 0, 0, 0, 0 };
static ReportData            s_sensorAccelDataX;
static ReportData            s_sensorAccelDataY;
static ReportData            s_sensorAccelDataZ;
static ReportData            s_sensorGyroDataX;
static ReportData            s_sensorGyroDataY;
static ReportData            s_sensorGyroDataZ;
static ReportData            s_sensorMagDataX;
static ReportData            s_sensorMagDataY;
static ReportData            s_sensorMagDataZ;
static ReportData            s_sensorLightData;
static ReportData            s_sensorTempData;
static ReportData            s_sensorPressureData;
static ReportData            s_sensorHumidityData;

/* local module function declarations */

/**
 * @brief Inititialize the accelerometer
 */
static void sensorInitializeAccel(void);

/**
 * @brief Inititialize the gyroscope
 */
static void sensorInitializeGyro(void);

/**
 * @brief Inititialize the magnetometer
 */
static void sensorInitializeMag(void);

/**
 * @brief Inititialize the ambient light sensor
 */
static void sensorInitializeLight(void);

/**
 * @brief Inititialize the environmental sensor
 */
static void sensorInitializeEnv(void);

/**
 * @brief The function does the post initialisation of the light sensor
 */
static Retcode_T sensorPostInitLightSensor(void);

/**
 * @brief The function does the post initialisation of the magnetometer
 */
static Retcode_T sensorPostInitMagSensor(void);

/**
 * @Brief Setup the data in the data structure to report the
 *        Average, Max and Min values of the Accelerometer
 *
 * @param[in] accelData
 *            Current Data Read from the accelerometer
 */
static void sensorAvgAccel(Accelerometer_XyzData_T accelData);

/**
 * @Brief Setup the data in the data structure to report the
 *        Average, Max and Min values of the Gyroscope
 *
 * @param[in] gyroData
 *            Current Data Read from the gyroscope
 */
static void sensorAvgGyro(Gyroscope_XyzData_T gyroData);

/**
 * @Brief Setup the data in the data structure to report the
 *        Average, Max and Min values of the Magnetometer
 *
 * @param[in] magData
 *            Current Data Read from the magnetometer
 */
static void sensorAvgMag(Magnetometer_XyzData_T magData);

/**
 * @Brief Setup the data in the data structure to report the
 *        Average, Max and Min values of the ambient light
 *
 * @param[in] lgtData
 *            Current Data Read from the ambient light sensor
 */
static void sensorAvgLight(uint32_t lgtData);

/**
 * @Brief Setup the data in the data structure to report the
 *        Average, Max and Min values of the Temperature,
 *        Humidity and Pressure
 *
 * @param[in] envData
 *            Current Data Read from the environmental sensor
 */
static void sensorAvgEnv(Environmental_Data_T envData);

/* local inline function definitions */

#endif /* _BXP_SENSOR_H_ */

/** ************************************************************************* */
