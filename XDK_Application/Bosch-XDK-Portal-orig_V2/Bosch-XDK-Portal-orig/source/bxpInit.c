/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	bxpInit.c
**
**	DESCRIPTION:	Initializes the MQTT Client Project
**
**	PURPOSE:        Initializes the Sensors, Wi-Fi, MQTT Client, and the Buttons.
**	                This is the initial point of entry for the user defined code
**
**	AUTHOR(S):      Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**  2016.Apr         crk        BCDS         Initial Release
**
*******************************************************************************/

/* system header files */
#include <stdint.h>
#include <stdio.h>

/* own header files */
#include "BCDS_bxpCommon.h"
#include "BCDS_bxpConfig.h"
#include "bxpMqttClient.h"
#include "BCDS_bxpSensor.h"
#include "BCDS_bxpWifi.h"

/* additional interface header files */
#include "FreeRTOS.h"
#include "timers.h"
#include "PTD_portDriver_ph.h"
#include "PTD_portDriver_ih.h"
#include "WDG_watchdog_ih.h"

/* paho header files */
#include "MQTTClient.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */

/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief Configures and Initializes the user application, Sensors, Wi-Fi module
 *            MQTT Client, buttons and watchdog timer
 */
void bisInit(void) {
	/**** Initialize the Configuration ****/
	CFG_init();

    /* Initialize Sensors */
	Sensor_init();

    /* Initialize WiFi */
    wifiInit();

    /* Initialize Client */
    clientInit();

    /* Initialize the Watchdog Timer */
    WDG_init(WDG_FREQ, WDG_TIMEOUT);

    /**** Turn Yellow LED on During Initialization/Configuration ****/
    PTD_pinOutClear(PTD_PORT_LED_YELLOW, PTD_PIN_LED_YELLOW);
    PTD_pinOutSet(PTD_PORT_LED_ORANGE, PTD_PIN_LED_ORANGE);
    PTD_pinOutClear(PTD_PORT_LED_RED, PTD_PIN_LED_RED);

    printf("Initialization Complete!\r\n");
}

/* global functions ********************************************************* */

/**
 * @brief Initializes the User Application
 *
 * @param[in] xTimer - necessary for timer callback, UNUSED.
 */
void appInitSystem(xTimerHandle xTimer)
{
	(void) (xTimer);

    /**** Turn Yellow LED on During Initialization/Configuration ****/
    PTD_pinOutSet(PTD_PORT_LED_YELLOW, PTD_PIN_LED_YELLOW);
    PTD_pinOutClear(PTD_PORT_LED_ORANGE, PTD_PIN_LED_ORANGE);
    PTD_pinOutClear(PTD_PORT_LED_RED, PTD_PIN_LED_RED);

    bisInit();
}

/** ************************************************************************* */
