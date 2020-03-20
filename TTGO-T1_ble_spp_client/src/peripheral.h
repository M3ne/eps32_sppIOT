#ifndef PERIPHERAL_H
#define PERIPHERAL_H


#define SCT_013_000_YHDC_30A

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/adc.h"
#include "spg30.h"
#include "temperature.h"
#include "ct-sensor.h"

#define BLINK_GPIO 5






i2c_port_t peripheral_getI2C_port();
void peripheral_initI2C();
void peripheral_initGPIO();
void peripheral_initTemp();
#endif