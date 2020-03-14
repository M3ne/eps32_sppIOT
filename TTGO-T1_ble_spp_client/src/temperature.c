#include "peripheral.h"
#include "temperature.h"


/**
 *  select values: 
 *          0: get data in Celsius
 *          1: get data in Fahrenheit 
 *          else: raw data in 10bits (float to be converted)
 */
float readTemp(uint8_t select){
    uint16_t raw_temp = 0;
    float temp_F,temp_C = 0;
    raw_temp = adc1_get_raw(ADC1_CHANNEL_0);
    temp_C = (raw_temp / 1024.0) * 3300/10;
    temp_F = (temp_C * 1.8) + 32;
    //printf("Read adc raw= %d/1024 C=%.1f°C F=%.1f°F \n\r",raw_temp,temp_C,temp_F);
    if(select == 0)
        return temp_C;
    if(select == 1)
        return temp_F;
    else
        return raw_temp*1.0;    
}