/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	BCDS_bxpConfig.h
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
#ifndef _BCDS_BXP_CONFIG_H_
#define _BCDS_BXP_CONFIG_H_

/* Config interface declaration ********************************************** */

/* Config type and macro definitions */

/* public type and macro definitions */
#define CFG_STR_TRUE         "true"
#define CFG_STR_FALSE        "false"
#define CFG_ENABLED			 0

#warning Provide Default WLAN and MQTT Configuration Here
// Default Network Configuration Settings
#define	CFG_DEFAULT_WLAN_SSID	  		"YOUR_SSID"      	  /**< WLAN SSID Name by Default*/
#define	CFG_DEFAULT_WLAN_PWD	  		"Your_Password"   		  /**< WLAN PWD by Default*/
#define CFG_DEFAULT_MQTT_BROKER_NAME    "172.16.1.1"          /**< MQTT Broker by Default*/
#define CFG_DEFAULT_MQTT_PORT           1883                          /**< MQTT Port Number by Default*/
/**
 *  The Stream Rate is period by which the live data is sent
 * The Report Samples are the number of samples used to calculate the Report Data (Avg, Max and Min)
 * A Report Sample is taken each time the live data is aquired
 * Report Rate = Stream Rate * Report Samples
 */

// Default Data Configuration Settings
#define CFG_DEFAULT_STREAM_RATE         1000/portTICK_RATE_MS     /**< Stream Data Rate by Default*/
#define CFG_DEFAULT_REPORT_SAMPLES      10                        /**< Data Average Report Samples by Default*/
#define CFG_DEFAULT_CONFIG_RATE         120000/portTICK_RATE_MS   /**< Coniguration Data Rate by Default*/
#define CFG_DEFAULT_GROUP_ID            "CISSOS_XDK"                 /**< Devices Group ID by Default*/
#define CFG_DEFAULT_ACCEL_EN            CFG_STR_TRUE              /**< Accelerometer Data Enable by Default*/
#define CFG_DEFAULT_GYRO_EN             CFG_STR_TRUE              /**< Gyroscope Data Enable by Default*/
#define CFG_DEFAULT_MAG_EN              CFG_STR_TRUE              /**< Magnetometer Data Enable by Default*/
#define CFG_DEFAULT_TEMP_EN             CFG_STR_TRUE              /**< Temperature Data Enable by Default*/
#define CFG_DEFAULT_HUM_EN              CFG_STR_TRUE              /**< Humidity Data Enable by Default*/
#define CFG_DEFAULT_PRES_EN             CFG_STR_TRUE              /**< Pressure Data Enable by Default*/
#define CFG_DEFAULT_LIGHT_EN            CFG_STR_TRUE              /**< Ambient Light Data Enable by Default*/

/* global function prototype declarations */

/**
 * @Brief initialize the CFG module and read and parse the SD Cards configuration file
 *
 * @return the task handle created for reading the configuration file , is
 *         NULL if the task could not be created !
 */
void CFG_init(void);

/**
 * @Brief returns the WLAN SSID name defined in the configuration file
 *        or the default WLAN SSID name
 */
const char *CFG_getWlanSsid(void);

/**
 * @Brief returns the WLAN Password defined in the configuration file
 *        or the default WLAN Password
 */
const char *CFG_getWlanPwd(void);

/**
 * @Brief returns the MQTT Client ID name defined in the configuration file
 *        or the default MQTT Client ID name
 */
const char *CFG_getMqttClientId(void);

//const char *CFG_getMqttRandomClient(void);
void CFG_getMqttRandomClient(char *);

/**
 * @Brief returns the MQTT Client ID name defined in the configuration file
 *        or the default MQTT Client ID name
 */
//const char *CFG_getMqttUserId(void);

/**
 * @Brief returns the MQTT Broker name defined in the configuration file
 *        or the default MQTT Broker name
 */
const char *CFG_getMqttBrokerName(void);

/**
 * @Brief returns the MQTT Port number defined in the configuration file
 *        or the default MQTT Port number
 */
int CFG_getMqttPort(void);

/**
 * @brief returns the string for the Group ID
 */
const char *CFG_getGroupId(void);

/**
 * @brief returns the publish rate for the configuration report
 */
long CFG_getConfigRate(void);

/**
 * @brief returns number of samples taken for each set of Report Data
 */
long CFG_getReportSamples(void);

/**
 * @brief returns the publish rate for the streamed sensor data
 */
long CFG_getStreamRate(void);

/**
 * @brief returns CMN_TRUE if the accelerometer data stream is enabled
 */
int CFG_isAccelEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the accelerometer data stream
 */
const char *CFG_getAccelStautsString(void);

/**
 * @brief returns CMN_TRUE if the gyroscope data stream is enabled
 */
int CFG_isGyroEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the gyroscope data stream
 */
const char *CFG_getGyroStautsString(void);

/**
 * @brief returns CMN_TRUE if the megnetometer data stream is enabled
 */
int CFG_isMagEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the magnetometer data stream
 */
const char *CFG_getMagStautsString(void);

/**
 * @brief returns CMN_TRUE if the ambient light data stream is enabled
 */
int CFG_isLightEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the ambient light data stream
 */
const char *CFG_getLightStautsString(void);

/**
 * @brief returns CMN_TRUE if the temperature data stream is enabled
 */
int CFG_isTempEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the temperature data stream
 */
const char *CFG_getTempStautsString(void);

/**
 * @brief returns CMN_TRUE if the pressure data stream is enabled
 */
int CFG_isPresEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the Pressure data stream
 */
const char *CFG_getPresStautsString(void);

/**
 * @brief returns CMN_TRUE if the Humidity data stream is enabled
 */
int CFG_isHumEnabled(void);

/**
 * @brief returns true/false string indicating the enable status
 *        of the humidity data stream
 */
const char *CFG_getHumStautsString(void);

/**
 * @brief Parse the config file read from the MQTT Broker
 *
 * @param[in] buffer
 *            The buffer containg the configuration file
 * @param[in] bufSize
 *            The size of the buffer
 *
 * @return CMN_TRUE if JSON file is parsed and stored correctly
 *
 */
uint8_t CFG_ParseJsonConfigFile(const char *buffer, uint16_t bufSize);

/**
 * @Brief deinitialize the CFG module
 */
void CFG_deinit(void);

/* public global variable declarations */

/* inline function definitions */

#endif /* _BCDS_BXP_CONFIG_H_ */

/** ************************************************************************* */
