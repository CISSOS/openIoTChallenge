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
 * @defgroup PTD_portDriver GPIO
 * @brief GPIO Driver
 * @ingroup Peripheral
 *
 * @{
 * @brief  This module provides interface for GPIO
 * 
 * ****************************************************************************/
/* header definition ******************************************************** */
#ifndef BCDS_PTD_PORTDRIVER_IH_H_
#define BCDS_PTD_PORTDRIVER_IH_H_

/* public interface declaration ********************************************* */
#include <stdbool.h>

/* additional interface header files */
#include "em_gpio.h"

/* public type and macro definitions */

/** Define the prototype to which PTD driver callback functions must conform.
 *
 * Functions implementing serial transmission callbacks do not need to return, therefore their return type
 * is void none.
 *
 * Example task function prototype which conforms to the type declaration:
 * @code void EXL_exampleTxCallbackFunction(void); @endcode
 */
typedef void (*PTD_intrCallback)(void);

/**
 * This structure have the members to map Pin and corresponding
 * call back function to get called when the interrupt occurs
 */
typedef struct{
	uint32_t pin;
	PTD_intrCallback pinCallBackFunction;
}PTD_pinConfig_t;


typedef struct PTD_configInit_s{
    GPIO_Port_TypeDef	port;
	uint32_t	pin;
	GPIO_Mode_TypeDef	modeEnable;
	GPIO_Mode_TypeDef	modeDisable;
	uint16_t	doutEnable;
	uint16_t	doutDisable;
}PTD_configInit_t;


#define PTD_GET_PORT(M) ((GPIO_Port_TypeDef)PTD_PORT_##M)
#define PTD_GET_PIN(M) (PTD_PIN_##M)
#define PTD_GET_PORT_AND_PIN(M) (GPIO_Port_TypeDef)PTD_PORT_##M,PTD_PIN_##M
#define PTD_GET_PORT_AND_DRIVE_MODE(M) (GPIO_Port_TypeDef)PTD_PORT_##M,(GPIO_DriveMode_TypeDef)PTD_DRIVE_MODE_##M
#define PTD_GET_PORT_AND_MODE(M) (GPIO_Port_TypeDef)PTD_PORT_##M,(GPIO_Mode_TypeDef)PTD_MODE_##M
#define PTD_GET_PORT_PIN_DOUT(M) (GPIO_Port_TypeDef)PTD_PORT_##M,PTD_PIN_##M,PTD_DOUT_##M
#define PTD_GET_PORT_PIN_MODE_DOUT(M) (GPIO_Port_TypeDef)PTD_PORT_##M,PTD_PIN_##M,(GPIO_Mode_TypeDef)PTD_MODE_##M,PTD_DOUT_##M

/***************************************************************************//**
 * @brief
 *   Set a single pin in GPIO data out register to 1.
 *
 * @note
 *   In order for the setting to take effect on the output pad, the pin must
 *   have been configured properly. If not, it will take effect whenever the
 *   pin has been properly configured.
  ******************************************************************************/
#define PTD_pinOutSet(port, pin) GPIO_PinOutSet((GPIO_Port_TypeDef)port, pin)

/***************************************************************************//**
 * @brief
 *   Toggle a single pin in GPIO port data out register.
 *
 * @note
 *   In order for the setting to take effect on the output pad, the pin must
 *   have been configured properly. If not, it will take effect whenever the
 *   pin has been properly configured.
 ******************************************************************************/
#define PTD_pinOutToggle(port, pin) GPIO_PinOutToggle((GPIO_Port_TypeDef)port,pin)

/***************************************************************************//**
 * @brief
 *   Read the pad values for GPIO port.
 ******************************************************************************/
#define PTD_portInGet(port) GPIO_PortInGet((GPIO_Port_TypeDef)port)

/***************************************************************************//**
 * @brief
 *   Clear the pad values for GPIO port.
 ******************************************************************************/
#define PTD_portOutClear(port,pins) GPIO_PortOutClear((GPIO_Port_TypeDef)port,pins)

/***************************************************************************//**
 * @brief
 *   Get current setting for a GPIO port data out register.
 ******************************************************************************/
#define PTD_portOutGet(port) GPIO_PortOutGet((GPIO_Port_TypeDef)port)

/***************************************************************************//**
 * @brief
 *   Set bits GPIO data out register to 1.
 *
 * @note
 *   In order for the setting to take effect on the respective output pads, the
 *   pins must have been configured properly. If not, it will take effect
 *   whenever the pin has been properly configured.
 ******************************************************************************/
#define PTD_portOutSet(port,pins) GPIO_PortOutSet((GPIO_Port_TypeDef)port,pins)

/***************************************************************************//**
 * @brief
 *   Set GPIO port data out register.
 *
 * @note
 *   In order for the setting to take effect on the respective output pads, the
 *   pins must have been configured properly. If not, it will take effect
 *   whenever the pin has been properly configured.
 ******************************************************************************/
#define PTD_portOutSetVal(port,val,mask) GPIO_PortOutSetVal((GPIO_Port_TypeDef)port,val,mask)

/***************************************************************************//**
 * @brief
 *   Toggle a single pin in GPIO port data out register.
 *
 * @note
 *   In order for the setting to take effect on the output pad, the pin must
 *   have been configured properly. If not, it will take effect whenever the
 *   pin has been properly configured.
 ******************************************************************************/
#define PTD_portOutToggle(port,pins) GPIO_PortOutToggle((GPIO_Port_TypeDef)port,pins)

/***************************************************************************//**
 * @brief
 *   Unlocks the GPIO configuration.
 ******************************************************************************/
#define PTD_unlock() GPIO_Unlock()

/***************************************************************************//**
 * @brief
 *   Set a single pin in GPIO data out port register to 0.
 *
 * @note
 *   In order for the setting to take effect on the output pad, the pin must
 *   have been configured properly. If not, it will take effect whenever the
 *   pin has been properly configured.
 ******************************************************************************/
#define PTD_pinOutClear(port,pin) GPIO_PinOutClear((GPIO_Port_TypeDef)port,pin)

/***************************************************************************//**
 * @brief
 *   Get current setting for a pin in a GPIO port data out register.
 ******************************************************************************/
#define PTD_pinOutGet(port,pin) GPIO_PinOutGet((GPIO_Port_TypeDef)port,pin)

/***************************************************************************//**
 * @brief
 *   Read the pad value for a single pin in a GPIO port.
 ******************************************************************************/
#define PTD_pinInGet(port,pin) GPIO_PinInGet((GPIO_Port_TypeDef)port,pin)

/***************************************************************************//**
 * @brief
 *  This function is used to enable the interrupt by setting the
 *  specified bit with specified value.
 *
 * @details
 *    Perform bit-band operation on peripheral memory location by
 *    providing atomic read-modify-write cycle for single bit modification
 *******************************************************************************/
#define PTD_intEnable(bit) BITBAND_Peripheral(&(GPIO->IEN), bit, 1)

/***************************************************************************//**
 * @brief
 *  This function is used to disable the interrupt by setting the
 *  specified bit with specified value.
 *
 * @details
 *    Perform bit-band operation on peripheral memory location by
 *    providing atomic read-modify-write cycle for single bit modification
 *
 *******************************************************************************/
#define PTD_intDisable(bit) BITBAND_Peripheral(&(GPIO->IEN), bit, 0)

/***************************************************************************//**
 * @brief
 *   This function is used to clear the interrupt.
 *
 ******************************************************************************/
#define PTD_intClear(pin) GPIO_IntClear(1 << pin)

/***************************************************************************//**
 * @brief
 *   Read the pad value for a single pin in a GPIO port.
 ******************************************************************************/
#define PTD_intGet() GPIO_IntGet()

/***************************************************************************//**
 * @brief
 *   Get enabled and pending GPIO interrupt flags.
 *   Useful for handling more interrupt sources in the same interrupt handler.
 *
 * @note
 *   Interrupt flags are not cleared by the use of this function.
 ******************************************************************************/
#define PTD_intGetEnabled() GPIO_IntGetEnabled()

/**************************************************************************//**
 * @brief
 *   Set one or more pending GPIO interrupts from SW.
 *****************************************************************************/
#define PTD_intSet(flags) GPIO_IntSet(flags)

/***************************************************************************//**
 * @brief
 *   Locks the GPIO configuration.
 ******************************************************************************/
#define PTD_lock() GPIO_Lock()

/* public function prototype declarations */
/**
 * @brief
 *   This function is to have the initialization activities for PTD driver
 *   One of the initialization activity is to Enable/disable clock.
 *
 * @details
 *   After reset, all the module clocking will be disabled. During disabled time
 *   write to the peripheral will not any effects and reading will result in 
 *   unreliable value. Normally one should avoid accessing peripheral registers
 *   during disabled period of the clock
 *
 * @note
 *   Synchronization into the low frequency domain is required whenever
 *   enabling/disabling of LF clock. Stall will be occured if same register
 *   is modified before completion of previous instruction. For reducing the 
 *   stalling time in some usecase please refer CMU_FreezeEnable() 
 *
 */
void PTD_portInit(void);

/**
 * @brief       : Configuring the interrupt by enabling and disabling IRQs.
 *
 */
void PTD_portInteruptConfig(void);

/***************************************************************************//**
 * @brief
 *	 This function is used to configure the portpin interrupts.
 *
 * @details
 *   This function validates whether interrupts should be configured for odd or 
 *   even. In accordance to it call back function will be mapped
 *
 *   If is recommended to disable to the GPIO interrupt, if the interrupt is 
 *   already enables. see GPIO_Disable()for more information
 *
 *   Before enabling the interrupts, GPIO interrupt handler should be implemented
 *
 *   Pending interrupts to the selected pin will be cleared by this function
 *   function.
 *
 * @note
 *   If specific GPIO pin of a port is mapped to port pin, then same pin number
 *   can not be used by other ports. For example pin1 in port A is configured for
 *   interrupt, then the same pin can not be used. Please refer manual for more 
 *   details.
 *
 * @param[in] port
 *   The GPIO port to configured.
 *
 * @param[in] pin
 *   The pin number of the port to be configured.
 *
 * @param[in] risingEdge
 *   If interrupts should be configured for rising edge then it will be true, 
 *   otherwise false.
 *
 * @param[in] fallingEdge
 *   If interrupts should be configured for falling edge then it will be true, 
 *   otherwise false.
 *
 * @param[in] enable
 *   If it is set to true, then interrupts will be called after configuration
 *   if false it will be disabled. See GPIO_IntDisable() and GPIO_IntEnable().
 *
 * @param[in] callBack
 *   Whenever interrupts are triggered the mapped callback function will be 
 *   triggered.
 ******************************************************************************/
void PTD_intConfig(GPIO_Port_TypeDef port, uint32_t pin, bool risingEdge,
		           bool fallingEdge, bool enable, PTD_intrCallback callBack);

/***************************************************************************//**
 * @brief
 *   This function is used to switch the callback routine for already registered
 *   interrupt configuration.
 *
 * @param [in] pin
 *   The pin number in the port.
 *
 * @param [in] callBack
 *   callback function have to be mapped to the pin which will be get called
 *   when interrupt occurs.
 * @returns
 *   This routine returns PTD_SUCCESS, PTD_FAILIRE, PTD_INVALID_PARAMETER
 ******************************************************************************/
int32_t PTD_interruptSwitchCallback(uint32_t pin, PTD_intrCallback callBack);

/***************************************************************************//**
 * @brief
 *  This function is used to set the number of Even and Odd interrupt pins that
 *  will be registered using PTD_intConfig().
 *
 * @details
 *   The number of even and odd interrupt pins have to be set to dynamically
 *   allocate the memory for the pin and call back function.
 *
 * @param[in] even_pins
 *   The number of even pins to set.
 *
 * @param[in] odd_pins
 *   The number of odd pin to set.
 *
 * @warning
 *   This function will allocate memory dynamically once.
 *   Since the allocated memory required until, the software runs,
 *   this memory will not be freed. This allocation has been limited
 *   to once by using a software flag.
  ******************************************************************************/
void PTD_setInterruptPinCount(uint8_t even_pins,uint8_t odd_pins);

/***************************************************************************//**
 * @brief
 *   Set the mode for a GPIO pin.
 *
 * @param[in] port
 *   The GPIO port to access.
 *
 * @param[in] pin
 *   The pin number in the port.
 *
 * @param[in] mode
 *   The desired pin mode.
 *
 * @param[in] out
 *   Value to set for pin in DOUT register. The DOUT setting is important for
 *   even some input mode configurations, determining pull-up/down direction.
 ******************************************************************************/
void PTD_pinModeSet(GPIO_Port_TypeDef port, uint16_t pin,GPIO_Mode_TypeDef mode, uint16_t out) __asm("GPIO_PinModeSet");

/***************************************************************************//**
 * @brief
 *   Set the drive mode for a GPIO pin.
 *
 * @param[in] port
 *   The GPIO port to access.
 *
 * @param[in] mode
 *   The desired pin mode.
 *
 ******************************************************************************/
void PTD_driveModeSet(GPIO_Port_TypeDef port, GPIO_DriveMode_TypeDef mode) __asm("GPIO_DriveModeSet");

/* public global variable declarations */

/* inline function definitions */

#endif /* BCDS_PTD_PORTDRIVER_IH_H_*/

/**@}*/