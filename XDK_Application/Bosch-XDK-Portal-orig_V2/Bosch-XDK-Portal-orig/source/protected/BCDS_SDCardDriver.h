/**
* Licensee agrees that the example code provided to Licensee has been developed and released by Bosch solely as an example to be used as a potential reference for Licensee’s application development. 
* Fitness and suitability of the example code for any use within Licensee’s applications need to be verified by Licensee on its own authority by taking appropriate state of the art actions and measures (e.g. by means of quality assurance measures).
* Licensee shall be responsible for conducting the development of its applications as well as integration of parts of the example code into such applications, taking into account the state of the art of technology and any statutory regulations and provisions applicable for such applications. Compliance with the functional system requirements and testing there of (including validation of information/data security aspects and functional safety) and release shall be solely incumbent upon Licensee. 
* For the avoidance of doubt, Licensee shall be responsible and fully liable for the applications and any distribution of such applications into the market.
* 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are 
* met:
* 
*     (1) Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer. 
* 
*     (2) Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in
*     the documentation and/or other materials provided with the
*     distribution.  
*     
*     (3)The name of the author may not be used to
*     endorse or promote products derived from this software without
*     specific prior written permission.
* 
*  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR 
*  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
*  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
*  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
*  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
*  POSSIBILITY OF SUCH DAMAGE.
*/
/**
 * @defgroup sdCardDriver sdCardDriver
 * @ingroup SDC
 *
 * @ingroup sdCardDriver
 *
 * @{
 * @brief  Driver for the SD Card
 *  \tableofcontents
 *  \section intro_sec SDC
 * SDC Interface layer API
 */

/* header definition ******************************************************** */
#ifndef BCDS_SDCARDDRIVER_H_
#define BCDS_SDCARDDRIVER_H_

#include "BCDS_MiscDrivers.h"
#if BCDS_FEATURE_SDCARD

/* public interface declaration ********************************************* */
#include "BCDS_Retcode.h"

/* public type and macro definitions */
/**
 * @note The following type casting has bee done based on the
 * 		  data type of the where the macros has been used
 */
#define SDCARD_LEVEL_SHIFT_ON				UINT16_C(0)
#define SDCARD_LEVEL_SHIFT_OFF				UINT16_C(1)
#define SDCARD_SPI_MODE					    UINT16_C(0)
#define SDCARD_SPI_LOCATION				    UINT16_C(1)
#define SDCARD_SPI_LOW_BITRATE				UINT32_C(100000)
#define SDCARD_SPI_HIGH_BITRATE             UINT32_C(12000000)
#define SDCARD_DETECT_ENABLE   			    UINT8_C(1)
#define SDCARD_MILLI_SECOND_VALUE           (SDCARD_SPI_HIGH_BITRATE / UINT16_C(8000))

/**
 *  @brief Return status for operations using corresponding Retcodes
 */
enum SDCardDriver_ErrorCode_E
{
    SDCARD_STATUS_INITIALIZED = 0x00, /**SD-card initialization status success */
    SDCARD_RW_FAIL,              /**SD-card Read or write Failure */
    SDCARD_DISK_WRITE_PROTECTED, /** SD-card checking whether in Protected state */
    SDCARD_DISK_NOT_READY,       /** SD-card disk was not ready */
    SDCARD_NO_DISK,              /** SD-card no disk */
    SDCARD_DISK_INVALID_PARAMETER,/** Validation for the SD-card **/
    SDCARD_SPI_INIT_FAILED,       /** SD-card SPI initialization failed */
    SDCARD_SPI_BUSY              /** SD-card SPI status busy or ready*/
};
typedef enum SDCardDriver_ErrorCode_E SDCardDriver_ErrorCode_T;

/**
 *  @brief SD card status returned by APIs of SD card driver whether initialized or not
 */
enum SDCardDriver_DiskStatus_E
{ /**< Error return values*/
    SDCARD_INITIALIZED = UINT8_C(0x00),/**SD-card initialization success */
    SDCARD_NOT_INITIALIZED = UINT8_C(0x01),/**SD-card initialization not done */
};
typedef enum SDCardDriver_DiskStatus_E SDCardDriver_DiskStatus_T;

/**
 *  @brief SD card status returned by APIs of SD card driver
 */
enum SDCardDriver_Status_E
{ /**< Error return values*/
    SDCARD_NOT_INSERTED, /**SD-card was not inserted */
    SDCARD_INSERTED /**SD-card was inserted */
};
typedef enum SDCardDriver_Status_E SDCardDriver_Status_T;

/* public interface declaration ********************************************* */
/**
 * @brief
 *      The SDCardDriver_Init API is used to initialize the SD card module.
 *      The initialization does the following:
 *      (1) SD Card related Port, Pin, mode and dout settings.
 *      (2) Enables the clock.
 *      (3) Registers the call back function which will be triggered
 *          when card inserted or removed.
 *      (4) SPI initialization with SD card related parameters.
 *
 * @retval
 * 		RETCODE_FAILURE - SD card initialization failed.
 *
 * @retval
 * 		RETCODE_SUCCESS - SD card Initialization success.
 */
Retcode_T SDCardDriver_Init(void);

/**
 * @brief
 *      The SDCardDriver_GetDetectStatus API is used to get the status
 *      of the SD card whether the SD card has been inserted or removed.
 *
 * @retval
 * 		SDCARD_INSERTED - SD card is present.
 *
 * @retval
 * 		SDCARD_NOT_INSERTED - SD card is not present.
 */
SDCardDriver_Status_T SDCardDriver_GetDetectStatus(void);

/**
 * @brief
 *      The SDC_sdCardDisConnect API is used to disconnect the SD card communication
 *      from controller and release the SPI bus.
 */
void SDCardDriver_Disconnect(void);

/**
 * @brief
 *      The SDC_sdCardDisConnect API is used to disconnect the SD card communication
 *      from controller and release the SPI bus.
 *
 * @retval
 * 		RETCODE_SUCCESS - SD card communication is enabled.
 *
 * @retval
 * 		RETCODE_FAILURE - SD card communication is Disabled.
 */
Retcode_T SDCardDriver_Connect(void);

/**
 * @brief
 *      The SDCardDriver_DiskInitialize API is used to initialize SD-card Disk sector.
 *      The initialization does the following:
 *      (1) verify the type of disk and card or disk status.
 *      (2) Enables the clock for SD card communication.
 *      (3) validating the Disk by passing commands and expected the responds.
 *
 * @param[in]
 * 		sdDrive - physical drive number.
 *
 * @retval
 * 		RETCODE_FAILURE - SD disk initialization failed.
 *
 * @retval
 * 		RETCODE_SUCCESS - Initialization success.
 */
Retcode_T SDCardDriver_DiskInitialize(uint8_t sdDrive);

/**
 * @brief
 *		  The SDCardDriver_DiskWrite API is used to write data into SD card after
 *		  Disk Initialization has success.
 *
 * @param[in] drive
 *                  Physical drive number (0)- Supports only single drive
 *
 * @param[in] writeBuffer
 *                  Data pointer to write data into Disk.
 *
 * @param[in] sector
 *                  Start sector number (Logical Block addressing(LBA)) to write into Disk.
 *
 * @param[in] writeCount
 *                  Write Sector count (1 to 255).
 *
 * @retval  RETCODE_FAILURE
 *  		       SD card R/W Error.
 *
 * @retval  RETCODE_SUCCESS
 *  		       SD card R/W succeed.
 *
 * @retval  SDCARD_DISK_WRITE_PROTECTED
 *  		       SD card write Protected.
 *
 * @retval  SDCARD_DISK_NOT_READY
 *  		       SD card is Not Ready.
 *
 * @retval  RETCODE_INVALIDPARAM
 *  		       Invalid parameter.
 */
Retcode_T SDCardDriver_DiskWrite(uint8_t drive, const uint8_t *writeBuffer,
        uint32_t sector, uint32_t writeCount);

/**
 * @brief
 *		  The SDCardDriver_DiskRead API is used to read data from SD card.
 *
 * @param[in] drive
 *                  Physical drive number (0)- Supports only single drive
 *
 * @param[in] readBuffer
 *                  Data pointer to store received data from Disk.
 *
 * @param[in] sector
 *                  Start sector number (Logical Block addressing(LBA)) to read.
 *
 * @param[in] ReadCount
 *                  Read Sector count (1 to 255).
 *
 * @retval  RETCODE_FAILURE
 *  		       SD card R/W Error.
 *
 * @retval  RETCODE_SUCCESS
 *  		       SD card R/W succeed.
 *
 * @retval  SDCARD_DISK_WRITE_PROTECTED
 *  		       SD card write Protected.
 *
 * @retval  SDCARD_DISK_NOT_READY
 *  		       SD card is Not Ready.
 *
 * @retval  RETCODE_INVALIDPARAM
 *  		       Invalid parameter.
 */
Retcode_T SDCardDriver_DiskRead(uint8_t drive, uint8_t *readBuffer,
        uint32_t sector, uint32_t readCount);

/**
 * @brief
 *		  The SDCardDriver_DiskIoctl API is used to have the functionalities to get
 *		  sector count, sector size, block size. It also used to invalidate
 *		  the disk initialization.
 *
 * @param[in] drive
 *                  Physical drive number (0)- Supports only single drive
 *
 * @param[in] control
 *                  Control code to specify the functionality
 *
 * @param[in] buffer
 *                  Buffer to send/receive data block
 *
 * @retval  RETCODE_SUCCESS
 *  		       specified control functionality success
 *
 * @retval  RETCODE_FAILURE
 *  		       specified control functionality failed
 *
 * @retval  SDCARD_DISK_NOT_READY
 *  		       SD card is Not Ready.
 *
 * @retval  RETCODE_INVALIDPARAM
 *  		       Invalid parameter.
 */
Retcode_T SDCardDriver_DiskIoctl(uint8_t drive, uint8_t control, void *buffer);

/**
 * @brief
 *		  The SDCardDriver_GetDiskStatus API is used to get the disk initialization status
 * @param[in]
 * 		drive - Physical drive nmuber (0)
 * @retval
 *		SDCARD_STATUS_NOT_INITIALIZED - Disk not initialized yet
 * @retval
 *		SDCARD_STATUS_INITIALIZED - Disk initialized
 */
Retcode_T SDCardDriver_GetDiskStatus(uint8_t drive);

/**
 * @brief
 *   This function is required by the FAT file system in order to provide
 *   timestamps for created files. Since this example does not include a
 *   reliable clock we hardcode a value here.
 *
 *   Refer to drivers/fatfs/doc/en/fattime.html for the format of this uint32_t.
 * @retval
 *    A uint32_t containing the current time and date as a packed datastructure.
 */
unsigned long get_fattime(void);

/* public global variable declarations */

/* inline function definitions */

#endif /* #if BCDS_FEATURE_SDCARD */

/**@}*/
#endif /* BCDS_SDCARDDRIVER_H_ */

/****************************************************************************/

