/* 
	DHT22 temperature sensor driver
*/

#ifndef DHT_H_
#define DHT_H_
#include <driver/gpio.h>
#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2
#define DHT_GPIO_CHANNEL GPIO_NUM_4

// == function prototypes =======================================
#include "esp_err.h"
void DHT22_setup();
// void setDHTgpio(int gpio);
esp_err_t DHT22_errorHandler(esp_err_t response);
esp_err_t DHT22_readDHT();
void DHT22_getHumidity(float* humidityPointer);
void DHT22_getTemperature(float * temperaturePointer);
// int getSignalLevel(int usTimeOut, bool state);

#endif