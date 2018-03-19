/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	bxpMqttClient.c
**
**	DESCRIPTION:	Source Code for the MQTT Paho Client
**
**	PURPOSE:        Initializes the Paho Client and sets up subscriptions,
**	                starts the task to pubish and receive data,
**	                initializes the timer to stream data,
**	                defines the callback function for subscibed topics
**
**	AUTHOR(S):		Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**  2016.Apr         crk        BCDS         Initial Release
**
*******************************************************************************/

/* system header files */
#include <stdint.h>

/* own header files */
#include "bxpMqttClient.h"
#include "BCDS_bxpCommon.h"
#include "BCDS_bxpConfig.h"
#include "BCDS_bxpSensor.h"

/* additional interface header files */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "PTD_portDriver_ph.h"
#include "PTD_portDriver_ih.h"
#include "BCDS_Basics.h"
#include "WDG_watchdog_ih.h"

/* paho header files */
#include "MQTTConnect.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */
// Buffers
static unsigned char buf[CLIENT_BUFF_SIZE];
static unsigned char readbuf[CLIENT_BUFF_SIZE];

// Client Task/Timer Variables
static xTimerHandle     clientStreamTimerHandle      = POINTER_NULL;  // timer handle for data stream
static xTimerHandle     clientConfigTimerHandle      = POINTER_NULL;  // timer handle for data stream
static uint32_t			clientTimerStreamValue       = NUMBER_UINT32_ZERO;		/**< Period in milliseconds for the stream timer */
static uint32_t			clientTimerConfigValue       = NUMBER_UINT32_ZERO;		/**< Period in milliseconds for the config timer */


static xTaskHandle      clientTaskHandler            = POINTER_NULL;  // task handle for MQTT Client
static uint32_t         clientMessageId              = 0;
mqttClientMessages      clientQueue                  = NO_ACTION;

// Subscribe topics variables
char clientTopicStream[CLIENT_BUFF_SIZE];
char clientTopicReport[CLIENT_BUFF_SIZE];
char clientTopicConfig[CLIENT_BUFF_SIZE];
char clientTopicSingleConfig[CLIENT_BUFF_SIZE];
char clientTopicGroupConfig[CLIENT_BUFF_SIZE];
const char *clientTopicStream_ptr = TOPIC_OUT_STREAM;
const char *clientTopicReport_ptr = TOPIC_OUT_REPORT;
const char *clientTopicConfig_ptr = TOPIC_OUT_CONFIG;
const char *clientTopicSingleConfig_ptr = TOPIC_IN_CONFIG;
const char *clientTopicGroupConfig_ptr = TOPIC_GROUP_CONFIG;

/* global variables ********************************************************* */
// Network and Client Configuration
Network n;
Client c;

/* inline functions ********************************************************* */

/* local functions ********************************************************** */
static void clientRecv(MessageData* md);
static void clientTask(void *pvParameters);
static void clientReconfigure(char * configData, int configLength);
static void clientUpdateTimerPeriod(void);

/**
 * @brief in MQTT_Client_ch.h header
 */
static void clientUpdateTimerPeriod(void)
{
	/**** Initialize Variables ****/
	int8_t _err = CMN_FALSE;

	/**** Read the current Streaming Rate ****/
	clientTimerStreamValue = CFG_getStreamRate();
	clientTimerConfigValue = CFG_getConfigRate();

	/**** Update the timer periods if valid, else stop the timers ****/
	if(FASTEST_TRANSMISSION_SPEED <= clientTimerStreamValue ) {
		_err = xTimerChangePeriod(clientStreamTimerHandle, clientTimerStreamValue/portTICK_PERIOD_MS, 100);
		/* Timer start fail case */
		if (pdFAIL == _err) {
			printf("The Stream timer was not started, Due to Insufficient heap memory\r\n");
		}
	}
	else {
		printf("Invalid stream rate = %ld; Turn Off Timer \r\n", clientTimerStreamValue);
		clientTimerStreamValue = NUMBER_UINT32_ZERO;
		_err = xTimerStop(clientStreamTimerHandle, UINT32_MAX);
	}

	if(FASTEST_TRANSMISSION_SPEED <= clientTimerConfigValue ) {
		_err = xTimerChangePeriod(clientConfigTimerHandle, clientTimerConfigValue/portTICK_PERIOD_MS, 100);
		/* Timer start fail case */
		if (pdFAIL == _err) {
			printf("The Config timer was not started, Due to Insufficient heap memory\r\n");
		}
	}
	else {
		printf("Invalid config rate = %ld; Turn Off Timer \r\n", clientTimerConfigValue);
		clientTimerConfigValue = NUMBER_UINT32_ZERO;
		_err = xTimerStop(clientConfigTimerHandle, UINT32_MAX);
	}
}

/**
 * @brief callback function for subriptions
 *        toggles LEDS or sets read data flag
 *
 * @param[in] md - received message from the MQTT Broker
 *
 * @return NONE
 */
static void clientReconfigure(char * configData, int configLength)
{
	/* Turn Yellow LED On During Configuration */
	PTD_pinOutSet(PTD_PORT_LED_YELLOW, PTD_PIN_LED_YELLOW);
	PTD_pinOutClear(PTD_PORT_LED_ORANGE, PTD_PIN_LED_ORANGE);
	PTD_pinOutClear(PTD_PORT_LED_RED, PTD_PIN_LED_RED);

    /* Stop Timers and Task During Configuration */
	clientStopTimer();
    vTaskSuspend(clientTaskHandler);

    /* Parse the JSON File Received */
    if(CFG_ParseJsonConfigFile((const char *)configData, configLength) != CMN_TRUE) {
    	printf("Error Parsing MQTT Configuration File");
    }

    /**** Update the Client's Configuration ****/
    clientUpdateTimerPeriod();
    clientQueue = SUBCRIBE_UPDATE;

    /* Restart the Timers and Task for Data Transfer */
    clientStartTimer();
    vTaskResume(clientTaskHandler);

	/* Turn Orange LED On to indicate Data Aquisition */
	PTD_pinOutSet(PTD_PORT_LED_ORANGE, PTD_PIN_LED_ORANGE);
	PTD_pinOutClear(PTD_PORT_LED_YELLOW, PTD_PIN_LED_YELLOW);
	PTD_pinOutClear(PTD_PORT_LED_RED, PTD_PIN_LED_RED);
}

/**
 * @brief callback function for subriptions
 *        toggles LEDS or sets read data flag
 *
 * @param[in] md - received message from the MQTT Broker
 *
 * @return NONE
 */
static void clientRecv(MessageData* md)
{
	/* Initialize Variables */
	MQTTMessage* message = md->message;

	if((strncmp(md->topicName->lenstring.data, clientTopicSingleConfig_ptr, md->topicName->lenstring.len) == 0)) {
		/* Reconfigure the XDK */
		clientReconfigure((char*)message->payload, (int) message->payloadlen);
	}
	else if((strncmp(md->topicName->lenstring.data, clientTopicGroupConfig_ptr, md->topicName->lenstring.len) == 0)) {
		/* Reconfigure the XDK */
		clientReconfigure((char*)message->payload, (int) message->payloadlen);
	}

	printf("Subscribed Topic, %.*s, Message Received: %.*s\r\n", md->topicName->lenstring.len, md->topicName->lenstring.data,
			                                                   (int)message->payloadlen, (char*)message->payload);
}

/**
 * @brief publish sensor data, get sensor data, or
 *        yield mqtt client to check subscriptions
 *
 * @param[in] pvParameters UNUSED/PASSED THROUGH
 *
 * @return NONE
 */
static void clientTask(void *pvParameters)
{
	/* Initialize Variables */
	(void) (pvParameters);
	MQTTMessage msg;

	/* Forever Loop Necessary for freeRTOS Task */
    for(;;)
    {
    	WDG_feedingWatchdog();
    	/* Publish Live Data Stream */
        if(g_sensorStreamBuffer.length > NUMBER_UINT32_ZERO)
        {
            msg.id = clientMessageId++;
            msg.qos = 0;
            msg.payload = g_sensorStreamBuffer.data;
            msg.payloadlen = g_sensorStreamBuffer.length;
            MQTTPublish(&c, clientTopicStream_ptr, &msg);

        	memset(g_sensorStreamBuffer.data, 0x00, SENSOR_DATA_BUF_SIZE);
        	g_sensorStreamBuffer.length = NUMBER_UINT32_ZERO;
        }
    	/* Publish Report Data Stream */
        else if(g_sensorReportBuffer.length > NUMBER_UINT32_ZERO)
        {
            msg.id = clientMessageId++;
            msg.qos = 0;
            msg.payload = g_sensorReportBuffer.data;
            msg.payloadlen = g_sensorReportBuffer.length;
            MQTTPublish(&c, clientTopicReport_ptr, &msg);

        	memset(g_sensorReportBuffer.data, 0x00, SENSOR_DATA_BUF_SIZE);
        	g_sensorReportBuffer.length = NUMBER_UINT32_ZERO;
        }
    	/* Publish Config Data Stream */
        else if(g_sensorConfigBuffer.length > NUMBER_UINT32_ZERO)
        {
            msg.id = clientMessageId++;
            msg.qos = 0;
            msg.payload = g_sensorConfigBuffer.data;
            msg.payloadlen = g_sensorConfigBuffer.length;
            MQTTPublish(&c, clientTopicConfig_ptr, &msg);

        	memset(g_sensorConfigBuffer.data, 0x00, SENSOR_DATA_BUF_SIZE);
        	g_sensorConfigBuffer.length = NUMBER_UINT32_ZERO;
        }
        /**** Subscribe to New Topics ****/
        else if(SUBCRIBE_UPDATE == clientQueue) {
            //mqttClientUnSubscribe();
            //mqttClientSubscribe();
            clientQueue = NO_ACTION;
        }
        /**** Disconnect Form the MQTT Broker ****/
        else if(BROKER_DISCONNECTION == clientQueue)
        {
        	clientQueue = NO_ACTION;
        	clientDeinit();
        }
        else {
            MQTTYield(&c, CLIENT_YIELD_TIMEOUT);
        }
    }
}

/* global functions ********************************************************* */

/**
 * @brief starts the data streaming timer
 *
 * @return NONE
 */
void clientStartTimer(void)
{
	/* Start the timers */
	xTimerStart(clientStreamTimerHandle, UINT32_MAX);
	xTimerStart(clientConfigTimerHandle, UINT32_MAX);
	return;
}
/**
 * @brief stops the data streaming timer
 *
 * @return NONE
 */
void clientStopTimer(void)
{
	/* Stop the timers */
	xTimerStop(clientStreamTimerHandle, UINT32_MAX);
	xTimerStop(clientConfigTimerHandle, UINT32_MAX);
	return;
}

/**
 * @brief Initializes the MQTT Paho Client, set up subscriptions and initializes the timers and tasks
 *
 * @return NONE
 */
void clientInit(void)
{
	/* Initialize Variables */
    int rc = 0;
    char * mqttBroker = (char*) CFG_getMqttBrokerName();
    int mqttPort = CFG_getMqttPort();
    printf("Just arrived in the point nr. 1 of bxpMqttClient.c \n");
    NewNetwork(&n);
    printf("Arrived point nr. 2 \n");
    ConnectNetwork(&n, mqttBroker, mqttPort);
    printf("Arrived point nr. 3 \n");
    MQTTClient(&c, &n, 1000, buf, CLIENT_BUFF_SIZE, readbuf, CLIENT_BUFF_SIZE);
    printf("Arrived point nr. 4 \n");

    /* Configure the MQTT Connection Data */
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.willFlag = 0;
    data.MQTTVersion = 4;
    char str[16];
    CFG_getMqttRandomClient(str);
    data.clientID.cstring = (char*) str;//CFG_getMqttRandomClient(); /* CFG_getMqttClientId() */
    data.keepAliveInterval = 100;
    data.cleansession = 1;
    printf("ClientID is %s \r\n",data.clientID.cstring);

    printf("Connecting to %s %d\r\n", mqttBroker, mqttPort);

    /* Connect to the MQTT Broker */
    rc = MQTTConnect(&c, &data);

    printf("Arrived to point nr.5 \n");
	/* Set Subscribe Topic Strings */
	memset(clientTopicStream, 0x00, CLIENT_BUFF_SIZE);
	printf("Arrived to point nr.6 \n");
	sprintf((char*) clientTopicStream, TOPIC_OUT_STREAM, (const char*) data.clientID.cstring);
	printf("Arrived to point nr.7 \n");
	clientTopicStream_ptr = (char*) clientTopicStream;
	printf("Arrived to point nr.8 \n");

	memset(clientTopicReport, 0x00, CLIENT_BUFF_SIZE);
	printf("Arrived to point nr.9 \n");
	sprintf((char*) clientTopicReport, TOPIC_OUT_REPORT, (const char*) data.clientID.cstring);
	printf("Arrived to point nr.10 \n");
	clientTopicReport_ptr = (char*) clientTopicReport;
	printf("Arrived to point nr.11 \n");

	memset(clientTopicConfig, 0x00, CLIENT_BUFF_SIZE);
	sprintf((char*) clientTopicConfig, TOPIC_OUT_CONFIG, (const char*) data.clientID.cstring);
	clientTopicConfig_ptr = (char*) clientTopicConfig;
	printf("Arrived to point nr.12 \n");
	memset(clientTopicSingleConfig, 0x00, CLIENT_BUFF_SIZE);
	sprintf((char*) clientTopicSingleConfig, TOPIC_IN_CONFIG, (const char*) data.clientID.cstring);
	clientTopicSingleConfig_ptr = (char*) clientTopicSingleConfig;
	printf("Arrived to point nr.13 \n");
	memset(clientTopicGroupConfig, 0x00, CLIENT_BUFF_SIZE);
	sprintf((char*) clientTopicGroupConfig, TOPIC_GROUP_CONFIG, (const char*) data.clientID.cstring);
	clientTopicGroupConfig_ptr = (char*) clientTopicGroupConfig;
	printf("Arrived to point nr.14 \n");

	/* Subscribe to the Topics (set callback functions) */
    rc = MQTTSubscribe(&c, clientTopicSingleConfig_ptr, QOS0, clientRecv);
    rc = MQTTSubscribe(&c, clientTopicGroupConfig_ptr, QOS0, clientRecv);
    printf("Arrived to point nr.15 \n");
	/* Create Live Data Stream Timer */
    clientStreamTimerHandle = xTimerCreate(
			(const char * const) "Data Stream",
			CFG_getStreamRate(),
			TIMER_AUTORELOAD_ON,
			NULL,
			Sensor_sendStreamDataTimerHandler);

    clientConfigTimerHandle = xTimerCreate(
			(const char * const) "Config Stream",
			CFG_getConfigRate(),
			TIMER_AUTORELOAD_ON,
			NULL,
			Sensor_sendConfigDataTimerHandler);
    printf("Arrived to point nr.16 \n");
	/* Create MQTT Client Task */
    rc = xTaskCreate(clientTask, (const char * const) "Mqtt Client App",
                    		CLIENT_TASK_STACK_SIZE, NULL, CLIENT_TASK_PRIORITY, &clientTaskHandler);
    printf("Arrived to point nr.17 \n");
    clientStartTimer();
    printf("Arrived to point nr.18 \n");
    /* Error Occured Exit App */
    if(rc < 0)
    {
    	printf("Arrived to point of clientDeinit() \n");
    	clientDeinit();
    }

    return;
}

/**
 * @brief Disconnect from the MQTT Client
 *
 * @return NONE
 */
void clientDeinit(void)
{
	printf("Oh....there was an error??? \n\n");
	/* Turn Red LED On If Failure to Connect to Client */
	PTD_pinOutSet(PTD_PORT_LED_RED, PTD_PIN_LED_RED);
	PTD_pinOutClear(PTD_PORT_LED_ORANGE, PTD_PIN_LED_ORANGE);
	PTD_pinOutClear(PTD_PORT_LED_YELLOW, PTD_PIN_LED_YELLOW);

    MQTTDisconnect(&c);
    n.disconnect(&n);
}
