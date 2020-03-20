
#include "peripheral.h"



static gpio_num_t i2c_gpio_sda = 22;
static gpio_num_t i2c_gpio_scl = 21;
static uint32_t i2c_frequency = 10000;
static i2c_port_t i2c_port = I2C_NUM_0;
static  size_t i2x_BUFF_RX_LEN = 0;
static  size_t i2x_BUFF_TX_LEN = 0;
uint8_t serialnumber[15];
i2c_config_t conf;

void peripheral_initI2C(){
    
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = i2c_gpio_sda;
    conf.scl_io_num = i2c_gpio_scl;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = i2c_frequency;
    i2c_param_config(i2c_port, &conf);
    i2c_driver_install(i2c_port, I2C_MODE_MASTER, i2x_BUFF_RX_LEN, i2x_BUFF_TX_LEN, 0);

}

i2c_port_t peripheral_getI2C_port(){
    return i2c_port;
}

void peripheral_initGPIO(){
    gpio_pad_select_gpio(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void peripheral_initTemp(){
    adc1_config_width(ADC_WIDTH_BIT_10);
    //TEMPERATURE ADC CHANNEL
    adc1_config_channel_atten(TEMPERATURE_ADC_CHANNEL,ADC_ATTEN_DB_0);
    //CURRENT ADC CHANNEL
    adc1_config_channel_atten(CURR_ADC_CHANNEL,ADC_ATTEN_DB_0);

    adc_power_on();
}