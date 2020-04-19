
#include "temperature.h"


/**
 *  select values: 
 *          0: get data in Celsius
 *          1: get data in Fahrenheit 
 *          else: raw data in 10bits (float to be converted)
 */
void t_readTemp(TemperatureData_t select, float * temperature){
    uint16_t raw_temp = 0;
    float temp_F,temp_C = 0;
    raw_temp = adc1_get_raw(ADC1_CHANNEL_0);
    temp_C = (raw_temp / 1024.0) * 3300/10;
    temp_F = (temp_C * 1.8) + 32;
    if(select == 0)
        *temperature = temp_C;
    if(select == 1)
        *temperature = temp_F;
    else
        *temperature = raw_temp*1.0;    
}