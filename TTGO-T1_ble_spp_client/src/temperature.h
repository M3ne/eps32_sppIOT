#ifndef TEMPERATURE_H
#define TEMPERATURE_H
#include "peripheral.h"




typedef enum{
    DATA_CELSIUS,
    DATA_FAHRENHEIT
} TemperatureData_t;

void t_readTemp(TemperatureData_t select, float * temperature);
#endif