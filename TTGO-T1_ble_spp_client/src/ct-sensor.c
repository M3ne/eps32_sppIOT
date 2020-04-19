#include "ct-sensor.h"


#ifdef SCT_013_000_YHDC_30A
    #define _channels_count  1
    uint8_t _sampling_period_in_seconds = 1;
    uint16_t _sampling_frequency_in_hertz = 100;
    uint8_t _max_current_input = 30;
    uint8_t _ref_voltage_input = 230;
    float _current_noise_level = 0.05;
    bool _skip_sampling_delay = true;
    float _ref_voltage_on_zero_current_per_channel = 1.65;
    uint8_t _current_rms_data_points_count;
#elif SCT_013_000_YHDC_100A
     #define _channels_count  1
    uint8_t _sampling_period_in_seconds = 1;
    uint16_t _sampling_frequency_in_hertz = 100;
    uint8_t _max_current_input = 100;
    uint8_t _ref_voltage_input = 230;
    float _current_noise_level = 0.05;
    bool _skip_sampling_delay = true;
    float _ref_voltage_on_zero_current_per_channel = 1.65;
    uint8_t _current_rms_data_points_count = 1;    
#else
    #define _channels_count 1
    uint8_t _sampling_period_in_seconds = 1;
    uint16_t _sampling_frequency_in_hertz = 100;
    uint8_t _max_current_input = 100;
    uint8_t _ref_voltage_input = 230;
    float _current_noise_level = 0.05;
    bool _skip_sampling_delay = true;
    float _ref_voltage_on_zero_current_per_channel = 1.65;
    uint8_t _current_rms_data_points_count = 1;
#endif

uint16_t _sampling_data_points_count;
uint16_t _sampling_delay_ms;

float _peak_current;
float _rms_current_data_points_per_channel[MAX_NUMER_OF_RMS_CURRENT_DATA_POINTS_PER_CHANNEL];
float _sum_of_current_rms_per_channel;
float _min_current_rms_per_channel;
float _avg_current_rms_per_channel;
float _max_current_rms_per_channel;




bool CTSensor_init(){
    _peak_current = sqrt(2) * _max_current_input;
    _sampling_data_points_count = _sampling_period_in_seconds * _sampling_frequency_in_hertz;
    _sampling_delay_ms = (uint16_t)roundf(1000 / _sampling_frequency_in_hertz);
    
    return ESP_OK;
}
void CTSensor_setup(){
    _sum_of_current_rms_per_channel = 0.0;
    _max_current_rms_per_channel = 0.0;
    _avg_current_rms_per_channel = 0.0;
    _min_current_rms_per_channel = _max_current_input;
    _current_rms_data_points_count = 1;
}

void CTSensor_calibrate(float ref_values){
        _ref_voltage_on_zero_current_per_channel = ref_values;
}

void CTSensor_print_calibration_info(){
    float sum_of_raw_values_per_channel = 0;
    uint8_t points = CALIBRATION_DATA_POINTS;

    while (points)
    {
        sum_of_raw_values_per_channel += adc1_get_raw(CURR_ADC_CHANNEL);
        vTaskDelay(DELAY_BETWEEN_I2C_READS_MS / portTICK_PERIOD_MS);
        points--;
    }

    printf("Current calibration set: {%f}; \n",
         _ref_voltage_on_zero_current_per_channel);
    printf("Desired calibration set: {%f}; \n",
         sum_of_raw_values_per_channel / CALIBRATION_DATA_POINTS);
}


void CTSensor_start_sampling(){
    float instant;
    float sum_of_squared_instants = 0;

    // printf("[SAMPLING] START #%d! \n", _current_rms_data_points_count);

    for (uint16_t p = 0; p < _sampling_data_points_count; p++)
    {
        float _ref_voltage_on_zero_current = _ref_voltage_on_zero_current_per_channel;
        instant = (adc1_get_raw(CURR_ADC_CHANNEL) - _ref_voltage_on_zero_current) / _ref_voltage_on_zero_current;
        sum_of_squared_instants += instant * instant;
        vTaskDelay(DELAY_BETWEEN_I2C_READS_MS/ portTICK_PERIOD_MS);
        
        if (!_skip_sampling_delay)
        {
            vTaskDelay(_sampling_delay_ms/ portTICK_PERIOD_MS);
        }
    }
    
    float rms = CTSensor_get_current_rms_for_instants(sum_of_squared_instants);
    CTSensor_add_current_rms_params_for(rms);

    _current_rms_data_points_count++;
    // printf("[SAMPLING] data point count so far: %d \n", _current_rms_data_points_count);
    // printf("------------------------------------\n");
}

void CTSensor_add_current_rms_params_for(float current_rms){
    // printf("Add current RMS: %f Amps to data points \n", current_rms);

    _rms_current_data_points_per_channel[_current_rms_data_points_count - 1] = current_rms;
    _sum_of_current_rms_per_channel+= current_rms;

    if (_min_current_rms_per_channel > current_rms){_min_current_rms_per_channel = current_rms;}

    if (_max_current_rms_per_channel < current_rms){_max_current_rms_per_channel = current_rms;}

    _avg_current_rms_per_channel = _sum_of_current_rms_per_channel / _current_rms_data_points_count;
}

float CTSensor_get_min_current_rms_in_amps_for(){
    return _min_current_rms_per_channel;
}

float CTSensor_get_avg_current_rms_in_amps_for(){
    return _avg_current_rms_per_channel;
}

float CTSensor_get_max_current_rms_in_amps_for(){
    return _max_current_rms_per_channel;
}

float CTSensor_get_energy_in_watts_hour_for(){
    return _ref_voltage_input * _sum_of_current_rms_per_channel * _sampling_period_in_seconds / 3600;
}

float CTSensor_get_voltage_in_volts_for(){
    return (float)_ref_voltage_input;
}

float CTSensor_get_avg_power_in_watts_for(){
    return _ref_voltage_input * CTSensor_get_avg_current_rms_in_amps_for();
}

float * CTSensor_get_rms_current_data_points_for(){
    return (float *)_rms_current_data_points_per_channel;
}

float CTSensor_get_current_rms_for_instants(float sum_of_squared_instants){
    // printf("Sum of squared instants: %f \n", sum_of_squared_instants);
    // printf("Sampling data points count: %d \n", _sampling_data_points_count);
    // printf("Peak current: %f \n", _peak_current);

    float rms = _peak_current * sqrt(sum_of_squared_instants / _sampling_data_points_count);
    bool should_filter_noise_out = rms < _current_noise_level;

    if (should_filter_noise_out){
        rms = 0.0;
    }

    return rms;
}


void CTSensor_print_debug_info(){
    printf("Sampling period in seconds: %d \n", _sampling_period_in_seconds);
    printf("Sampling frequency in hertz: %d \n", _sampling_frequency_in_hertz);
    printf("Sampling data points count: %d \n", _sampling_data_points_count);
    printf("Sampling delay: %d \n", _sampling_delay_ms);
    printf("Skip sampling delay: %d \n", _skip_sampling_delay);
    printf("Max current input from sensors: %d \n", _max_current_input);
    printf("Max peak current: %f \n", _peak_current);
    printf("Ref constant voltage input from grid: %d \n", _ref_voltage_input);
    printf("Current RMS data points count: %d \n", _current_rms_data_points_count);
    printf("Current RMS noise level: %f\n", _current_noise_level);
    printf("------------------------------------\n");
}

void CTSensor_print_channels_info(){
    printf("Raw value: %d V \n", adc1_get_raw(CURR_ADC_CHANNEL));
    printf("Ref voltage on zero current: %f V \n", _ref_voltage_on_zero_current_per_channel);
    printf("Min Current RMS: %f Amps \n", CTSensor_get_min_current_rms_in_amps_for());
    printf("Avg Current RMS: %f Amps \n", CTSensor_get_avg_current_rms_in_amps_for());
    printf("Max Current RMS: %f Amps \n", CTSensor_get_max_current_rms_in_amps_for());
    printf("Energy: %f Watts-Hour \n", CTSensor_get_energy_in_watts_hour_for());
    printf("------------------------------------\n");
}