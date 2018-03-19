/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	bxpConfig.c
**
**	DESCRIPTION:	Manages the Configuration Parameters
**
**	PURPOSE:        Reads the SD Card and parses the configuration files
**	                Parses the JSON via MQTT Subscriptions
**	                Stores the configuration data
**	                Reads the stored configuration data
**
**	AUTHOR(S):      Bosch Connected Devices and Solutions GmbH (BCDS)
**
**	Revision History:
**
**	Date			 Name		Company      Description
**	2015             unknown    BCDS         Initial Release (DMS_demoSensorMonitor)
**  2015.Oct         crk        BCDS         Added Configuration Variables to store
**                                           MQTT Data; Cleaned up uncessary code
**  2016.Jan         crk        BCDS         Added Data Configuration and JSON Parsing;
**                                           Further Consolidation/Cleanup of Code
**
*******************************************************************************/

/* module includes ********************************************************** */

/* system header files */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* own header files */
#include "BCDS_bxpConfig.h"
#include "BCDS_bxpCommon.h"
#include "BCDS_bxpWifi.h"

/* SDK interface header files */
#include "BCDS_SDCardDriver.h"
#include "PTD_portDriver_ph.h"
#include "PTD_portDriver_ih.h"
#include "ff.h"

/* local header files */
#include "bxpConfig.h"

/* constant definitions ***************************************************** */

/* local variables ********************************************************** */
/** Variable containers for network configuration values */
static char N1Val[CFG_MAX_LINE_SIZE];
static char N2Val[CFG_MAX_LINE_SIZE];
static char N3Val[CFG_MAX_LINE_SIZE];
static char N4Val[CFG_MAX_LINE_SIZE];

/** Variable containers for data configuration values */
static char D1Val[CFG_MAX_LINE_SIZE];
static char D2Val[CFG_MAX_LINE_SIZE];
static char D3Val[CFG_MAX_LINE_SIZE];
static char D4Val[CFG_MAX_LINE_SIZE];
static char D5Val[CFG_MAX_LINE_SIZE];
static char D6Val[CFG_MAX_LINE_SIZE];
static char D7Val[CFG_MAX_LINE_SIZE];
static char D8Val[CFG_MAX_LINE_SIZE];
static char D9Val[CFG_MAX_LINE_SIZE];
static char D10Val[CFG_MAX_LINE_SIZE];
static char D11Val[CFG_MAX_LINE_SIZE];
static char D12Val[CFG_MAX_LINE_SIZE];

/** Attribute names on the Network configuration file */
static char N1Name[] = "HUAWEI P9";
static char N2Name[] = "2c17a655";
static char N3Name[] = "192.168.43.1";
static char N4Name[] = "1883";

/** Attribute names on the Data configuration file */
static char D1Name[]  = "groupId";
static char D2Name[]  = "confRate";
static char D3Name[]  = "reportSamples";
static char D4Name[]  = "streamRate";
static char D5Name[]  = "temp";
static char D6Name[]  = "hum";
static char D7Name[]  = "light";
static char D8Name[]  = "pres";
static char D9Name[]  = "acc";
static char D10Name[] = "gyro";
static char D11Name[] = "mag";
static char D12Name[] = "sensors";
/*
 * Configuration holder structure array
 */
static configLine_t s_nconfig[] = {
			 {N1Name, N1Val, CMN_FALSE},
			 {N2Name, N2Val, CMN_FALSE},
			 {N3Name, N3Val, CMN_FALSE},
			 {N4Name, N4Val, CMN_FALSE}
};

static configLine_t s_dconfig[] = {
			 {D1Name,  D1Val,  CMN_FALSE},
			 {D2Name,  D2Val,  CMN_FALSE},
			 {D3Name,  D3Val,  CMN_FALSE},
			 {D4Name,  D4Val,  CMN_FALSE},
			 {D5Name,  D5Val,  CMN_FALSE},
			 {D6Name,  D6Val,  CMN_FALSE},
			 {D7Name,  D7Val,  CMN_FALSE},
			 {D8Name,  D8Val,  CMN_FALSE},
			 {D9Name,  D9Val,  CMN_FALSE},
			 {D10Name, D10Val, CMN_FALSE},
			 {D11Name, D11Val, CMN_FALSE},
			 {D12Name, D12Val, CMN_FALSE},
};


/* global variables ********************************************************* */

/* inline functions ********************************************************* */

/* local functions ********************************************************** */

/**
 * @brief in CFG_parser_ch.h header
 */
static uint8_t cfgJsonTokenParser(const char *buffer, jsmntok_t *token, jsmnerr_t numTokens)
{
	/**** Initialize Variables ****/
	uint8_t _res = CMN_TRUE;
	uint8_t _tokenNameIdx, _attNameIdx, _tokenValueIdx;
	uint8_t _tokenNameSize = 0;
	uint8_t _tokenValueSize = 0;

	/**** Read Each Token Name/Value Pair ****/
	for (_tokenNameIdx = 1; _tokenNameIdx < numTokens; _tokenNameIdx += 2) {
		/**** Initialize current pair's values ****/
		_tokenValueIdx = _tokenNameIdx + 1;
		_tokenNameSize = token[_tokenNameIdx].end - token[_tokenNameIdx].start;
		_tokenValueSize = token[_tokenValueIdx].end - token[_tokenValueIdx].start;

        /**** Find Token Name ****/
        for(_attNameIdx = 0; _attNameIdx <= ATT_DATA_SIZE; _attNameIdx++) {
           if(strncmp(buffer + token[_tokenNameIdx].start, s_dconfig[_attNameIdx].attName, _tokenNameSize) == 0) {
        	   if (_attNameIdx != ATT_IDX_JSON) {
        		   /**** Ignore Quotations at the begining and end of the JSON token and copy the toke to the configuration variables ****/
        		   if ((strncmp(buffer + token[_tokenValueIdx].start, "\"", 1) == 0) || (strncmp(buffer + token[_tokenValueIdx].start, "\'", 1) == 0)) {
        			   strncpy(s_dconfig[_attNameIdx].attValue, buffer + (token[_tokenValueIdx].start + 1), (_tokenValueSize - 2));
        		   }
        		   else {
        			   strncpy(s_dconfig[_attNameIdx].attValue, buffer + token[_tokenValueIdx].start, _tokenValueSize);
        		   }
        		   /**** Set the defined flag for the configuration variable table ****/
                   if(s_dconfig[_attNameIdx].defined == CMN_FALSE) {
                       s_dconfig[_attNameIdx].defined = CMN_TRUE;
                   }
        	   }
               break;
           }
        }
	}
	return _res;
}

/**
 * @brief in CFG_parser_ch.h header
 */
static tokensType_t cfgGetToken(const char *buffer,
						uint16_t *idxAtBuffer,
						uint16_t bufSize,
						char *token,
						uint16_t *tokenSize,
						int8_t fileIndex)
{
	/**** Initialize Local Variables ****/
	tokensType_t  _res = CFG_TOKEN_TYPE_UNKNOWN;
    int8_t        _attSize = 0;
    configLine_t *_config_ptr;
    uint8_t       _i =0;

    switch(fileIndex) {
        case CFG_IDX_NET :
        {
        	_config_ptr = s_nconfig;
        	_attSize = ATT_NET_SIZE;
        	break;
        }
        case CFG_IDX_DATA :
		{
			_config_ptr = s_dconfig;
			_attSize = ATT_DATA_SIZE;
			break;
		}
        default :
		{
			printf("Unknown File! \r\n");
			_res = CMN_FALSE;
			break;
		}
    }


	/**** erase the OUTPUT token buffer ****/
	memset(token,0,*tokenSize);

    /**** Bypass all unusefull chars like \r \n and blanks ****/
    while(
          ((buffer[*idxAtBuffer]=='\r') ||
          (buffer[*idxAtBuffer]=='\n') ||
          (buffer[*idxAtBuffer]==' ') )
          &&
            (*idxAtBuffer < bufSize)
          )
    {
    	*idxAtBuffer = *idxAtBuffer + 1;
    }
    /**** is the next char an EQUAL sign? ****/
    if(buffer[*idxAtBuffer] == '=') {
        token[_i] = buffer[*idxAtBuffer];
        *idxAtBuffer = *idxAtBuffer + 1;
        _i = _i+1;
        _res =  CFG_TOKEN_EQUAL;
    }
    /**** Next Characther is part of token ****/
    else {
    	/**** Store Characters until end of Token ****/
    	while(
			  (buffer[*idxAtBuffer]!='\r') &&
			  (buffer[*idxAtBuffer]!='\n') &&
			  (buffer[*idxAtBuffer]!=' ') &&
			  (buffer[*idxAtBuffer]!='=') &&
			  (*idxAtBuffer < bufSize)
			 ) {
				if(_i< *tokenSize -1) {
					token[_i] = buffer[*idxAtBuffer];
					*idxAtBuffer = *idxAtBuffer +1 ;
					_i = _i+1;
				}
			};
    	/**** Is this string a known attribute name? ****/
        for(uint8_t _j=0; _j < _attSize; _j++) {
            if(0==strcmp(token,_config_ptr[_j].attName)) {
            	_res = CFG_TOKEN_ATT_NAME;
            	break;
            }
        }
        /**** The string is not known, so it's a value ****/
        if((_i >0) && (CFG_TOKEN_TYPE_UNKNOWN == _res )) {
			_res =  CFG_TOKEN_VALUE;
        }
    }
    /**** At this point nothing has been found, End Of File! ****/
    if(CFG_TOKEN_TYPE_UNKNOWN == _res ) {
    	_res =  CFG_TOKEN_EOF;
    }
    return _res;
}

/**
 * @brief in CFG_parser_ch.h header
 */
static uint8_t cfgParseSDConfigFile(const char *buffer, uint16_t bufSize, int8_t fileIndex)
{
	/**** Initialize Variables ****/
    uint16_t      _idxAtBuffer = 0;
    uint8_t       _res= CMN_TRUE;
    states_t      _state = STATE_EXP_ATT_NAME;
    uint8_t       _currentConfigToSet = -1;
    uint8_t       _numberOfDefinedAttributes = 0;
    char          _aToken[CFG_MAX_LINE_SIZE];
    uint16_t      _outputTokenSize= 0;
    int8_t        _attSize = 0;
    configLine_t *_config_ptr;

    switch(fileIndex) {
        case CFG_IDX_NET :
        {
        	_config_ptr = s_nconfig;
        	_attSize = ATT_NET_SIZE;
        	break;
        }
        case CFG_IDX_DATA :
		{
			_config_ptr = s_dconfig;
			_attSize = ATT_DATA_SIZE;
			break;
		}
        default :
		{
			printf("Unknown File! \r\n");
			_res = CMN_FALSE;
			break;
		}
    }

    while(CMN_TRUE == _res )
    {
    	/**** Parse Token from Buffer ****/
    	_outputTokenSize = CFG_MAX_LINE_SIZE;
    	tokensType_t _getTokenType = cfgGetToken(buffer,&_idxAtBuffer, bufSize,_aToken,&_outputTokenSize, fileIndex);

    	/**** End of File Token ****/
        if(_getTokenType == CFG_TOKEN_EOF )
            break;

        /**** State Machine for Parsing the Buffer ****/
        switch(_state) {
            case STATE_EXP_ATT_NAME :
            {
            	/**** Compare Expected Token with Parsed Token ****/
            	if(_getTokenType !=CFG_TOKEN_ATT_NAME)
                {
                    printf("Expecting attname at %d\r\n", (_idxAtBuffer - strlen(_aToken)));
                    _res = CMN_FALSE;
                    break;
                }

                /**** Find Token Name ****/
                for(uint8_t i=0; i < _attSize; i++) {
                   if(strcmp(_aToken, _config_ptr[i].attName)==0) {
                       _currentConfigToSet = i;
                       _state =STATE_EXP_ATT_EQUAL;
                       break;
                   }
                }
                break;
            }
            case STATE_EXP_ATT_EQUAL :
            {
            	/**** Compare Expected Token with Parsed Token ****/
                if(_getTokenType !=CFG_TOKEN_EQUAL) {
                	printf("Expecting sign '=' at %d\r\n", (_idxAtBuffer - strlen(_aToken)));
                    _res = CMN_FALSE;
                    break;
                }
                _state = STATE_EXP_ATT_VALUE;
                 break;
            }
            case STATE_EXP_ATT_VALUE :
            {
            	/**** Compare Expected Token with Parsed Token ****/
            	if(_getTokenType !=CFG_TOKEN_VALUE) {
                	printf("Expecting value string at %d\r\n", (_idxAtBuffer - strlen(_aToken)));
                    _res = CMN_FALSE;
                    break;
                }

                /**** Copy Token into Attribute Value Variable ****/
                strcpy(_config_ptr[_currentConfigToSet].attValue, _aToken);

                /**** Check if the Attribute is Already Defined ****/
                if((_config_ptr[_currentConfigToSet].defined == CMN_FALSE) || (fileIndex == CFG_IDX_DATA)) {
                	_config_ptr[_currentConfigToSet].defined = CMN_TRUE;
                    _numberOfDefinedAttributes++;
                    _state = STATE_EXP_ATT_NAME ;
                }
                else if (fileIndex == CFG_IDX_NET) {
                	_res = CMN_FALSE;
                	printf("Attribute %s Defined Twice! \r\n", _config_ptr[_currentConfigToSet].attName);
                }
                break;
            }
            default :
            	printf("Unexpected state %d \r\n", _state);
            	break;
        }
    }
    if (fileIndex == CFG_IDX_NET) {
        /**** Verifying that all atributes are defined ****/
        for(uint8_t i=0; _res == CMN_TRUE && i < _attSize; i++)
        {
        	if(_config_ptr[i].defined == 0) {
            	printf("Value for attribute %s not defined !\r\n", _config_ptr[i].attName);
                _res = CMN_FALSE;
            }
        }
    }
    return _res;
}

/**
 * @brief in CFG_parser_ch.h header
 */
static void cfgSdcDisconnect(void)
{
    SDCardDriver_Disconnect();
    GPIO_PinOutClear( (GPIO_Port_TypeDef) PTD_PORT_LED_RED, PTD_PIN_LED_RED);
}

/**
 * @brief in CFG_parser_ch.h header
 */
static void cfgReadSDConfigFiles(void)
{
	/**** Initialize Variables ****/
	FRESULT 	 _fileSystemResult;
	FIL     	 _fileReadHandle;
	FATFS        _fileSystemObject;
	UINT     	 _bytesRead;
	int8_t       _fileIdx = 0;

	/**** Initialize the attribute values holders ****/
	memset(N1Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(N2Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(N3Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(N4Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D1Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D2Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D3Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D4Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D5Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D6Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D7Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D8Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D9Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D10Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D11Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);
	memset(D12Val, NUMBER_UINT8_ZERO, CFG_MAX_LINE_SIZE);

	if (CFG_DETECT_SD_CARD_INSERTED != SDCardDriver_GetDetectStatus())
	{
		/**** Potentially no sd card present! ****/
		printf("No SD card inserted !!\r\n");
		cfgSdcDisconnect();
		return;
	}

	if (SDCARD_NOT_INITIALIZED == s_cfgDiskInitStatus )
	{
		/**** Initialize SD card ****/
		s_cfgDiskInitStatus = SDCardDriver_DiskInitialize(CFG_DRIVE_ZERO);
	}

	if (SDCARD_INITIALIZED == s_cfgDiskInitStatus)
	{
		/**** Step 1 - Initialize file system ****/
		 if (f_mount(&_fileSystemObject, CFG_DEFAULT_LOGICAL_DRIVE, CFG_MOUNT_NOW) != FR_OK){
			printf("f_mount failed !!\r\n");
			cfgSdcDisconnect();
			return;
		 }

		 for (_fileIdx = 0; _fileIdx < CFG_IDX_SIZE; _fileIdx++) {
			/**** Step 2 - Open the file ****/
			_fileSystemResult = f_open(&_fileReadHandle, s_fconfig[_fileIdx].cfgFileName, FA_OPEN_EXISTING | FA_READ);
			if (_fileSystemResult != FR_OK){
				printf("f_open of Network Config failed !!\r\n");
				break;
			}

			/**** Step 3 - Set the file read pointer to first location ****/
			_fileSystemResult = f_lseek(&_fileReadHandle, CFG_SEEK_FIRST_LOCATION);
			if (_fileSystemResult != FR_OK){
				printf("f_lseek failed !!\r\n");
				break;
			}

			/**** Step 4 - Read a buffer from file ****/
			memset(s_cfgFileReadBuffer, NUMBER_UINT8_ZERO, CFG_FILE_READ_BUFFER_SIZE);
	        _bytesRead = 0;

			_fileSystemResult = f_read(&_fileReadHandle, s_cfgFileReadBuffer, CFG_FILE_READ_BUFFER_SIZE, &_bytesRead);

			/**** Step 5 - Parse the Configuration File ****/
			if(_fileSystemResult != FR_OK){
				printf("f_read failed!!\r\n");
			}

			/**** Parse the SD Cards Configuration File */
			cfgParseSDConfigFile( (const char*) s_cfgFileReadBuffer, _bytesRead, _fileIdx);

			/**** Step 6 - Close the file ****/
			_fileSystemResult = f_close(&_fileReadHandle);
			if (_fileSystemResult != FR_OK){
				printf("f_close failed!!\r\n");
			}
		 }
	}
	/**** Disconnect the SD Card ****/
	cfgSdcDisconnect();
	return;
}

/* global functions ********************************************************* */

/**
 * @brief in CFG_parser_ih.h header
 */
void CFG_init(void){
	/**** Initialize SD card ****/
	Retcode_T _sdInitreturnValue = SDCardDriver_Init();

	if (RETCODE_SUCCESS != _sdInitreturnValue) {
		/**** Potentially no sd card present ! ****/
		printf("SDC_init failed !!\r\n");
	}
	else {
		/* Read and Parse the SD Card Configuration Files */
		cfgReadSDConfigFiles();
	}
	return;
}



/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getWlanSsid(void) {
	if(CMN_TRUE == s_nconfig[ATT_IDX_NET_SSID].defined) {
		return (const char*)s_nconfig[ATT_IDX_NET_SSID].attValue;
	}
	else
		return CFG_DEFAULT_WLAN_SSID;
}

/**
 * @brief in CFG_parser_ih.h header
 */
extern const char *CFG_getWlanPwd(void) {
	if(CMN_TRUE == s_nconfig[ATT_IDX_NET_PWD].defined) {
		return (const char*)s_nconfig[ATT_IDX_NET_PWD].attValue;
	}
	else
		return CFG_DEFAULT_WLAN_PWD;
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getMqttClientId(void) {
	/**** Use the WLAN MAC address as the Client ID if available ****/
	if (0 != strcmp(WIFI_DEFAULT_MAC, g_mac_addr)) {
		return WIFI_DEFAULT_MAC; /*g_mac_addr;*/
	}
    else
		return WIFI_DEFAULT_MAC;
}

//const char *CFG_getMqttRandomClient(void) {
void CFG_getMqttRandomClient(char *dest){
    const char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    size_t size=15;
    --size;
    srand((unsigned)xTaskGetTickCount());
    for (size_t n = 0; n < size; n++) {
        int key = rand() % (int) (sizeof charset - 1);
        //str[n] = charset[key];
        *dest++=charset[key];
    }
    //str[size] = '\0';
    *dest='\0';
    //return *str;
}

/**
 * @brief in CFG_parser_ih.h header
 */
//const char *CFG_getMqttUserId(void) {
//	return CFG_DEFAULT_MQTT_USR;
//}


/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getMqttBrokerName(void) {
	if(CMN_TRUE == s_nconfig[ATT_IDX_BROKER].defined) {
		return (const char*) s_nconfig[ATT_IDX_BROKER].attValue;
	}
	else
		return CFG_DEFAULT_MQTT_BROKER_NAME;
}


/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_getMqttPort(void) {
	if(CMN_TRUE == s_nconfig[ATT_IDX_PORT].defined) {
		return strtol(s_nconfig[ATT_IDX_PORT].attValue, NULL, 10);
	}
	else
		return CFG_DEFAULT_MQTT_PORT;
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getGroupId(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_GID].defined) {
		return (const char*) s_dconfig[ATT_IDX_GID].attValue;
	}
	else
		return CFG_DEFAULT_GROUP_ID;
}

/**
 * @brief in CFG_parser_ih.h header
 */
long CFG_getConfigRate(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_CONF].defined) {
		return strtol(s_dconfig[ATT_IDX_CONF].attValue, NULL, 10);
	}
	else
		return CFG_DEFAULT_CONFIG_RATE;
}

/**
 * @brief in CFG_parser_ih.h header
 */
long CFG_getReportSamples(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_REP].defined) {
		return strtol(s_dconfig[ATT_IDX_REP].attValue, NULL, 10);
	}
	else
		return CFG_DEFAULT_REPORT_SAMPLES;
}

/**
 * @brief in CFG_parser_ih.h header
 */
long CFG_getStreamRate(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_STRM].defined) {
		return strtol(s_dconfig[ATT_IDX_STRM].attValue, NULL, 10);
	}
	else
		return CFG_DEFAULT_STREAM_RATE;
}


/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isAccelEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_ACCEL].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_ACCEL].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_ACCEL_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getAccelStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_ACCEL].defined) {
		return (const char*) s_dconfig[ATT_IDX_ACCEL].attValue;
	}
    else
		return CFG_DEFAULT_ACCEL_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isGyroEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_GYRO].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_GYRO].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_GYRO_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getGyroStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_GYRO].defined) {
		return (const char*) s_dconfig[ATT_IDX_GYRO].attValue;
	}
    else
		return CFG_DEFAULT_GYRO_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isMagEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_MAG].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_MAG].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_MAG_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getMagStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_MAG].defined) {
		return (const char*) s_dconfig[ATT_IDX_MAG].attValue;
	}
    else
		return CFG_DEFAULT_MAG_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isLightEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_LIGHT].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_LIGHT].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_LIGHT_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getLightStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_LIGHT].defined) {
		return (const char*) s_dconfig[ATT_IDX_LIGHT].attValue;
	}
    else
		return CFG_DEFAULT_LIGHT_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isTempEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_TEMP].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_TEMP].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_TEMP_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getTempStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_TEMP].defined) {
		return (const char*) s_dconfig[ATT_IDX_TEMP].attValue;
	}
    else
		return CFG_DEFAULT_TEMP_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isPresEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_PRES].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_PRES].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_PRES_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getPresStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_PRES].defined) {
		return (const char*) s_dconfig[ATT_IDX_PRES].attValue;
	}
    else
		return CFG_DEFAULT_PRES_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
int CFG_isHumEnabled(void) {
	if(CMN_TRUE == s_dconfig[ATT_IDX_HUM].defined) {
        return strcmp(CFG_STR_TRUE, s_dconfig[ATT_IDX_HUM].attValue);
	}
	return strcmp(CFG_STR_TRUE, CFG_DEFAULT_HUM_EN);
}

/**
 * @brief in CFG_parser_ih.h header
 */
const char *CFG_getHumStautsString(void) {
    if(CMN_TRUE == s_dconfig[ATT_IDX_HUM].defined) {
		return (const char*) s_dconfig[ATT_IDX_HUM].attValue;
	}
    else
		return CFG_DEFAULT_HUM_EN;
}

/**
 * @brief in CFG_parser_ih.h header
 */
uint8_t CFG_ParseJsonConfigFile(const char *buffer, uint16_t bufSize)
{
	/**** Initialize Variables ****/
	uint8_t   _res = CMN_FALSE;
	FIL       _fileObject;
	FRESULT   _fileSystemResult;
	UINT  _bytesWritten;
    uint32_t  _writeLength = 0;
    uint8_t  *_writeBuf = (uint8_t *)malloc(bufSize+1);
    jsmn_parser _jsonParser;
    jsmnerr_t _jsonNumTokens;
    jsmntok_t _jsonToken[CFG_MAX_TOKENS];

    memset(_writeBuf,'\0',bufSize+1);

    /**** Parse the JSON File ****/
    jsmn_init(&_jsonParser);
    _jsonNumTokens = jsmn_parse(&_jsonParser, buffer, bufSize, _jsonToken, CFG_MAX_TOKENS);
    cfgJsonTokenParser(buffer, _jsonToken, _jsonNumTokens);

    /**** Writhe the Buffer to send to the SD Card ****/
	for (uint8_t i = 0; i < ATT_DATA_SIZE - 1; i++) {
		_writeLength += sprintf((char*) _writeBuf + _writeLength, "%s = %s \r\n", s_dconfig[i].attName, s_dconfig[i].attValue);
	}
	_writeBuf[_writeLength]='\0';

	if (_res){
		/**** Step 1 - Initialize SD card API****/
		if (RETCODE_SUCCESS != SDCardDriver_Init()) {
			printf("SDC_init failed !!\r\n");
			return _res;
		}

		/**** Step 2 - Check for presensce of an SD Card ****/
		if (CFG_DETECT_SD_CARD_INSERTED != SDCardDriver_GetDetectStatus())
		{
			printf("No SD card inserted !!\r\n");
			cfgSdcDisconnect();
			return _res;
		}

		/**** Step 3 - Check if the SD Card Disk is initialized ****/
		if (SDCARD_NOT_INITIALIZED == s_cfgDiskInitStatus ) /*SD-Card Disk Not Initialized */
		{
			s_cfgDiskInitStatus = SDCardDriver_DiskInitialize(CFG_DRIVE_ZERO); /* Initialize SD card */
		}

		if (SDCARD_INITIALIZED == s_cfgDiskInitStatus)
		{
			/**** Step 4 - Initialize file system ****/
			 if (f_mount(CFG_DRIVE_ZERO, CFG_DEFAULT_LOGICAL_DRIVE, CFG_MOUNT_NOW) != FR_OK){
				printf("f_mount failed !!\r\n");
				cfgSdcDisconnect();
				_res = CMN_FALSE;
				return _res;
			 }

	        /**** Step 5 - open the file to write ****/
	        /*  If file does not exist create it*/
			 _fileSystemResult = f_open(&_fileObject, CFG_DATA_FILE_NAME,
	            FA_CREATE_ALWAYS | FA_WRITE);
	        if (_fileSystemResult != FR_OK)
	        {
	        	printf("f_open failed; cannot create the file !!\r\n");
	        	_res = CMN_FALSE;
				return _res;
	        }

	        /****Step 6 - Set the file write pointer to first location *****/
	        _fileSystemResult = f_lseek(&_fileObject, CFG_SEEK_FIRST_LOCATION);
	        if (_fileSystemResult != FR_OK)
	        {
	        	printf("f_lseek failed; cannot set the pointer !!\r\n");
	        	_res = CMN_FALSE;
				return _res;
	        }

	        /**** Step 7 - Write a buffer to file *****/
	        _fileSystemResult = f_write(&_fileObject, _writeBuf, _writeLength,
	            &_bytesWritten);
	        if ((_fileSystemResult != FR_OK) || (_writeLength != _bytesWritten))
	        {
	        	printf("f_write failed; cannot write the data !!\r\n");
	        	_res = CMN_FALSE;
				return _res;
	        }

	        /**** Step 8 - Close the file ****/
	        _fileSystemResult = f_close(&_fileObject);
	        if (_fileSystemResult != FR_OK)
	        {
	        	printf("f_close failed; cannot close the file !!\r\n");
	        	_res = CMN_FALSE;
				return _res;
	        }
		}
	}
	/**** Disconnect the SD Card ****/
	free(_writeBuf);
	cfgSdcDisconnect();
	return _res;
}

/**
 * @brief in CFG_parser_ih.h header
 */
void CFG_deinit(void){
  /* Nothing to Do */
}

/** ************************************************************************* */
