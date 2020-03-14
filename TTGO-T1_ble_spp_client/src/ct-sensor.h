// #ifndef CT_SENSOR_H
// #define CT_SENSOR_H

// #include <stdio.h>
// #include <stdint.h>
// #include <stdarg.h>
// #include <math.h>
// #include "stdbool.h"

// // uncomment current sensor used
// # define SCT_013_000_YHDC_30A


// #define DEBUG_MODE 1
// #define CALIBRATION_DATA_POINTS 100
// #define DELAY_BETWEEN_I2C_READS_MS 15
// #define MAX_NUMER_OF_RMS_CURRENT_DATA_POINTS_PER_CHANNEL 100



// #ifdef SCT_013_000_YHDC_30A
//     #define _channels_count  1
//     uint8_t _sampling_period_in_seconds = 1;
//     uint16_t _sampling_frequency_in_hertz = 100;
//     uint8_t _max_current_input = 30;
//     uint8_t _ref_voltage_input = 230;
//     float _current_noise_level = 0.05;
//     bool _skip_sampling_delay = true;
//     double _ref_voltage_on_zero_current_per_channel[4] = {1.65, 1.65, 1.65, 1.65};
//     uint8_t _current_rms_data_points_count = 1;
// #elif SCT_013_000_YHDC_100A
//      #define _channels_count  1
//     uint8_t _sampling_period_in_seconds = 1;
//     uint16_t _sampling_frequency_in_hertz = 100;
//     uint8_t _max_current_input = 100;
//     uint8_t _ref_voltage_input = 230;
//     float _current_noise_level = 0.05;
//     bool _skip_sampling_delay = true;
//     double _ref_voltage_on_zero_current_per_channel[4] = {1.65, 1.65, 1.65, 1.65};
//     uint8_t _current_rms_data_points_count = 1;    
// #else
//      #define _channels_count 1
//     uint8_t _sampling_period_in_seconds = 1;
//     uint16_t _sampling_frequency_in_hertz = 100;
//     uint8_t _max_current_input = 100;
//     uint8_t _ref_voltage_input = 230;
//     float _current_noise_level = 0.05;
//     bool _skip_sampling_delay = true;
//     double _ref_voltage_on_zero_current_per_channel[4] = {1.65, 1.65, 1.65, 1.65};
//     uint8_t _current_rms_data_points_count = 1;
// #endif



//   bool is_debuging_mode = true;

 




//   bool init();
//   void start_sampling();
//   void setup();
//   void teardown();

//   double get_min_current_rms_in_amps_for(uint8_t channel);
//   double get_avg_current_rms_in_amps_for(uint8_t channel);
//   double get_max_current_rms_in_amps_for(uint8_t channel);
//   double get_energy_in_watts_hour_for(uint8_t channel);
//   double get_voltage_in_volts_for(uint8_t channel);
//   double get_avg_power_in_watts_for(uint8_t channel);
//   double *get_rms_current_data_points_for(uint8_t channel);

//   void calibrate(float *ref_values);

//   void print_debug_info();
//   void print_calibration_info();
//   void print_channels_info();


//   virtual float get_raw_value_for(uint8_t channel) = 0;
//   virtual void delay(uint16_t miliseconds) = 0;



//   void _add_current_rms_params_for(uint8_t channel, double current_rms);
//   double _get_current_rms_for_instants(double sum_of_squared_instants);

//   void _log(const char *format, ...);

//   void _new_buffers();
//   void _delete_buffers();


// #endif