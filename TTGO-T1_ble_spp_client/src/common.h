#ifndef COMMON_H
#define COMMON_H
/*
 * DEFINES
 ****************************************************************************************
 */
/****************************************************************************************
*   DevID shall be different for each device: master device has DevID=0,
*   others will have a progressive unique number. 
*   The DevID will identify the specific module so it position and funtionalities.
*****************************************************************************************/
#define DevID                       1


#define HRPS_HT_MEAS_MAX_LEN            (13)


#define GATTC_TAG                   "GATTC_SPP_DEMOclient:"
#define PROFILE_NUM                 1
#define PROFILE_APP_ID              0
#define BT_BD_ADDR_STR              "%02x:%02x:%02x:%02x:%02x:%02x"
#define BT_BD_ADDR_HEX(addr)        addr[0],addr[1],addr[2],addr[3],addr[4],addr[5]
#define ESP_GATT_SPP_SERVICE_UUID   0xABF0
#define SCAN_ALL_THE_TIME           0


#define HRPS_HT_MEAS_MAX_LEN        (13)

#define HRPS_MANDATORY_MASK         (0x0F)
#define HRPS_BODY_SENSOR_LOC_MASK   (0x30)
#define HRPS_HR_CTNL_PT_MASK        (0xC0)

#define HEART_PROFILE_NUM 			    1
#define ESP_HEART_RATE_APP_ID		  	0x55
#define SAMPLE_DEVICE_NAME          "ESP_CLIENT_SENSE"
#define HEART_RATE_SVC_INST_ID	   	0
#define HEART_PROFILE_APP_IDX 			0

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <stdarg.h>
#include <math.h>

#include "esp_int_wdt.h"
#include "esp_system.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt.h"
#include "esp_bt_device.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_gatt_common_api.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "peripheral.h"
#include "spg30.h"
#include "temperature.h"
//#include "ct-sensor.h"
#include "dht22.h"

#include <string.h>
#include <stdio.h>
#include "argtable3/argtable3.h"
#include "driver/uart.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "soc/rtc_wdt.h"
#include "soc/timer_group_reg.h"

#include "peripheral.h"


struct gattc_profile_inst {
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
};
enum{
    SPP_IDX_SVC,

    SPP_IDX_SPP_DATA_RECV_VAL,

    SPP_IDX_SPP_DATA_NTY_VAL,
    SPP_IDX_SPP_DATA_NTF_CFG,

    SPP_IDX_SPP_COMMAND_VAL,

    SPP_IDX_SPP_STATUS_VAL,
    SPP_IDX_SPP_STATUS_CFG,

#ifdef SUPPORT_HEARTBEAT
    SPP_IDX_SPP_HEARTBEAT_VAL,
    SPP_IDX_SPP_HEARTBEAT_CFG,
#endif

    SPP_IDX_NB,
};
enum
{
    HRS_IDX_SVC,

    HRS_IDX_HR_MEAS_CHAR,
    HRS_IDX_HR_MEAS_VAL,
    HRS_IDX_HR_MEAS_NTF_CFG,

    HRS_IDX_BOBY_SENSOR_LOC_CHAR,
    HRS_IDX_BOBY_SENSOR_LOC_VAL,

    HRS_IDX_HR_CTNL_PT_CHAR,
    HRS_IDX_HR_CTNL_PT_VAL,

    HRS_IDX_NB,
};




// static uint8_t heart_rate_service_uuid[16] = {
//     /* LSB <--------------------------------------------------------------------------------> MSB */
//     //first uuid, 16bit, [12],[13] is the value
//     0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0x18, 0x0D, 0x00, 0x00,
// };

static esp_ble_adv_data_t heart_rate_adv_config = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x20,
    .max_interval = 0x40,
    .appearance = 0x00,
    .manufacturer_len = 0, //TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data =  NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 32,
    .p_service_uuid = heart_rate_service_uuid,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

struct gatts_profile_inst {
    esp_gatts_cb_t gatts_cb;
    uint16_t gatts_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_handle;
    esp_bt_uuid_t char_uuid;
    esp_gatt_perm_t perm;
    esp_gatt_char_prop_t property;
    uint16_t descr_handle;
    esp_bt_uuid_t descr_uuid;
};


/*
 *  Heart Rate PROFILE ATTRIBUTES
 ****************************************************************************************
 */

// Heart Rate Sensor Service
static const uint16_t heart_rate_svc = ESP_GATT_UUID_HEART_RATE_SVC;

#define CHAR_DECLARATION_SIZE   (sizeof(uint8_t))
static const uint16_t primary_service_uuid = ESP_GATT_UUID_PRI_SERVICE;
static const uint16_t character_declaration_uuid = ESP_GATT_UUID_CHAR_DECLARE;
static const uint16_t character_client_config_uuid = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
static const uint8_t char_prop_notify = ESP_GATT_CHAR_PROP_BIT_NOTIFY;
static const uint8_t char_prop_read = ESP_GATT_CHAR_PROP_BIT_READ;
static const uint8_t char_prop_read_write = ESP_GATT_CHAR_PROP_BIT_WRITE|ESP_GATT_CHAR_PROP_BIT_READ;

/// Heart Rate Sensor Service - Heart Rate Measurement Characteristic, notify
// static const uint16_t heart_rate_meas_uuid = ESP_GATT_HEART_RATE_MEAS;
// static const uint8_t heart_measurement_ccc[2] ={ 0x00, 0x00};


/// Heart Rate Sensor Service -Body Sensor Location characteristic, read
// static const uint16_t body_sensor_location_uuid = ESP_GATT_BODY_SENSOR_LOCATION;
// static const uint8_t body_sensor_loc_val[1] = {0x00};


/// Heart Rate Sensor Service - Heart Rate Control Point characteristic, write&read
// static const uint16_t heart_rate_ctrl_point = ESP_GATT_HEART_RATE_CNTL_POINT;
// static const uint8_t heart_ctrl_point[1] = {0x00};


/// Full HRS Database Description - Used to add attributes into the database
/*static const esp_gatts_attr_db_t heart_rate_gatt_db[HRS_IDX_NB] =
{
    // Heart Rate Service Declaration
    [HRS_IDX_SVC]                      	=  
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&primary_service_uuid, ESP_GATT_PERM_READ,
      sizeof(uint16_t), sizeof(heart_rate_svc), (uint8_t *)&heart_rate_svc}},

    // Heart Rate Measurement Characteristic Declaration
    [HRS_IDX_HR_MEAS_CHAR]            = 
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
      CHAR_DECLARATION_SIZE,CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_notify}},
      
    // Heart Rate Measurement Characteristic Value
    [HRS_IDX_HR_MEAS_VAL]             	=   
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&heart_rate_meas_uuid, ESP_GATT_PERM_READ,
      HRPS_HT_MEAS_MAX_LEN,0, NULL}},

    // Heart Rate Measurement Characteristic - Client Characteristic Configuration Descriptor
    [HRS_IDX_HR_MEAS_NTF_CFG]     	=    
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_client_config_uuid, ESP_GATT_PERM_READ|ESP_GATT_PERM_WRITE,
      sizeof(uint16_t),sizeof(heart_measurement_ccc), (uint8_t *)heart_measurement_ccc}},

    // Body Sensor Location Characteristic Declaration
    [HRS_IDX_BOBY_SENSOR_LOC_CHAR]  = 
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
      CHAR_DECLARATION_SIZE,CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read}},

    // Body Sensor Location Characteristic Value
    [HRS_IDX_BOBY_SENSOR_LOC_VAL]   = 
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&body_sensor_location_uuid, ESP_GATT_PERM_READ,
      sizeof(uint8_t), sizeof(body_sensor_loc_val), (uint8_t *)body_sensor_loc_val}},

    // Heart Rate Control Point Characteristic Declaration
    [HRS_IDX_HR_CTNL_PT_CHAR]          = 
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&character_declaration_uuid, ESP_GATT_PERM_READ,
      CHAR_DECLARATION_SIZE,CHAR_DECLARATION_SIZE, (uint8_t *)&char_prop_read_write}},
                                         			
    // Heart Rate Control Point Characteristic Value
    [HRS_IDX_HR_CTNL_PT_VAL]             = 
    {{ESP_GATT_AUTO_RSP}, {ESP_UUID_LEN_16, (uint8_t *)&heart_rate_ctrl_point, ESP_GATT_PERM_WRITE|ESP_GATT_PERM_READ,
      sizeof(uint8_t), sizeof(heart_ctrl_point), (uint8_t *)heart_ctrl_point}},  
};


#endif