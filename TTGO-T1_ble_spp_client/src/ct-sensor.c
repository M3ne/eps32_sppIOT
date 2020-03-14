// #include "ct-sensor.h"



//   uint16_t _sampling_data_points_count;
//   uint16_t _sampling_delay_ms;
  
//   double _peak_current;


//   double _rms_current_data_points_per_channel[_channels_count][MAX_NUMER_OF_RMS_CURRENT_DATA_POINTS_PER_CHANNEL];
//   double _sum_of_current_rms_per_channel[_channels_count];
//   double _min_current_rms_per_channel[_channels_count];
//   double _avg_current_rms_per_channel[_channels_count];
//   double _max_current_rms_per_channel[_channels_count];


// bool CTSensor_init(){
//     _peak_current = sqrt(2) * _max_current_input;
//     _sampling_data_points_count = _sampling_period_in_seconds * _sampling_frequency_in_hertz;
//     _sampling_delay_ms = (uint16_t)roundf(1000 / _sampling_frequency_in_hertz);
//     return true;
// }

// void CTSensor_calibrate(float *ref_values)
// {
//     for (uint8_t c = 0; c < _channels_count; c++)
//     {
//         _ref_voltage_on_zero_current_per_channel[c] = ref_values[c];
//     }
// }

// void CTSensor_print_calibration_info()
// {
//     float sum_of_raw_values_per_channel[_channels_count] = {0, 0, 0, 0};
//     uint8_t points = CALIBRATION_DATA_POINTS;

//     while (points)
//     {
//         for (uint8_t c = 0; c < _channels_count; c++)
//         {
//             sum_of_raw_values_per_channel[c] += get_raw_value_for(c);
//             delay(DELAY_BETWEEN_I2C_READS_MS);
//         }
//         points--;
//     }

//     printf("Current calibration set: {%f, %f, %f, %f}; \n",
//          _ref_voltage_on_zero_current_per_channel[0],
//          _ref_voltage_on_zero_current_per_channel[1],
//          _ref_voltage_on_zero_current_per_channel[2],
//          _ref_voltage_on_zero_current_per_channel[3]);
//     printf("Desired calibration set: {%f, %f, %f, %f}; \n",
//          sum_of_raw_values_per_channel[0] / CALIBRATION_DATA_POINTS,
//          sum_of_raw_values_per_channel[1] / CALIBRATION_DATA_POINTS,
//          sum_of_raw_values_per_channel[2] / CALIBRATION_DATA_POINTS,
//          sum_of_raw_values_per_channel[3] / CALIBRATION_DATA_POINTS);
// }


// void CTSensor_start_sampling()
// {
//     double instant;
//     double sum_of_squared_instants[_channels_count] = {};

//     printf("[SAMPLING] START #%d! \n", _current_rms_data_points_count);

//     for (uint16_t p = 0; p < _sampling_data_points_count; p++)
//     {
//         for (uint8_t i = 0; i < _channels_count; i++)
//         {
//             float _ref_voltage_on_zero_current = _ref_voltage_on_zero_current_per_channel[i];
//             instant = (get_raw_value_for(i) - _ref_voltage_on_zero_current) / _ref_voltage_on_zero_current;
//             sum_of_squared_instants[i] += instant * instant;
//             delay(DELAY_BETWEEN_I2C_READS_MS);
//         }
//         if (!_skip_sampling_delay)
//         {
//             delay(_sampling_delay_ms);
//         }
//     }

//     for (uint8_t channel = 0; channel < _channels_count; channel++)
//     {
//         double rms = _get_current_rms_for_instants(sum_of_squared_instants[channel]);

//         _add_current_rms_params_for(channel, rms);
//     }

//     _current_rms_data_points_count++;
//     printf("[SAMPLING] data point count so far: %d \n", _current_rms_data_points_count);
//     printf("------------------------------------\n");
// }

// void CTSensor_add_current_rms_params_for(uint8_t channel, double current_rms)
// {
//     printf("[CHANNEL %d] Add current RMS: %f Amps to data points \n", channel, current_rms);

//     _rms_current_data_points_per_channel[channel][_current_rms_data_points_count - 1] = current_rms;

//     _sum_of_current_rms_per_channel[channel] += current_rms;

//     if (_min_current_rms_per_channel[channel] > current_rms)
//     {
//         _min_current_rms_per_channel[channel] = current_rms;
//     }

//     if (_max_current_rms_per_channel[channel] < current_rms)
//     {
//         _max_current_rms_per_channel[channel] = current_rms;
//     }

//     _avg_current_rms_per_channel[channel] = _sum_of_current_rms_per_channel[channel] / _current_rms_data_points_count;
// }

// double CTSensor_get_min_current_rms_in_amps_for(uint8_t channel)
// {
//     return _min_current_rms_per_channel[channel];
// }

// double CTSensor_get_avg_current_rms_in_amps_for(uint8_t channel)
// {
//     return _avg_current_rms_per_channel[channel];
// }

// double CTSensor_get_max_current_rms_in_amps_for(uint8_t channel)
// {
//     return _max_current_rms_per_channel[channel];
// }

// double CTSensor_get_energy_in_watts_hour_for(uint8_t channel)
// {
//     return _ref_voltage_input * _sum_of_current_rms_per_channel[channel] * _sampling_period_in_seconds / 3600;
// }

// double CTSensor_get_voltage_in_volts_for(uint8_t channel)
// {
//     return (double)_ref_voltage_input;
// }

// double CTSensor_get_avg_power_in_watts_for(uint8_t channel)
// {
//     return _ref_voltage_input * get_avg_current_rms_in_amps_for(channel);
// }

// double * CTSensor_get_rms_current_data_points_for(uint8_t channel)
// {
//     return _rms_current_data_points_per_channel[channel];
// }

// double CTSensor_get_current_rms_for_instants(double sum_of_squared_instants)
// {
//     printf("Sum of squared instants: %f \n", sum_of_squared_instants);
//     printf("Sampling data points count: %d \n", _sampling_data_points_count);
//     printf("Peak current: %f \n", _peak_current);

//     double rms = _peak_current * sqrt(sum_of_squared_instants / _sampling_data_points_count);
//     bool should_filter_noise_out = rms < _current_noise_level;

//     if (should_filter_noise_out)
//     {
//         rms = 0.0;
//     }

//     return rms;
// }

// void CTSensor_setup()
// {
//     _new_buffers();

//     for (uint8_t channel = 0; channel < _channels_count; channel++)
//     {
//         _sum_of_current_rms_per_channel[channel] = 0.0;
//         _max_current_rms_per_channel[channel] = 0.0;
//         _avg_current_rms_per_channel[channel] = 0.0;
//         _min_current_rms_per_channel[channel] = _max_current_input;
//     }

//     _current_rms_data_points_count = 1;
// }





// void CTSensor_print_debug_info()
// {
//     printf("Sampling period in seconds: %d \n", _sampling_period_in_seconds);
//     printf("Sampling frequency in hertz: %d \n", _sampling_frequency_in_hertz);
//     printf("Sampling data points count: %d \n", _sampling_data_points_count);
//     printf("Sampling delay: %d \n", _sampling_delay_ms);
//     printf("Skip sampling delay: %d \n", _skip_sampling_delay);
//     printf("Max current input from sensors: %d \n", _max_current_input);
//     printf("Max peak current: %f \n", _peak_current);
//     printf("Ref constant voltage input from grid: %d \n", _ref_voltage_input);
//     printf("Current RMS data points count: %d \n", _current_rms_data_points_count);
//     printf("Current RMS noise level: %f\n", _current_noise_level);
//     printf("------------------------------------\n");
// }

// void CTSensor_print_channels_info()
// {
//     for (uint8_t c = 0; c < _channels_count; c++)
//     {
//         printf("[CHANNEL %d] Raw value: %f V \n", c, get_raw_value_for(c));
//         printf("[CHANNEL %d] Ref voltage on zero current: %f V \n", c, _ref_voltage_on_zero_current_per_channel[c]);
//         printf("[CHANNEL %d] Min Current RMS: %f Amps \n", c, get_min_current_rms_in_amps_for(c));
//         printf("[CHANNEL %d] Avg Current RMS: %f Amps \n", c, get_avg_current_rms_in_amps_for(c));
//         printf("[CHANNEL %d] Max Current RMS: %f Amps \n", c, get_max_current_rms_in_amps_for(c));
//         printf("[CHANNEL %d] Energy: %f Watts-Hour \n", c, get_energy_in_watts_hour_for(c));
//         printf("------------------------------------\n");
//     }
// }
