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
 * @defgroup PinAssignments PinAssignments
 * @brief GPIO Pin Assignments
 * @ingroup Configuration
 *
 * @{
 * @brief  This contains the XDK C1 sample Hardware pin configuration details.
 *
 * ****************************************************************************/

/* header definition ******************************************************** */
#ifndef XDK110_PTD_PORTDRIVER_PH_H_
#define XDK110_PTD_PORTDRIVER_PH_H_

/* public interface declaration ********************************************* */

/* public type and macro definitions */
#define PTD_GPIO_LOW         				 0
#define PTD_GPIO_HIGH         				 1
#define PTD_GPIO_ON          				 1
#define PTD_GPIO_OFF                         0
#define PTD_GPIO_PULLUP                      1
#define PTD_GPIO_PULLDOWN                    0
#define PTD_GPIO_INPUT_FILTER_ON             1
#define PTD_GPIO_INPUT_FILTER_OFF            0

/** push button 1 pin configuration */
#define PTD_PORT_PUSH_BUTTON_1             gpioPortE
#define PTD_PIN_PUSH_BUTTON_1              9
#define PTD_MODE_PUSH_BUTTON_1             gpioModeInputPullFilter
#define PTD_DOUT_PUSH_BUTTON_1             PTD_GPIO_PULLUP

/** push button 2 pin configuration */
#define PTD_PORT_PUSH_BUTTON_2             gpioPortE
#define PTD_PIN_PUSH_BUTTON_2              14
#define PTD_MODE_PUSH_BUTTON_2             gpioModeInputPullFilter
#define PTD_DOUT_PUSH_BUTTON_2             PTD_GPIO_PULLUP

/** ORANGE LED pin configuration */
#define PTD_PORT_LED_ORANGE             gpioPortB
#define PTD_PIN_LED_ORANGE              1
#define PTD_MODE_LED_ORANGE             gpioModePushPull
#define PTD_DOUT_LED_ORANGE             PTD_GPIO_OFF

/** YELLOW LED pin configuration */
#define PTD_PORT_LED_YELLOW             gpioPortB
#define PTD_PIN_LED_YELLOW              0
#define PTD_MODE_LED_YELLOW             gpioModePushPull
#define PTD_DOUT_LED_YELLOW             PTD_GPIO_OFF

/** RED LED pin configuration */
#define PTD_PORT_LED_RED             	gpioPortA
#define PTD_PIN_LED_RED                 12
#define PTD_MODE_LED_RED                gpioModePushPull
#define PTD_DOUT_LED_RED                PTD_GPIO_OFF

/** SPI MOSI pin configuration for WIFI module */
#define PTD_PORT_WIFI_MOSI              gpioPortE
#define PTD_PIN_WIFI_MOSI               10
#define PTD_MODE_WIFI_MOSI              gpioModePushPull
#define PTD_DOUT_WIFI_MOSI              PTD_GPIO_LOW

/** SPI MISO pin configuration for WIFI module */
#define PTD_PORT_WIFI_MISO              gpioPortE
#define PTD_PIN_WIFI_MISO               11
#define PTD_MODE_WIFI_MISO              gpioModeInput
#define PTD_DOUT_WIFI_MISO              PTD_GPIO_INPUT_FILTER_OFF

/** SPI Chip Select pin configuration for WIFI module */
#define PTD_PORT_WIFI_CS                gpioPortC
#define PTD_PIN_WIFI_CS                 5
#define PTD_MODE_WIFI_CS                gpioModeWiredAnd
#define PTD_DOUT_WIFI_CS                PTD_GPIO_LOW

/** SPI clock pin configuration for WIFI module */
#define PTD_PORT_WIFI_CLK               gpioPortE
#define PTD_PIN_WIFI_CLK                12
#define PTD_MODE_WIFI_CLK               gpioModePushPull
#define PTD_DOUT_WIFI_CLK               PTD_GPIO_LOW

/** Interrupt pin configuration for WIFI module */
#define PTD_PORT_WIFI_IRQ               gpioPortA
#define PTD_PIN_WIFI_IRQ                10
#define PTD_MODE_WIFI_IRQ               gpioModeInput
#define PTD_DOUT_WIFI_IRQ               PTD_GPIO_INPUT_FILTER_ON

/** Enable/Disable pin configuration for WIFI module */
#define PTD_PORT_WIFI_ENABLE_DISABLE    gpioPortA
#define PTD_PIN_WIFI_ENABLE_DISABLE     11
#define PTD_MODE_WIFI_ENABLE_DISABLE    gpioModeWiredOrPullDown
#define PTD_DOUT_WIFI_ENABLE_DISABLE    PTD_GPIO_LOW

/** Reset pin configuration for WIFI module */
#define PTD_PORT_WIFI_RESET             gpioPortA
#define PTD_PIN_WIFI_RESET              15
#define PTD_MODE_WIFI_RESET             gpioModeWiredAnd
#define PTD_DOUT_WIFI_RESET             PTD_GPIO_LOW       

/** 3V3 VDD enable port pin configuration for WIFI module and SD card */
#define PTD_PORT_WIFI_SD_VDD_PORT        gpioPortC
#define PTD_PIN_WIFI_SD_VDD_PORT         11
#define PTD_MODE_WIFI_SD_VDD_PORT        gpioModeWiredOr
#define PTD_DOUT_WIFI_SD_VDD_PORT        PTD_GPIO_HIGH  

/** 2V5 VDD enable pin configuration for WIFI module - default value PTD_GPIO_HIGH means disabled during intitalization */
#define PTD_PORT_WIFI_VDD_2V5_ENABLE     gpioPortA
#define PTD_PIN_WIFI_VDD_2V5_ENABLE      8
#define PTD_MODE_WIFI_VDD_2V5_ENABLE     gpioModeWiredAnd
#define PTD_DOUT_WIFI_VDD_2V5_ENABLE     PTD_GPIO_HIGH

/** 2V5 VDD SNOOZE control pin configuration for WIFI module - default value PTD_GPIO_LOW means snooze mode disabled during intitalization*/
#define PTD_PORT_WIFI_VDD_2V5_SNOOZE     gpioPortF
#define PTD_PIN_WIFI_VDD_2V5_SNOOZE      5
#define PTD_MODE_WIFI_VDD_2V5_SNOOZE     gpioModeWiredOr
#define PTD_DOUT_WIFI_VDD_2V5_SNOOZE     PTD_GPIO_LOW

/** UART TX pin configuration for BLE module */
#define PTD_PORT_BTLE_TX              gpioPortE
#define PTD_PIN_BTLE_TX               0
#define PTD_MODE_BTLE_TX              gpioModePushPull
#define PTD_DRIVE_MODE_BTLE_TX        gpioDriveModeLow
#define PTD_DOUT_BTLE_TX              PTD_GPIO_HIGH

/** UART RX pin configuration for BLE module */
#define PTD_PORT_BTLE_RX              gpioPortE
#define PTD_PIN_BTLE_RX               1
#define PTD_MODE_BTLE_RX              gpioModeInput
#define PTD_DRIVE_MODE_BTLE_RX        gpioDriveModeLow
#define PTD_DOUT_BTLE_RX              PTD_GPIO_INPUT_FILTER_OFF

/** WAKEUP pin configuration for BLE module */
#define PTD_PORT_BTLE_WAKEUP            gpioPortD
#define PTD_PIN_BTLE_WAKEUP             10
#define PTD_MODE_BTLE_WAKEUP            gpioModeWiredOr
#define PTD_DRIVE_MODE_BTLE_WAKEUP      gpioDriveModeHigh
#define PTD_DOUT_BTLE_WAKEUP            PTD_GPIO_LOW

/** RESET pin configuration for BTLE module */
#define PTD_PORT_BTLE_RESET             gpioPortA
#define PTD_PIN_BTLE_RESET              9
#define PTD_MODE_BTLE_RESET             gpioModeWiredOr
#define PTD_DRIVE_MODE_BTLE_RESET       gpioDriveModeStandard
#define PTD_DOUT_BTLE_RESET             PTD_GPIO_LOW

/** Interrupt pin for BTLE */
#define PTD_PORT_BTLE_INT               gpioPortD
#define PTD_PIN_BTLE_INT                11
#define PTD_MODE_BTLE_INT               gpioModeDisabled
#define PTD_DOUT_BTLE_INT               PTD_GPIO_INPUT_FILTER_OFF

/** DATA pin for I2C */
#define PTD_PORT_I2C0_SDA2               gpioPortD
#define PTD_PIN_I2C0_SDA2                14
#define PTD_MODE_I2C0_SDA2               gpioModeWiredAnd
#define PTD_DOUT_I2C0_SDA2               PTD_GPIO_HIGH

/** CLOCK pin for I2C */
#define PTD_PORT_I2C0_SCL2               gpioPortD
#define PTD_PIN_I2C0_SCL2                15
#define PTD_MODE_I2C0_SCL2               gpioModeWiredAnd
#define PTD_DOUT_I2C0_SCL2               PTD_GPIO_HIGH

/** Interrupt pin for BMA280 Interrupt 1 */
#define PTD_PORT_BMA280_INT1             gpioPortF
#define PTD_PIN_BMA280_INT1              8
#define PTD_MODE_BMA280_INT1             gpioModeInput
#define PTD_DOUT_BMA280_INT1             PTD_GPIO_INPUT_FILTER_ON

/** Interrupt pin for BMA280 Interrupt 2 */
#define PTD_PORT_BMA280_INT2             gpioPortF
#define PTD_PIN_BMA280_INT2              9
#define PTD_MODE_BMA280_INT2             gpioModeInput
#define PTD_DOUT_BMA280_INT2             PTD_GPIO_INPUT_FILTER_ON

/** VDD pin configuration for BMA280 Module */
#define PTD_PORT_BMA280_VDD             gpioPortE
#define PTD_PIN_BMA280_VDD              7
#define PTD_MODE_BMA280_VDD             gpioModePushPullDrive
#define PTD_DOUT_BMA280_VDD             PTD_GPIO_LOW

/** VDD pin configuration for BMA280 Module */
#define PTD_PORT_BME280_VDD             gpioPortE
#define PTD_PIN_BME280_VDD              8
#define PTD_MODE_BME280_VDD             gpioModePushPullDrive
#define PTD_DOUT_BME280_VDD             PTD_GPIO_LOW

/** SD card Level shifter pin */
#define PTD_PORT_SDC_LEVEL_SHIFT		gpioPortC
#define PTD_PIN_SDC_LEVEL_SHIFT			7
#define PTD_MODE_SDC_LEVEL_SHIFT		gpioModeWiredAnd
#define PTD_DOUT_SDC_LEVEL_SHIFT        PTD_GPIO_HIGH

/** SD card detect pin */
#define PTD_PORT_SDC_PRESENT			gpioPortF
#define PTD_PIN_SDC_PRESENT				12
#define PTD_MODE_SDC_PRESENT			gpioModeInputPullFilter
#define PTD_DOUT_SDC_PRESENT			PTD_GPIO_HIGH

/** SD card chip select pin */
#define PTD_PORT_SDC_CS					gpioPortB
#define PTD_PIN_SDC_CS					15
#define PTD_MODE_SDC_CS					gpioModeWiredAnd
#define PTD_DRIVE_MODE_SDC_CS           gpioDriveModeStandard
#define PTD_DOUT_SDC_CS					PTD_GPIO_HIGH

/** SPI MOSI pin configuration for SD CARD module */
#define PTD_PORT_SDC_MOSI             	gpioPortD
#define PTD_PIN_SDC_MOSI              	0
#define PTD_MODE_SDC_MOSI             	gpioModePushPull
#define PTD_DRIVE_MODE_SDC_MOSI         gpioDriveModeHigh
#define PTD_DOUT_SDC_MOSI             	PTD_GPIO_LOW

/** SPI MISO pin configuration for SD CARD module */
#define PTD_PORT_SDC_MISO             	gpioPortD
#define PTD_PIN_SDC_MISO              	1
#define PTD_MODE_SDC_MISO             	gpioModeInput
#define PTD_DRIVE_MODE_SDC_MISO         gpioDriveModeHigh
#define PTD_DOUT_SDC_MISO             	PTD_GPIO_INPUT_FILTER_OFF

/** SPI CLK pin configuration for SD CARD module */
#define PTD_PORT_SDC_CLK              	gpioPortD
#define PTD_PIN_SDC_CLK               	2
#define PTD_MODE_SDC_CLK              	gpioModePushPull
#define PTD_DRIVE_MODE_SDC_CLK          gpioDriveModeHigh
#define PTD_DOUT_SDC_CLK              	PTD_GPIO_LOW

/** VDD pin configuration for BMG160 Module */
#define PTD_PORT_BMG160_VDD             gpioPortD
#define PTD_PIN_BMG160_VDD              13
#define PTD_MODE_BMG160_VDD             gpioModePushPullDrive
#define PTD_DOUT_BMG160_VDD             PTD_GPIO_LOW

/** Interrupt pin for BMG160 interrupt 1 */
#define PTD_PORT_BMG160_INT1            gpioPortC
#define PTD_PIN_BMG160_INT1             6
#define PTD_MODE_BMG160_INT1            gpioModeInput
#define PTD_DOUT_BMG160_INT1            PTD_GPIO_INPUT_FILTER_ON

/** Interrupt pin for BMG160 interrupt 2 */
#define PTD_PORT_BMG160_INT2            gpioPortA
#define PTD_PIN_BMG160_INT2             7
#define PTD_MODE_BMG160_INT2            gpioModeInput
#define PTD_DOUT_BMG160_INT2            PTD_GPIO_INPUT_FILTER_ON

/** VDD pin configuration for BMM150 */
#define PTD_PORT_BMM150_VDD             gpioPortE
#define PTD_PIN_BMM150_VDD              5
#define PTD_MODE_BMM150_VDD             gpioModePushPull
#define PTD_DOUT_BMM150_VDD             PTD_GPIO_LOW

/** Interrupt pin for BMM150 */
#define PTD_PORT_BMM150_INT             gpioPortD
#define PTD_PIN_BMM150_INT              12
#define PTD_MODE_BMM150_INT             gpioModeInput
#define PTD_DOUT_BMM150_INT             PTD_GPIO_INPUT_FILTER_ON

/** Data ready pin for BMM150 */
#define PTD_PORT_BMM150_DATA_READY      gpioPortE
#define PTD_PIN_BMM150_DATA_READY       15
#define PTD_MODE_BMM150_DATA_READY      gpioModeInput
#define PTD_DOUT_BMM150_DATA_READY      PTD_GPIO_INPUT_FILTER_ON

/** VDD pin for BMI160 */
#define PTD_PORT_BMI160_VDD             gpioPortE
#define PTD_PIN_BMI160_VDD              3
#define PTD_MODE_BMI160_VDD             gpioModePushPullDrive
#define PTD_DOUT_BMI160_VDD             PTD_GPIO_LOW

/** Interrupt pin for BMI160 interrupt 1 */
#define PTD_PORT_BMI160_INT1            gpioPortA
#define PTD_PIN_BMI160_INT1             13
#define PTD_MODE_BMI160_INT1            gpioModeInput
#define PTD_DOUT_BMI160_INT1            PTD_GPIO_INPUT_FILTER_ON

/** Interrupt pin for BMI160 interrupt 2 */
#define PTD_PORT_BMI160_INT2            gpioPortA
#define PTD_PIN_BMI160_INT2             14
#define PTD_MODE_BMI160_INT2            gpioModeInputPull
#define PTD_DOUT_BMI160_INT2            PTD_GPIO_LOW

/** Interrupt pin for MAX44009 sensor */
#define PTD_PORT_MAX44009_INT          gpioPortE
#define PTD_PIN_MAX44009_INT           4
#define PTD_MODE_MAX44009_INT          gpioModeInput
#define PTD_DOUT_MAX44009_INT          PTD_GPIO_INPUT_FILTER_ON

/** Vcc enable pin for MAX44009 sensor */
#define PTD_PORT_MAX44009_VDD_PORT     gpioPortE
#define PTD_PIN_MAX44009_VDD_PORT      6
#define PTD_MODE_MAX44009_VDD_PORT     gpioModePushPullDrive
#define PTD_DOUT_MAX44009_VDD_PORT     PTD_GPIO_LOW

/** VDD pin for AKU340 sensor */
#define PTD_PORT_AKU340_VDD             gpioPortD
#define PTD_PIN_AKU340_VDD              9
#define PTD_MODE_AKU340_VDD             gpioModePushPull
#define PTD_DOUT_AKU340_VDD             PTD_GPIO_LOW

/** ADC input pin of AKU340 sensor */
#define PTD_PORT_AKU340_ADC_OUT         gpioPortD
#define PTD_PIN_AKU340_ADC_OUT          4
#define PTD_MODE_AKU340_ADC_OUT         gpioModeInputPull
#define PTD_DOUT_AKU340_ADC_OUT         PTD_GPIO_INPUT_FILTER_OFF

/** ADC input pin of Battery voltage monitoring */
#define PTD_PORT_VBAT_ADC_OUT         gpioPortD
#define PTD_PIN_VBAT_ADC_OUT          7
#define PTD_MODE_VBAT_ADC_OUT         gpioModeInput
#define PTD_DOUT_VBAT_ADC_OUT         PTD_GPIO_INPUT_FILTER_OFF

/** Timer output pin for compare operations of extension bus */
#define PTD_PORT_EXTENSION_TIM0_CC0         gpioPortA
#define PTD_PIN_EXTENSION_TIM0_CC0          0
#define PTD_MODE_EXTENSION_TIM0_CC0         gpioModePushPull
#define PTD_DOUT_EXTENSION_TIM0_CC0         PTD_GPIO_LOW

/** Timer output pin for compare operations of extension bus */
#define PTD_PORT_EXTENSION_TIM0_CC1         gpioPortC
#define PTD_PIN_EXTENSION_TIM0_CC1          0
#define PTD_MODE_EXTENSION_TIM0_CC1         gpioModePushPull
#define PTD_DOUT_EXTENSION_TIM0_CC1         PTD_GPIO_LOW

/** Timer output pin for compare operations of extension bus */
#define PTD_PORT_EXTENSION_TIM0_CC2         gpioPortC
#define PTD_PIN_EXTENSION_TIM0_CC2          1
#define PTD_MODE_EXTENSION_TIM0_CC2         gpioModePushPull
#define PTD_DOUT_EXTENSION_TIM0_CC2         PTD_GPIO_LOW

/** Timer output pin for pwm operations of extension bus */
#define PTD_PORT_EXTENSION_TIM0_CDTI0       gpioPortC
#define PTD_PIN_EXTENSION_TIM0_CDTI0        2
#define PTD_MODE_EXTENSION_TIM0_CDTI0       gpioModePushPull
#define PTD_DOUT_EXTENSION_TIM0_CDTI0       PTD_GPIO_LOW

/** Timer output pin for pwm operations of extension bus */
#define PTD_PORT_EXTENSION_TIM0_CDTI1       gpioPortC
#define PTD_PIN_EXTENSION_TIM0_CDTI1        3
#define PTD_MODE_EXTENSION_TIM0_CDTI1       gpioModePushPull
#define PTD_DOUT_EXTENSION_TIM0_CDTI1       PTD_GPIO_LOW

/** Timer output pin for pwm operations of extension bus */
#define PTD_PORT_EXTENSION_TIM0_CDTI2       gpioPortC
#define PTD_PIN_EXTENSION_TIM0_CDTI2        4
#define PTD_MODE_EXTENSION_TIM0_CDTI2       gpioModePushPull
#define PTD_DOUT_EXTENSION_TIM0_CDTI2       PTD_GPIO_LOW

/** Timer input pin for capture operations of extension bus */
#define PTD_PORT_EXTENSION_TIM2_CC0       gpioPortC
#define PTD_PIN_EXTENSION_TIM2_CC0        8
#define PTD_MODE_EXTENSION_TIM2_CC0       gpioModeInputPullFilter
#define PTD_DOUT_EXTENSION_TIM2_CC0       PTD_GPIO_LOW

/** Timer input pin for capture operations of extension bus */
#define PTD_PORT_EXTENSION_TIM2_CC1       gpioPortC
#define PTD_PIN_EXTENSION_TIM2_CC1        9
#define PTD_MODE_EXTENSION_TIM2_CC1       gpioModeInputPullFilter
#define PTD_DOUT_EXTENSION_TIM2_CC1       PTD_GPIO_LOW

/** Timer input pin for capture operations of extension bus */
#define PTD_PORT_EXTENSION_TIM2_CC2       gpioPortC
#define PTD_PIN_EXTENSION_TIM2_CC2        10
#define PTD_MODE_EXTENSION_TIM2_CC2       gpioModeInputPullFilter
#define PTD_DOUT_EXTENSION_TIM2_CC2       PTD_GPIO_LOW

/** ADC input pin for extension bus */
#define PTD_PORT_EXTENSION_ADC0_CH5       gpioPortD
#define PTD_PIN_EXTENSION_ADC0_CH5        5
#define PTD_MODE_EXTENSION_ADC0_CH5       gpioModeInputPullFilter
#define PTD_DOUT_EXTENSION_ADC0_CH5       PTD_GPIO_INPUT_FILTER_ON

/** ADC input pin for extension bus */
#define PTD_PORT_EXTENSION_ADC0_CH6       gpioPortD
#define PTD_PIN_EXTENSION_ADC0_CH6        6
#define PTD_MODE_EXTENSION_ADC0_CH6       gpioModeInput
#define PTD_DOUT_EXTENSION_ADC0_CH6       PTD_GPIO_INPUT_FILTER_OFF

/** GPIO input/output pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_GPIO_IN_OUT_0               gpioPortA
#define PTD_PIN_EXTENSION_GPIO_IN_OUT_0                1
#define PTD_MODE_EXTENSION_GPIO_IN_OUT_0               gpioModePushPull
#define PTD_DOUT_EXTENSION_GPIO_IN_OUT_0               PTD_GPIO_LOW

/** GPIO input/output pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_GPIO_IN_OUT_1               gpioPortE
#define PTD_PIN_EXTENSION_GPIO_IN_OUT_1                2
#define PTD_MODE_EXTENSION_GPIO_IN_OUT_1               gpioModeInput
#define PTD_DOUT_EXTENSION_GPIO_IN_OUT_1               PTD_GPIO_INPUT_FILTER_OFF

/** UART TX pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_UART1_TX              gpioPortB
#define PTD_PIN_EXTENSION_UART1_TX               9
#define PTD_MODE_EXTENSION_UART1_TX              gpioModePushPull
#define PTD_DOUT_EXTENSION_UART1_TX              PTD_GPIO_HIGH

/** UART RX pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_UART1_RX              gpioPortB
#define PTD_PIN_EXTENSION_UART1_RX               10
#define PTD_MODE_EXTENSION_UART1_RX              gpioModeInput
#define PTD_DOUT_EXTENSION_UART1_RX              PTD_GPIO_INPUT_FILTER_OFF

/** UART RTS output pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_UART1_RTS              gpioPortB
#define PTD_PIN_EXTENSION_UART1_RTS               2
#define PTD_MODE_EXTENSION_UART1_RTS              gpioModePushPull
#define PTD_DOUT_EXTENSION_UART1_RTS              PTD_GPIO_HIGH

/** UART CTS input pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_UART1_CTS              gpioPortF
#define PTD_PIN_EXTENSION_UART1_CTS               6
#define PTD_MODE_EXTENSION_UART1_CTS              gpioModeInput
#define PTD_DOUT_EXTENSION_UART1_CTS              PTD_GPIO_INPUT_FILTER_OFF

/** SPI MOSI pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_US2_MOSI                gpioPortB
#define PTD_PIN_EXTENSION_US2_MOSI                 3
#define PTD_MODE_EXTENSION_US2_MOSI                gpioModePushPull
#define PTD_DOUT_EXTENSION_US2_MOSI                PTD_GPIO_LOW

/** SPI MISO pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_US2_MISO               gpioPortB
#define PTD_PIN_EXTENSION_US2_MISO                4
#define PTD_MODE_EXTENSION_US2_MISO               gpioModeInput
#define PTD_DOUT_EXTENSION_US2_MISO               PTD_GPIO_INPUT_FILTER_OFF

/** SPI Chip Select pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_US2_CS                gpioPortD
#define PTD_PIN_EXTENSION_US2_CS                 8
#define PTD_MODE_EXTENSION_US2_CS                gpioModePushPull
#define PTD_DOUT_EXTENSION_US2_CS                PTD_GPIO_HIGH

/** SPI clock pin configuration for Extension Bus */
#define PTD_PORT_EXTENSION_US2_SCK               gpioPortB
#define PTD_PIN_EXTENSION_US2_SCK                5
#define PTD_MODE_EXTENSION_US2_SCK               gpioModePushPull
#define PTD_DOUT_EXTENSION_US2_SCK               PTD_GPIO_LOW

/** DATA pin for I2C in Extension Bus */
#define PTD_PORT_EXTENSION_I2C1_SDA               gpioPortB
#define PTD_PIN_EXTENSION_I2C1_SDA                11
#define PTD_MODE_EXTENSION_I2C1_SDA               gpioModeWiredAnd
#define PTD_DOUT_EXTENSION_I2C1_SDA               PTD_GPIO_HIGH

/** CLOCK pin for I2C in Extension Bus */
#define PTD_PORT_EXTENSION_I2C1_SCL               gpioPortB
#define PTD_PIN_EXTENSION_I2C1_SCL                12
#define PTD_MODE_EXTENSION_I2C1_SCL               gpioModeWiredAnd
#define PTD_DOUT_EXTENSION_I2C1_SCL               PTD_GPIO_HIGH

/** Drive Mode Setting for all  the pins of Port A */
#define PTD_PORT_PORT_A                      gpioPortA
#define PTD_DRIVE_MODE_PORT_A            gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port B */
#define PTD_PORT_PORT_B                      gpioPortB
#define PTD_DRIVE_MODE_PORT_B            gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port C */
#define PTD_PORT_PORT_C                      gpioPortC
#define PTD_DRIVE_MODE_PORT_C            gpioDriveModeStandard

/** Drive Mode Setting for all  the pins of Port D */
#define PTD_PORT_PORT_D                      gpioPortD
#define PTD_DRIVE_MODE_PORT_D            gpioDriveModeHigh

/** Drive Mode Setting for all  the pins of Port E */
#define PTD_PORT_PORT_E                      gpioPortE
#define PTD_DRIVE_MODE_PORT_E            gpioDriveModeLow

/** Drive Mode Setting for all  the pins of Port F */
#define PTD_PORT_PORT_F                      gpioPortF
#define PTD_DRIVE_MODE_PORT_F            gpioDriveModeStandard

/** Number of ODD interrupts in the project */
#define PTD_NO_OF_ODD_INTERRUPTS		6

/** Number of EVEN interrupts in the project */
#define PTD_NO_OF_EVEN_INTERRUPTS		8

/** Maximum Number of Port and Pin */
#define PTD_MAXIMUM_PORT                   gpioPortF
#define PTD_MAXIMUM_PIN                    15

/* public type and macro definitions */

/* public function prototype declarations */

/* public global variable declarations */

/* inline function definitions */

#endif /* XDK110_PTD_PORTDRIVER_PH_H_ */

/**@}*/

/** ************************************************************************* */
