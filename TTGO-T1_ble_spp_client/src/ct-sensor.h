#ifndef CT_SENSOR_H
#define CT_SENSOR_H


//#include "peripheral.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <driver/adc.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "peripheral.h"





#define DEBUG_MODE 0
#define CALIBRATION_DATA_POINTS 100
#define DELAY_BETWEEN_I2C_READS_MS 2
#define MAX_NUMER_OF_RMS_CURRENT_DATA_POINTS_PER_CHANNEL 100
#define CT_refVal 3300.0/2  //TODO measure value with ADC channel
// uncomment current sensor used

// uncomment current sensor used



//bool is_debuging_mode = true;

//bool is_debuging_mode = true;

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

#define CURR_ADC_CHANNEL ADC1_CHANNEL_0
#define CURRref_ADC_CHANNEL ADC1_CHANNEL_3


bool CTSensor_init();
void CTSensor_setup();
void CTSensor_calibrate(float ref_values);
void CTSensor_print_calibration_info();
void CTSensor_start_sampling();
void CTSensor_add_current_rms_params_for(float current_rms);
float CTSensor_get_min_current_rms_in_amps_for();
float CTSensor_get_avg_current_rms_in_amps_for();
float CTSensor_get_max_current_rms_in_amps_for();
float CTSensor_get_energy_in_watts_hour_for();
float CTSensor_get_voltage_in_volts_for();
float CTSensor_get_avg_power_in_watts_for();
float *CTSensor_get_rms_current_data_points_for();
float CTSensor_get_current_rms_for_instants(float sum_of_squared_instants);
void CTSensor_print_debug_info();
void CTSensor_print_channels_info();

#endif