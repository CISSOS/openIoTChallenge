/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	mqttClient.h
**
**	DESCRIPTION:	Interface header for the MQTT Paho Client module
**
**	PURPOSE:        Contains all the definitions and macros for the
**                  MQTT client module
**
**	AUTHOR(S):		Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**  2016.Apr         crk        BCDS         Initial Release
**
*******************************************************************************/

/* header definition ******************************************************** */
#ifndef _BXP_MQTT_CLIENT_H_
#define _BXP_MQTT_CLIENT_H_

/* Paho Client interface declaration ********************************************** */
#include "MQTTClient.h"

/* Paho Client type and macro definitions */
#define CLIENT_TASK_STACK_SIZE            1024
#define CLIENT_TASK_PRIORITY              1

#define CLIENT_BUFF_SIZE                  1000
#define CLIENT_YIELD_TIMEOUT              10

#define TOPIC_OUT_STREAM        "BCDS/%s/single/XDK-Vito/out/stream"
#define TOPIC_OUT_REPORT        "BCDS/%s/single/XDK-Vito/out/report"
#define TOPIC_OUT_CONFIG        "BCDS/%s/single/XDK-Vito/out/config"
#define TOPIC_IN_CONFIG         "BCDS/%s/single/XDK-Vito/in/config"
#define TOPIC_GROUP_CONFIG      "BCDS/%s/group/XDK-Vito/in/config"

// MQTT Event Messages Index
typedef enum events
{
	NO_ACTION = -1,
	SUBCRIBE_UPDATE = 0,
	BROKER_DISCONNECTION = 1,
} mqttClientMessages;



/* global function prototype declarations */
void clientInit(void);
void clientDeinit(void);
void clientStartTimer(void);
void clientStopTimer(void);

/* global variable declarations */
extern Network n;
extern Client c;

/* global inline function definitions */

#endif /* _BXP_MQTT_CLIENT_H_ */
