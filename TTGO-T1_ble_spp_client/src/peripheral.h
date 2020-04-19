#ifndef PERIPHERAL_H
#define PERIPHERAL_H


#define SCT_013_000_YHDC_30A

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/adc.h"
#include "spg30.h"
#include "temperature.h"
#include "ct-sensor.h"
#include "dht22.h"

//  typedef enum {
//     ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO36 */
//     ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO37 */
//     ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO38 */
//     ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO39 */
//     ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32 */
//     ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33 */
//     ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34 */
//     ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35 */
//     ADC1_CHANNEL_MAX,
// } adc1_channel_t;
#define TEMPERATURE_ADC_CHANNEL ADC1_CHANNEL_0
#define BLINK_GPIO 5
#define CT_ADC_CHANNEL ADC1_CHANNEL_1






i2c_port_t peripheral_getI2C_port();
void peripheral_initI2C();
void peripheral_initGPIO();
void peripheral_initADC();
#endif