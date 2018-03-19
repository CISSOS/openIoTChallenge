/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	bxpSensor.c
**
**	DESCRIPTION:	Manages the Sensor Data
**
**	PURPOSE:        Initializes the sensors and data buffers,
**					reads the sensor data, caclulates the report data,
**					and writes the data into a JSON formatted buffer
**
**	AUTHOR(S):      Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**	2015             unknown    BCDS         Initial Release (DMS_demoSensorMonitor)
**  2015.Oct         crk        BCDS         Edited to work only with wifi, to send
**                                           JSON formated data stream, updated the buffer
**                                           structure and removed unessary functions
**  2016.Jan         crk        BCDS         Added Report (Averaging) stream,
**                                           Configuration stream, and clean up the code
**
*******************************************************************************/

/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* own header files */
#include "BCDS_bxpSensor.h"
#include "BCDS_bxpCommon.h"
#include "BCDS_bxpConfig.h"

/* interface header files */
#include "BCDS_PowerMgt.h"
#include "XdkSensorHandle.h"
#include "BCDS_Retcode.h"

/* local header files */
#include "bxpSensor.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/* global variables ********************************************************* */

/* Data Buffers */
DataBuffer g_sensorStreamBuffer;
DataBuffer g_sensorReportBuffer;
DataBuffer g_sensorConfigBuffer;

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorInitializeAccel(void)
{
	Retcode_T _accelReturnValue_BMA280_S = RETCODE_FAILURE;

	_accelReturnValue_BMA280_S = Accelerometer_init(xdkAccelerometers_BMA280_Handle);
	if(RETCODE_SUCCESS != _accelReturnValue_BMA280_S) {
		printf("accelerometerInit BMA280 failed = %ld \r\n",_accelReturnValue_BMA280_S);
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorInitializeGyro(void)
{
	Retcode_T _gyroG160ReturnValue = RETCODE_FAILURE;

	_gyroG160ReturnValue = Gyroscope_init(xdkGyroscope_BMG160_Handle);
	if(RETCODE_SUCCESS != _gyroG160ReturnValue) {
		printf("gyroscopeInit BMG160 failed = %ld \r\n",_gyroG160ReturnValue);
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorInitializeMag(void)
{
	Retcode_T _magnetoReturnValue = RETCODE_FAILURE;

	_magnetoReturnValue = Magnetometer_init(xdkMagnetometer_BMM150_Handle);
	if(RETCODE_SUCCESS != _magnetoReturnValue) {
		printf("magnetometerInit BMM150 failed = %ld \r\n",_magnetoReturnValue);
	}

	if(RETCODE_SUCCESS == _magnetoReturnValue  ) {
		_magnetoReturnValue = sensorPostInitMagSensor();
	}

	if(RETCODE_SUCCESS != _magnetoReturnValue) {
		printf("sensorPostInitMagSensor BMM150 failed = %ld \r\n",_magnetoReturnValue);
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorInitializeLight(void)
{
	Retcode_T _lgtReturnValue = RETCODE_FAILURE;

	_lgtReturnValue = LightSensor_init(xdkLightSensor_MAX44009_Handle);
	if(RETCODE_SUCCESS != _lgtReturnValue) {
		printf("lightsensorInit MAX09 failed = %ld \r\n",_lgtReturnValue);
	}

	if(RETCODE_SUCCESS == _lgtReturnValue) {
		_lgtReturnValue = sensorPostInitLightSensor();
	}

	if(RETCODE_SUCCESS != _lgtReturnValue) {
		printf("sensorPostInitLightSensor MAX09 failed = %ld \r\n",_lgtReturnValue);
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorInitializeEnv(void)
{
	Retcode_T _envReturnValue = RETCODE_FAILURE;

	_envReturnValue = Environmental_init(xdkEnvironmental_BME280_Handle);
	if(RETCODE_SUCCESS != _envReturnValue) {
		printf("environmentalInit BME280 failed = %ld \r\n",_envReturnValue);
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static Retcode_T sensorPostInitLightSensor(void)
{
	Retcode_T _res = RETCODE_FAILURE;

    /** Manual Mode should be enable in order to configure the continuous mode, brightness  and integration time*/
	_res = LightSensor_setManualMode(xdkLightSensor_MAX44009_Handle, LIGHTSENSOR_MAX44009_ENABLE_MODE);

    return _res;
}

/**
 * @brief in DTA_send_ch.h header
 */
static Retcode_T sensorPostInitMagSensor(void)
{
	Retcode_T _res = RETCODE_FAILURE;
	_res = Magnetometer_setPowerMode(xdkMagnetometer_BMM150_Handle,
			MAGNETOMETER_BMM150_POWERMODE_NORMAL);
	return _res;
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorAvgAccel(Accelerometer_XyzData_T accelData)
{
	/**** Insert Read Data into Data structure ****/
	s_sensorAccelDataX.CurrentData = accelData.xAxisData;
	s_sensorAccelDataY.CurrentData = accelData.yAxisData;
	s_sensorAccelDataZ.CurrentData = accelData.zAxisData;

	/**** Calculate the total sum of all data in the running average set ****/
	s_sensorAccelDataX.Sum += s_sensorAccelDataX.CurrentData;
	s_sensorAccelDataY.Sum += s_sensorAccelDataY.CurrentData;
	s_sensorAccelDataZ.Sum += s_sensorAccelDataZ.CurrentData;

	/**** Calc Avg Values ****/
	s_sensorAccelDataX.Avg = (((float)s_sensorAccelDataX.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorAccelDataY.Avg = (((float)s_sensorAccelDataY.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorAccelDataZ.Avg = (((float)s_sensorAccelDataZ.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);

	/**** Set the initial Max and Min Values ****/
	if (s_sensorRunAvgCounter == 1) {
		s_sensorAccelDataX.Max = s_sensorAccelDataX.CurrentData;
		s_sensorAccelDataX.Min = s_sensorAccelDataX.CurrentData;
		s_sensorAccelDataY.Max = s_sensorAccelDataY.CurrentData;
		s_sensorAccelDataY.Min = s_sensorAccelDataY.CurrentData;
		s_sensorAccelDataZ.Max = s_sensorAccelDataZ.CurrentData;
		s_sensorAccelDataZ.Min = s_sensorAccelDataZ.CurrentData;
	}

	else {
	    /**** Calc Max Values ****/
	    if (s_sensorAccelDataX.CurrentData > s_sensorAccelDataX.Max) {
	    	s_sensorAccelDataX.Max = s_sensorAccelDataX.CurrentData;
	    }
	    if (s_sensorAccelDataY.CurrentData > s_sensorAccelDataY.Max) {
	    	s_sensorAccelDataY.Max = s_sensorAccelDataY.CurrentData;
	    }
	    if (s_sensorAccelDataZ.CurrentData > s_sensorAccelDataZ.Max) {
	    	s_sensorAccelDataZ.Max = s_sensorAccelDataZ.CurrentData;
	    }

	    /**** Calc Min Values ****/
	    if (s_sensorAccelDataX.CurrentData < s_sensorAccelDataX.Min) {
			s_sensorAccelDataX.Min = s_sensorAccelDataX.CurrentData;
		}
		if (s_sensorAccelDataY.CurrentData < s_sensorAccelDataY.Min) {
			s_sensorAccelDataY.Min = s_sensorAccelDataY.CurrentData;
		}
		if (s_sensorAccelDataZ.CurrentData < s_sensorAccelDataZ.Min) {
			s_sensorAccelDataZ.Min = s_sensorAccelDataZ.CurrentData;
		}
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorAvgGyro(Gyroscope_XyzData_T gyroData)
{
	/**** Insert Read Data into Data structure ****/
	s_sensorGyroDataX.CurrentData = gyroData.xAxisData;
	s_sensorGyroDataY.CurrentData = gyroData.yAxisData;
	s_sensorGyroDataZ.CurrentData = gyroData.zAxisData;

	/**** Calculate the total sum of all data in the running average set ****/
	s_sensorGyroDataX.Sum += s_sensorGyroDataX.CurrentData;
	s_sensorGyroDataY.Sum += s_sensorGyroDataY.CurrentData;
	s_sensorGyroDataZ.Sum += s_sensorGyroDataZ.CurrentData;

	/**** Calc Avg Values ****/
	s_sensorGyroDataX.Avg = (((float)s_sensorGyroDataX.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorGyroDataY.Avg = (((float)s_sensorGyroDataY.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorGyroDataZ.Avg = (((float)s_sensorGyroDataZ.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);

	/**** Set the initial Max and Min values ****/
	if (s_sensorRunAvgCounter == 1) {
		s_sensorGyroDataX.Max = s_sensorGyroDataX.CurrentData;
		s_sensorGyroDataX.Min = s_sensorGyroDataX.CurrentData;
		s_sensorGyroDataY.Max = s_sensorGyroDataY.CurrentData;
		s_sensorGyroDataY.Min = s_sensorGyroDataY.CurrentData;
		s_sensorGyroDataZ.Max = s_sensorGyroDataZ.CurrentData;
		s_sensorGyroDataZ.Min = s_sensorGyroDataZ.CurrentData;
	}

	else {
	    /**** Calc Max Values ****/
	    if (s_sensorGyroDataX.CurrentData > s_sensorGyroDataX.Max) {
	    	s_sensorGyroDataX.Max = s_sensorGyroDataX.CurrentData;
	    }
	    if (s_sensorGyroDataY.CurrentData > s_sensorGyroDataY.Max) {
	    	s_sensorGyroDataY.Max = s_sensorGyroDataY.CurrentData;
	    }
	    if (s_sensorGyroDataZ.CurrentData > s_sensorGyroDataZ.Max) {
	    	s_sensorGyroDataZ.Max = s_sensorGyroDataZ.CurrentData;
	    }

	    /**** Calc Min Values ****/
	    if (s_sensorGyroDataX.CurrentData < s_sensorGyroDataX.Min) {
			s_sensorGyroDataX.Min = s_sensorGyroDataX.CurrentData;
		}
		if (s_sensorGyroDataY.CurrentData < s_sensorGyroDataY.Min) {
			s_sensorGyroDataY.Min = s_sensorGyroDataY.CurrentData;
		}
		if (s_sensorGyroDataZ.CurrentData < s_sensorGyroDataZ.Min) {
			s_sensorGyroDataZ.Min = s_sensorGyroDataZ.CurrentData;
		}
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorAvgMag(Magnetometer_XyzData_T magData)
{
	/**** Insert Read Data into Data Structure ****/
	s_sensorMagDataX.CurrentData = magData.xAxisData;
	s_sensorMagDataY.CurrentData = magData.yAxisData;
	s_sensorMagDataZ.CurrentData = magData.zAxisData;

	/**** Calculate the total sum of all data in the running average set ****/
	s_sensorMagDataX.Sum += s_sensorMagDataX.CurrentData;
	s_sensorMagDataY.Sum += s_sensorMagDataY.CurrentData;
	s_sensorMagDataZ.Sum += s_sensorMagDataZ.CurrentData;

	/**** Calc Avg Values ****/
	s_sensorMagDataX.Avg = (((float)s_sensorMagDataX.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorMagDataY.Avg = (((float)s_sensorMagDataY.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorMagDataZ.Avg = (((float)s_sensorMagDataZ.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);

	/**** Set the initial Max and Min Values ****/
	if (s_sensorRunAvgCounter == 1) {
		s_sensorMagDataX.Max = s_sensorMagDataX.CurrentData;
		s_sensorMagDataX.Min = s_sensorMagDataX.CurrentData;
		s_sensorMagDataY.Max = s_sensorMagDataY.CurrentData;
		s_sensorMagDataY.Min = s_sensorMagDataY.CurrentData;
		s_sensorMagDataZ.Max = s_sensorMagDataZ.CurrentData;
		s_sensorMagDataZ.Min = s_sensorMagDataZ.CurrentData;
	}

	else {
	    /**** Calc Max Values ****/
	    if (s_sensorMagDataX.CurrentData > s_sensorMagDataX.Max) {
	    	s_sensorMagDataX.Max = s_sensorMagDataX.CurrentData;
	    }
	    if (s_sensorMagDataY.CurrentData > s_sensorMagDataY.Max) {
	    	s_sensorMagDataY.Max = s_sensorMagDataY.CurrentData;
	    }
	    if (s_sensorMagDataZ.CurrentData > s_sensorMagDataZ.Max) {
	    	s_sensorMagDataZ.Max = s_sensorMagDataZ.CurrentData;
	    }

	    /**** Calc Min Values ****/
	    if (s_sensorMagDataX.CurrentData < s_sensorMagDataX.Min) {
			s_sensorMagDataX.Min = s_sensorMagDataX.CurrentData;
		}
		if (s_sensorMagDataY.CurrentData < s_sensorMagDataY.Min) {
			s_sensorMagDataY.Min = s_sensorMagDataY.CurrentData;
		}
		if (s_sensorMagDataZ.CurrentData < s_sensorMagDataZ.Min) {
			s_sensorMagDataZ.Min = s_sensorMagDataZ.CurrentData;
		}
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorAvgLight(uint32_t lgtData)
{
	/**** Insert Read Data into Data Structure ****/
	s_sensorLightData.CurrentData = lgtData;

	/**** Calculate the total sum of all data in the running average set ****/
	s_sensorLightData.Sum += s_sensorLightData.CurrentData;

	/**** Calc Avg Values ****/
	s_sensorLightData.Avg = (((float)s_sensorLightData.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);

	/**** Set the initial Max and Min Values ****/
	if (s_sensorRunAvgCounter == 1) {
		s_sensorLightData.Max = s_sensorLightData.CurrentData;
		s_sensorLightData.Min = s_sensorLightData.CurrentData;
	}

	else {
	    /**** Calc Max Values ****/
	    if (s_sensorLightData.CurrentData > s_sensorLightData.Max) {
	    	s_sensorLightData.Max = s_sensorLightData.CurrentData;
	    }

	    /**** Calc Min Values ****/
	    if (s_sensorLightData.CurrentData < s_sensorLightData.Min) {
	    	s_sensorLightData.Min = s_sensorLightData.CurrentData;
		}
	}
}

/**
 * @brief in DTA_send_ch.h header
 */
static void sensorAvgEnv(Environmental_Data_T envData)
{
	/**** Insert Read Data into Data Stucture ****/
	s_sensorTempData.CurrentData = envData.temperature;
	s_sensorPressureData.CurrentData = envData.pressure;
	s_sensorHumidityData.CurrentData = envData.humidity;

	/**** Calculate the total sum of all data in the running average set ****/
	s_sensorTempData.Sum += s_sensorTempData.CurrentData;
	s_sensorPressureData.Sum += s_sensorPressureData.CurrentData;
	s_sensorHumidityData.Sum += s_sensorHumidityData.CurrentData;

	/**** Calc Avg Values ****/
	s_sensorTempData.Avg = (((float)s_sensorTempData.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorPressureData.Avg = (((float)s_sensorPressureData.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);
	s_sensorHumidityData.Avg = (((float)s_sensorHumidityData.Sum / (int32_t) s_sensorRunAvgCounter) + SENSOR_ROUNDING_VAR);

	/**** Set the initial Max and Min Values ****/
	if (s_sensorRunAvgCounter == 1) {
		s_sensorTempData.Max = s_sensorTempData.CurrentData;
		s_sensorTempData.Min = s_sensorTempData.CurrentData;
		s_sensorPressureData.Max = s_sensorPressureData.CurrentData;
		s_sensorPressureData.Min = s_sensorPressureData.CurrentData;
		s_sensorHumidityData.Max = s_sensorHumidityData.CurrentData;
		s_sensorHumidityData.Min = s_sensorHumidityData.CurrentData;
	}

	else {
	    /**** Calc Max Values ****/
	    if (s_sensorTempData.CurrentData > s_sensorTempData.Max) {
	    	s_sensorTempData.Max = s_sensorTempData.CurrentData;
	    }
	    if (s_sensorPressureData.CurrentData > s_sensorPressureData.Max) {
	    	s_sensorPressureData.Max = s_sensorPressureData.CurrentData;
	    }
	    if (s_sensorHumidityData.CurrentData > s_sensorHumidityData.Max) {
	    	s_sensorHumidityData.Max = s_sensorHumidityData.CurrentData;
	    }

	    /**** Calc Min Values ****/
	    if (s_sensorTempData.CurrentData < s_sensorTempData.Min) {
	    	s_sensorTempData.Min = s_sensorTempData.CurrentData;
		}
		if (s_sensorPressureData.CurrentData < s_sensorPressureData.Min) {
			s_sensorPressureData.Min = s_sensorPressureData.CurrentData;
		}
		if (s_sensorHumidityData.CurrentData < s_sensorHumidityData.Min) {
			s_sensorHumidityData.Min = s_sensorHumidityData.CurrentData;
		}
	}
}

/* global functions ********************************************************* */

/**
 * @brief in DTA_send_ih.h header
 */
void Sensor_sendStreamDataTimerHandler(xTimerHandle pvParameters)
{
	/**** Initialize Variables ****/
	(void) pvParameters;
	Accelerometer_XyzData_T 	   _accelBMA280Data  = {NUMBER_UINT16_ZERO,NUMBER_UINT16_ZERO,NUMBER_UINT16_ZERO};
	Gyroscope_XyzData_T     	   _gyroBMG160Data	 = {NUMBER_UINT32_ZERO,NUMBER_UINT32_ZERO,NUMBER_UINT32_ZERO};
	Magnetometer_XyzData_T         _magData    	     = {NUMBER_UINT16_ZERO,NUMBER_UINT16_ZERO,NUMBER_UINT16_ZERO,NUMBER_UINT16_ZERO};
	Environmental_Data_T		   _envData		     = {NUMBER_UINT32_ZERO,NUMBER_UINT32_ZERO,NUMBER_UINT32_ZERO};
	uint32_t			           _lgtData		     = {NUMBER_UINT32_ZERO};

	/**** Read the Sensors ****/
	Accelerometer_readXyzGValue    (xdkAccelerometers_BMA280_Handle     , &_accelBMA280Data);
	Gyroscope_readXyzDegreeValue   (xdkGyroscope_BMG160_Handle 	    	, &_gyroBMG160Data);
	Environmental_readData         (xdkEnvironmental_BME280_Handle      , &_envData);
	LightSensor_readLuxData        (xdkLightSensor_MAX44009_Handle  	, &_lgtData);
	Magnetometer_readXyzTeslaData  (xdkMagnetometer_BMM150_Handle       , &_magData);

	/**** Average the Sensor Data ****/
	sensorAvgAccel(_accelBMA280Data);
	sensorAvgGyro(_gyroBMG160Data);
	sensorAvgMag(_magData);
	sensorAvgLight(_lgtData);
	sensorAvgEnv(_envData);

	/**** Increment the Running Avgerage Counter ****/
	s_sensorRunAvgCounter += 1;

	/****** da qui *********/
	/**** Write the sensor data into the Stream Buffer in JSON Format ****/
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "{\n");
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"metrics\": {\n");

	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"acc_x\": \"%ld\",\n", _accelBMA280Data.xAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"acc_y\": \"%ld\",\n", _accelBMA280Data.yAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"acc_z\": \"%ld\",\n", _accelBMA280Data.zAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"gyro_x\": \"%ld\",\n", _gyroBMG160Data.xAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"gyro_y\": \"%ld\",\n", _gyroBMG160Data.yAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"gyro_z\": \"%ld\",\n", _gyroBMG160Data.zAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"mag_x\": \"%ld\",\n", _magData.xAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"mag_y\": \"%ld\",\n", _magData.yAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"mag_z\": \"%ld\",\n", _magData.zAxisData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"light\": \"%ld\",\n", _lgtData);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"temperature\": \"%ld\",\n", _envData.temperature);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"pressure\": \"%ld\",\n", _envData.pressure);
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"humidity\": \"%ld\"\n", _envData.humidity);

	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "}}\n");
	/****** fino qui ******/
	//g_sensorStreamBuffer.length -= 2;

	/**** Send the Report Data if the Correct Number of Samples have been taken ****/
	if (s_sensorRunAvgCounter == ((uint32_t) CFG_getReportSamples())) {
		Sensor_sendReportData();
	}
}

/**
 * @brief in DTA_send_ih.h header
 */
void Sensor_sendReportData(void)
{
	/**** Write the report data into the Report Buffer in JSON Format ****/
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "{\n");
	g_sensorStreamBuffer.length += sprintf(g_sensorStreamBuffer.data + g_sensorStreamBuffer.length, "\"metrics\": {\n");
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_x_min\": \"%ld\",\n", s_sensorAccelDataX.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_x_max\": \"%ld\",\n", s_sensorAccelDataX.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_x_avg\": \"%ld\",\n", s_sensorAccelDataX.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_y_min\": \"%ld\",\n", s_sensorAccelDataY.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_y_max\": \"%ld\",\n", s_sensorAccelDataY.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_x_avg\": \"%ld\",\n", s_sensorAccelDataY.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_z_min\": \"%ld\",\n", s_sensorAccelDataZ.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_z_max\": \"%ld\",\n", s_sensorAccelDataZ.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"acc_z_avg\": \"%ld\",\n", s_sensorAccelDataZ.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_x_min\": \"%ld\",\n", s_sensorGyroDataX.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_x_max\": \"%ld\",\n", s_sensorGyroDataX.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_x_avg\": \"%ld\",\n", s_sensorGyroDataX.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_y_min\": \"%ld\",\n", s_sensorGyroDataY.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_y_max\": \"%ld\",\n", s_sensorGyroDataY.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_y_avg\": \"%ld\",\n", s_sensorGyroDataY.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_z_min\": \"%ld\",\n", s_sensorGyroDataZ.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_z_max\": \"%ld\",\n", s_sensorGyroDataZ.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"gyro_z_avg\": \"%ld\",\n", s_sensorGyroDataZ.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_x_min\": \"%ld\",\n", s_sensorMagDataX.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_x_max\": \"%ld\",\n", s_sensorMagDataX.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_x_avg\": \"%ld\",\n", s_sensorMagDataX.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_y_min\": \"%ld\",\n", s_sensorMagDataY.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_y_max\": \"%ld\",\n", s_sensorMagDataY.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_y_avg\": \"%ld\",\n", s_sensorMagDataY.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_z_min\": \"%ld\",\n", s_sensorMagDataZ.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_z_max\": \"%ld\",\n", s_sensorMagDataZ.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"mag_z_avg\": \"%ld\",\n", s_sensorMagDataZ.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"light_min\": \"%ld\",\n", s_sensorLightData.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"light_max\": \"%ld\",\n", s_sensorLightData.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"light_avg\": \"%ld\",\n", s_sensorLightData.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"temp_min\": \"%ld\",\n", s_sensorTempData.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"temp_max\": \"%ld\",\n", s_sensorTempData.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"temp_avg\": \"%ld\",\n", s_sensorTempData.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"pressure_min\": \"%ld\",\n", s_sensorPressureData.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"pressure_max\": \"%ld\",\n", s_sensorPressureData.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"pressure_avg\": \"%ld\",\n", s_sensorPressureData.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"hum_min\": \"%ld\",\n", s_sensorHumidityData.Min);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"hum_max\": \"%ld\",\n", s_sensorHumidityData.Max);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "\"hum_avg\": \"%ld\"\n", s_sensorHumidityData.Avg);
	g_sensorReportBuffer.length += sprintf(g_sensorReportBuffer.data + g_sensorReportBuffer.length, "}}\n");


	/**** Reset the Averaging Data ****/
	Sensor_resetReportData();
}

/**
 * @brief in DTA_send_ih.h header
 */
void Sensor_sendConfigDataTimerHandler(xTimerHandle pvParameters)
{
	(void) pvParameters;

	/**** Write the configuration data into the Config Buffer in JSON Format ****/
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "{\n");
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"groupId\": \"%s\",\n",  CFG_getGroupId());
    g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"confRate\": %ld,\n", CFG_getConfigRate());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"reportSamples\": %ld,\n", CFG_getReportSamples());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"streamRate\": %ld,\n",CFG_getStreamRate());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"sensors\": {\n");
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"temp\": %s,\n", CFG_getTempStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"hum\": %s,\n", CFG_getHumStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"light\": %s,\n", CFG_getLightStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"pres\": %s,\n", CFG_getPresStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"acc\": %s,\n", CFG_getAccelStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"gyro\": %s,\n", CFG_getGyroStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "\"mag\": %s,\n", CFG_getMagStautsString());
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "}\n");
	g_sensorConfigBuffer.length += sprintf(g_sensorConfigBuffer.data + g_sensorConfigBuffer.length, "}\n");
}

/**
 * @brief in DTA_send_ih.h header
 */
void Sensor_resetReportData(void)
{
	/**** Reset the Report Data ****/
	s_sensorAccelDataX = ResetDataBuf;
	s_sensorAccelDataY = ResetDataBuf;
	s_sensorAccelDataZ = ResetDataBuf;
	s_sensorGyroDataX = ResetDataBuf;
	s_sensorGyroDataY = ResetDataBuf;
	s_sensorGyroDataZ = ResetDataBuf;
	s_sensorMagDataX = ResetDataBuf;
	s_sensorMagDataY = ResetDataBuf;
	s_sensorMagDataZ = ResetDataBuf;
	s_sensorLightData = ResetDataBuf;
	s_sensorTempData = ResetDataBuf;
	s_sensorPressureData = ResetDataBuf;
	s_sensorHumidityData = ResetDataBuf;

	s_sensorRunAvgCounter = SENSOR_INIT_ONE;
}

/**
 * @brief in DTA_send_ih.h header
 */
void Sensor_init(void)
{
	/* Initialize all sensors */
	sensorInitializeAccel();
	sensorInitializeGyro();
	sensorInitializeMag();
	sensorInitializeLight();
	sensorInitializeEnv();

	/* Initialize Memory Buffers */
	memset(g_sensorStreamBuffer.data, 0x00, SENSOR_DATA_BUF_SIZE);
	g_sensorStreamBuffer.length = NUMBER_UINT32_ZERO;

	memset(g_sensorReportBuffer.data, 0x00, SENSOR_DATA_BUF_SIZE);
	g_sensorReportBuffer.length = NUMBER_UINT32_ZERO;

	memset(g_sensorConfigBuffer.data, 0x00, SENSOR_DATA_BUF_SIZE);
	g_sensorConfigBuffer.length = NUMBER_UINT32_ZERO;

	Sensor_resetReportData();
}

/**
 * @brief in DTA_send_ih.h header
 */
void Sensor_deinit(void)
{
  ;/* nothing to do */
}

/** ************************************************************************* */
