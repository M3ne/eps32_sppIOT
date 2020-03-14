#ifndef SPG30_H
#define SPG30_H
#include "esp_err.h"
#include "peripheral.h"
#include <stdio.h>
#include "stdint.h"
#include "esp_task.h"


// the i2c address
#define SGP30_I2CADDR_DEFAULT 0x58 ///< SGP30 has only one I2C address

// commands and constants
#define SGP30_FEATURESET 0x0022    ///< The required set for this library
#define SGP30_CRC8_POLYNOMIAL 0x31 ///< Seed for SGP30's CRC polynomial
#define SGP30_CRC8_INIT 0xFF       ///< Init value for CRC
#define SGP30_WORD_LEN 2           ///< 2 bytes per word

#define SGP30_BITRATE 1000/portTICK_RATE_MS ///< "# / ticks4ms" -> 1000/portTICK_RATE_MS = 1000kHz, 500/portTICK_RATE_MS = 500kHz, etc

esp_err_t spg30_init(i2c_port_t i2c_num);
esp_err_t spg30_IAQmeasure(i2c_port_t i2c_num, uint16_t *TVOC, uint16_t *eCO2);
esp_err_t spg30_getIAQBaseline(i2c_port_t i2c_num, uint16_t *eco2_base, uint16_t *tvoc_base);
esp_err_t spg30_IAQmeasureRaw(i2c_port_t i2c_num, uint16_t *rawEthanol, uint16_t *rawH2);
esp_err_t spg30_setIAQBaseline(i2c_port_t i2c_num, uint16_t eco2_base, uint16_t tvoc_base);
esp_err_t spg30_setHumidity(i2c_port_t i2c_num, uint32_t absolute_humidity);

#endif