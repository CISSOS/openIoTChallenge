/******************************************************************************
**	COPYRIGHT (c) 2016		Bosch Connected Devices and Solutions GmbH
**
**	The use of this software is subject to the XDK SDK EULA
**
*******************************************************************************
**
**	OBJECT NAME:	bxpConfig.h
**
**	DESCRIPTION:	Local Header for bxpConfig.c source file
**
**	PURPOSE:        Contains the local macro, typedef, variables and function
**	                definitions for the CFG module
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
#ifndef _BXP_CONFIG_H_
#define _BXP_CONFIG_H_

/* local interface declaration ********************************************** */

/* other header files */
#include "jsmn.h"

/* local type and macro definitions */

// SD Card Configuration
#define CFG_DRIVE_ZERO				    UINT8_C(0)         /**< SD Card Drive 0 location */
#define CFG_DETECT_SD_CARD_INSERTED 	UINT8_C(1)         /**< Check for SD Card Insertion Status */
#define CFG_SEEK_FIRST_LOCATION		    UINT8_C(0)         /**< File seek to the first location */
#define CFG_MAX_LINE_SIZE   	   		50                 /**< Max Line size of the configuration files */
#define CFG_FILE_READ_BUFFER_SIZE       256                /**< Size of the read buffer */
#define CFG_NETWORK_FILE_NAME           "NCONFIG.txt"    /**< Network configuration file name on the SD Card */
#define CFG_DATA_FILE_NAME              "DCONFIG.txt"    /**< Data configuration file name on the SD Card */
#define CFG_MAX_TOKENS                  64                 /**< Max number of tokens fron the JSON file */

#define CFG_DELAY_MOUNT                  0
#define CFG_MOUNT_NOW                    1
#define CFG_DEFAULT_LOGICAL_DRIVE        ""

typedef enum jsmnerr jsmnerr_t;

// Configuration array's cell elements
typedef struct {
    char *attName;        /**< Attribute name at the configuration file */
    char *attValue;       /**< Attribute value  at the configuration file */
    uint8_t   defined;    /**< To specify if the attribute has been read from the configuration file */
} configLine_t,*configLine_tp;

// Network configuration array index
typedef enum {
    ATT_IDX_NET_SSID,
    ATT_IDX_NET_PWD,
    ATT_IDX_BROKER,
	ATT_IDX_PORT,
    ATT_NET_SIZE,
} attributesNetIndex_t;

// Nata configuration array index
typedef enum {
    ATT_IDX_GID,
    ATT_IDX_CONF,
    ATT_IDX_REP,
    ATT_IDX_STRM,
	ATT_IDX_TEMP,
	ATT_IDX_HUM,
	ATT_IDX_LIGHT,
	ATT_IDX_PRES,
	ATT_IDX_ACCEL,
	ATT_IDX_GYRO,
	ATT_IDX_MAG,
	ATT_IDX_JSON,
    ATT_DATA_SIZE,
} attributesDataIndex_t;

// Configuration file name array's cell elements
typedef struct {
    char *cfgFileName;        /* Configuration file Name */
} configFile_t,*configFile_tp;

// Configuration file name array index
typedef enum {
    CFG_IDX_NET ,
    CFG_IDX_DATA ,
	CFG_IDX_SIZE ,
} configFileIndex_t;

// SD Card's Configuration File's Token Index
typedef enum {
	CFG_TOKEN_TYPE_UNKNOWN,
    CFG_TOKEN_EOF,
    CFG_TOKEN_ATT_NAME,
    CFG_TOKEN_EQUAL,
    CFG_TOKEN_VALUE,
} tokensType_t;

// The states in the configuration file parser states machine
typedef enum {
    STATE_EXP_ATT_NONE,
    STATE_EXP_ATT_NAME,
    STATE_EXP_ATT_EQUAL,
    STATE_EXP_ATT_VALUE,
} states_t;

/* local module variable declarations */

// SD Card variables
static SDCardDriver_DiskStatus_T      s_cfgDiskInitStatus = SDCARD_NOT_INITIALIZED;
static uint8_t		         s_cfgFileReadBuffer[CFG_FILE_READ_BUFFER_SIZE];

// Configuration File Names
static char s_cfgNetworkName[]	=	CFG_NETWORK_FILE_NAME;    /**< Network Configuration file name */
static char s_cfgDataName[]	    =	CFG_DATA_FILE_NAME;       /**< Data Configuration file name */

// Configuration file holder structure array
static configFile_t s_fconfig[] = {
		 {s_cfgNetworkName},
		 {s_cfgDataName}
};

/* local module function declarations */

/**
 * @brief Parse the JSON token buffer; Store Value in local Variables
 *
 * @param[in] buffer
 *            The buffer containg the JSON file;
 *            token[1] is the first attribute name;
 *            subsequently, odd numbered tokens are names
 *            and even numbered tokens are values
 * @param[in] token
 *            Buffer containing the structure of token parameters
 * @param[in] numTokens
 *            Number of Tokens in the JSON file
 *
 * @return CMN_TRUE if the tokens are correctly parsed
 *
 */
static uint8_t cfgJsonTokenParser(const char *buffer, jsmntok_t *token, jsmnerr_t numTokens);

/**
 * @Brief extracts tokens from the input buffer, copies into the token buffer and returns its
 * size at tokensize
 *
 * @param[in] buffer
 *            The input buffer
 * @param[in] idxAtBuffer
 *            The input buffer index
 * @param[in] bufSize
 *            The size of the input buffer
 * @param[in] fileIndex
 *            The index of the file being parsed
 *
 * @param[out] token
 *            The buffer that will contain the token values
 * @param[out] tokenSize
 *            The pointer to the variable that receives the size of the token
 *
 * @return the TOKEN types found (TOKEN_NONE is EOF)
 */
static tokensType_t cfgGetToken(const char *buffer,
						uint16_t *idxAtBuffer,
						uint16_t bufSize,
						char *token,
						uint16_t *tokenSize,
						int8_t fileIndex);

/**
 * @Brief Parse the config file read into the buffer
 *
 * @param[in] buffer
 *            The buffer containing the configuration file
 * @param[in] bufSize
 *            The size of the buffer
 * @param[in] fileIndex
 *            The index of the file being parsed
 *
 * @return CMN_TRUE if configuration file is correct and contains necessary attribute/values
 *
 */
static uint8_t cfgParseSDConfigFile(const char *buffer, uint16_t bufSize, int8_t fileIndex);

/**
 * @brief Disconnect from the SD Card
 */
static void cfgSdcDisconnect(void);

/**
 * @Brief Read the configuration files from the SD Card
 *
*/
static void cfgReadSDConfigFiles(void);


/* local inline function definitions */

#endif /* _BXP_CONFIG_H_ */

/** ************************************************************************* */
