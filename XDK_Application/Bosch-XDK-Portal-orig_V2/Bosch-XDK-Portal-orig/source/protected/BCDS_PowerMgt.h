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
/*----------------------------------------------------------------------------*/

/**
 *  Public interface header for RTOS wrapper and feature
 *  extender library for FreeRTOS and OpenRTOS.
 *
 *  The PowerMgt_SuppressTicksAndSleep() API implements the low power tickless feature.
 *  On FreeRTOS and OpenRTOS based systems the API needs to be mapped to the
 *  portSUPPRESS_TICKS_AND_SLEEP() macro in FreeRTOSConfig.h. The API should not
 *  be called by the application writers.
 */

#ifndef BCDS_POWERMGT_H
#define BCDS_POWERMGT_H

/* Include all headers which are needed by this file. */

#include "BCDS_Basics.h"

/* Put the type and macro definitions here */
#define POWER_MGT_SYSTEM_CLOCK_FREQUENCY        32768U /**< System timer clock frequency is 32.768 kHz */
#define POWER_MGT_SYSTEM_TICK_RATE_HZ           1000U /**< System timer targeted period time is 1 millisecond(s) */

/** Type representing the supported, run-time configurable operation modes.
 */
enum PowerMgt_SleepMode_E
{
    Normal_Mode = 0, /**< Normal operation mode (EM0) */
    Sleep_Mode = 1, /**< Sleep mode (EM1) */
    DeepSleep_Mode = 2, /**< Deep sleep mode (EM2) */
    Stop_Mode = 3, /**< Stop mode (EM3) */
    Max_Mode_Limit
};
typedef enum PowerMgt_SleepMode_E PowerMgt_SleepMode_T;

typedef enum PowerMgt_SleepMode_E *PowerMgt_SleepModePtr_T;

/* Put the function declarations here */

/** Retrieve the system time, in seconds.
 *
 * @return  Time elapsed since the RTOS scheduler was started in seconds.
 */
uint32_t PowerMgt_GetSystemTime(void);

/** Retrieve the system time, in milliseconds.
 *
 * @return Time elapsed since the RTOS scheduler was started in milliseconds.
 */
uint64_t PowerMgt_GetSystemTimeMs(void);

/** Block the requested operation mode.
 *
 * The API implements nest counters to track the number of block and unblock
 * requests.
 *
 * For example to allow only the Normal_Mode operation mode, it is sufficient
 * to block Sleep_Mode as it implicitly blocks DeepSleep_Mode too.
 *
 * Every call to the API should be aligned with a call to PowerMgt_SleepBlockEnd().
 *
 * The nest counter of each operation mode support a depth level of 255.
 *
 * @param operationModeToBlock One of the enum members from PowerMgt_SleepMode_T.
 */
void PowerMgt_SleepBlockBegin(PowerMgt_SleepMode_T operationModeToBlock);

/** Request unblocking of an operation mode.
 *
 * The API implements nest counters to track the number of block and unblock
 * requests.
 *
 * For example if DeepSleep_Mode was requested to be blocked twice,
 * a single call to the API will only decrease the related nest counter, but
 * will not unblock the operation mode which means it will still not be possible
 * to enter it.
 *
 * Every call to the API should be aligned with a call to PowerMgt_SleepBlockBegin().
 *
 * The nest counter of each operation mode support a depth level of 255.
 *
 * @param operationModeToUnblock
 */
void PowerMgt_SleepBlockEnd(PowerMgt_SleepMode_T operationModeToUnblock);

/** The function like macro converts milliseconds to internal system ticks resolution.
 *
 * The macro only accepts literal values so that the compiler's preprocessor
 * could perform all the calculations at compilation time.
 *
 * @param[in] milliseconds Literal value representing time in milliseconds.
 *
 * @return Number of system ticks corresponding to the given input time in milliseconds.
 */
#define PowerMgt_GetMsDelayTimeInSystemTicks(milliseconds) \
    ((UINT32_C(milliseconds))*(POWER_MGT_SYSTEM_CLOCK_FREQUENCY))/(((POWER_MGT_SYSTEM_CLOCK_FREQUENCY / POWER_MGT_SYSTEM_TICK_RATE_HZ))*1000U)

/** The function transitions the system into an available low power state for
 * the requested time frame and handles scheduled, planned wake events as well
 * as external event driven wake events.
 *
 * The API should not be called by the application writers, but it needs to be
 * mapped by the RTOS.
 *
 * @param expectedWaitTime Number of RTOS system ticks to be skipped in
 *        tickless mode by the kernel.
 *
 * Note : This function is used for RTC based Energy mode
 */
void PowerMgt_SuppressTicksAndSleep(uint32_t expectedWaitTime);

/** The function transitions the system into an available low power state for
 * the requested time frame and handles scheduled, planned wake events as well
 * as external event driven wake events.
 *
 * The API should not be called by the application writers, but it needs to be
 * mapped by the RTOS.
 *
 * @param expectedWaitTime Number of RTOS system ticks to be skipped in
 *        tickless mode by the kernel.
 *
 * Note : This function is used for BURTC based Energy mode
 */
void PowerMgt_SuppressTicksAndSleep64(uint64_t expectedWaitTime);

#endif /* BCDS_POWERMGT_H */

