#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/adc.h"

#define BLINK_GPIO 5


i2c_port_t peripheral_getI2C_port();
void peripheral_initI2C();
void peripheral_initGPIO();
void peripheral_initTemp();