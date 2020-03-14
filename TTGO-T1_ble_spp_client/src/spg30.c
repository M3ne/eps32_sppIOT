
#include "spg30.h"


uint8_t spg30_generateCRC(uint8_t *data, uint8_t datalen);
esp_err_t spg30_execCommand(i2c_port_t i2c_num, uint8_t command[],
                     uint8_t commandLength,
                     bool ack_mstr_req,
                     uint16_t delayms,
                     uint16_t *readdata,
                     uint8_t readlen);
esp_err_t spg30_IAQinit(i2c_port_t i2c_num);


uint8_t serialnumber[10];
uint8_t featureset[10];

esp_err_t spg30_init(i2c_port_t i2c_num){
  esp_err_t err_detail;
  
  /********************************************************************************************************************************
    *       Get Serial ID                                                                                                           *
    *                                                                                                                               *
    * The readout of the serial ID register can be used to identify the chip and verify the presence of the sensor. The appropriate *
    * command structure is shown in Table 11. After issuing the measurement command and sending the ACK Bit the sensor              *
    * needs the time tIDLE = 0.5ms to respond to the I2C read header with an ACK Bit. Hence, it is recommended to wait tIDLE =0.5ms *
    * before issuing the read header.                                                                                               *
    * The get serial ID command returns 3 words, and every word is followed by an 8-bit CRC checksum. Together the 3 words          *
    * constitute a unique serial ID with a length of 48 bits.                                                                       *
    * The ID returned with this command are represented in the big endian (or MSB first) format                                     *
    *********************************************************************************************************************************/
  uint8_t command[3];
  command[0] = 0x36;
  command[1] = 0x82;
  
  if ((err_detail=spg30_execCommand(i2c_num, command, 2, true, 10, serialnumber, 3)) != ESP_OK)
      return err_detail;

  /* printf("GET SERIAL ID\n");
  printf("===> ANSWER: 0x%02x%02x 0x%02x%02x 0x%02x%02x \n", serialnumber[0], serialnumber[1], serialnumber[3],serialnumber[4],serialnumber[6],serialnumber[7]);
  printf ("===> CRC4wr: 0x%02x     0x%02x     0x%02x    \n", serialnumber[2], serialnumber[5], serialnumber[8]);
  */
  /********************************************************************************************************************************
    *       FEATURE SET                                                                                                             *
    *                                                                                                                               *
    * The SGP30 features a versioning system for the available set of measurement commands and on-chip algorithms. This so          *
    * called feature set version number can be read out by sending a “Get_feature_set_version” command. The sensor responds         *
    * with 2 data bytes (MSB first) and 1 CRC byte. This feature set version number is used to refer to a corresponding set of      *
    * available measurement commands as listed in Table 9.                                                                          *
    *********************************************************************************************************************************/
  
  command[0] = 0x20;
  command[1] = 0x2F;
  if ((err_detail=spg30_execCommand(i2c_num, command, 2, true, 10, featureset, 1)) != ESP_OK)
      return err_detail;

  //if (((((uint16_t)featureset[1])<<8)+(featureset[1] & 0xF0)) != SGP30_FEATURESET)
  { //NOTE: from datasheet 0x0009 but could be updated for new devices releases.
    //            if the FeatureSet is different, must add/modify the instruction IDs
    //            or the protocol command values.
  //  return false;
  }

  /* printf("FEATURE SET\n");
  printf("===> ANSWER: 0x%02x%02x \n", featureset[0], featureset[1]);
  pr intf("===> CRC4wr: 0x%02x  \n", featureset[2]);
  */
  if ((err_detail=spg30_IAQinit(i2c_num)) != ESP_OK)
        return err_detail; 

  return ESP_OK;
}


/*!
 *   @brief  Commands the sensor to begin the IAQ algorithm. Must be called
 *           after startup.
 *   @param i2c_num:
 *              i2c_port_t identifier about the I2C port to be used.
 * 
 *   @returns esp_err_t value, ESP_OK if command completed successfully, otherwise the value identify the problem. 
 */
esp_err_t spg30_IAQinit(i2c_port_t i2c_num) {
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x03;
  return spg30_execCommand(i2c_num, command, 2, true, 10, command, 0);
  vTaskDelay(300/portTICK_PERIOD_MS);
}


/*!
 *  @brief  Commands the sensor to take a single eCO2/VOC measurement. Places
 *          results in {@link TVOC} and {@link eCO2}
 * 
 *  @param i2c_num:
 *              i2c_port_t identifier about the I2C port to be used.
 *  @param  *TVOC: 
 *            uint16_t pointer of IAQ-calculated Total Volatile Organic
 *            Compounds in ppb. This value is set when you call {@link IAQmeasure()}
 *  @param  *eCO2: 
 *            uint16_t pointer of IAQ-calculated equivalent CO2 in ppm. This
 *            value is set when you call {@link IAQmeasure()}
 * 
 *  @returns esp_err_t value, ESP_OK if command completed successfully, otherwise the value identify the problem. 
 * 
 *******************************************************************************/
esp_err_t spg30_IAQmeasure(i2c_port_t i2c_num, uint16_t *TVOC, uint16_t *eCO2) {
  esp_err_t err_detail;
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x08;
  uint8_t reply[6]; // 2 word (4 byte) + 2 byte crc8 (1 each word)

  if ((err_detail=spg30_execCommand(i2c_num, command, 2, true, 100, reply, 2))!= ESP_OK)
    return err_detail;
  
  *TVOC = (((uint16_t)reply[3])<<8) + reply[4];
  *eCO2 = (((uint16_t)reply[0])<<8) + reply[1];

  // Check CRCs
  uint8_t crc=0;
  crc = spg30_generateCRC(reply,2);
  if(crc != reply[2])
    return ESP_ERR_INVALID_CRC;
  crc = spg30_generateCRC(reply+3,2);
  if(crc != reply[5])
    return ESP_ERR_INVALID_CRC;

  return ESP_OK;
}

 /*!
  *  @brief  Commands the sensor to take a single H2/ethanol raw measurement. Places results in {@link rawH2} and {@link rawEthanol}
  *  
 *   @param i2c_num:
 *              i2c_port_t identifier about the I2C port to be used.
  *  @param uint16_t *rawH2:
  *  @param uint16_t *rawEthanol: 
  * 
  *  @returns esp_err_t value, ESP_OK if command completed successfully, otherwise the value identify the problem. 
  */
esp_err_t spg30_IAQmeasureRaw(i2c_port_t i2c_num, uint16_t *rawEthanol, uint16_t *rawH2) {
    esp_err_t err_detail;
    uint8_t command[2];
    command[0] = 0x20;
    command[1] = 0x50;
    uint8_t reply[6]; // 2 word (4 byte) + 2 byte crc8 (1 each word)

    if ((err_detail=spg30_execCommand(i2c_num, command, 2, true, 100, reply, 2))!= ESP_OK)
      return err_detail;

    //TODO: verify the CRC8 cor rawEthanol value (reply[5] <- crc)  
    *rawEthanol = (((uint16_t)reply[3])<<8) + reply[4];
    //TODO: verify the CRC8 cor rawH2 value (reply[2] <- crc)
    *rawH2 = (((uint16_t)reply[0])<<8) + reply[1];

    // Check CRCs
    uint8_t crc=0;
    crc = spg30_generateCRC(reply,2);
    if(crc != reply[2])
      return ESP_ERR_INVALID_CRC;
    crc = spg30_generateCRC(reply+3,2);
    if(crc != reply[5])
      return ESP_ERR_INVALID_CRC;

    return ESP_OK;
} 


/*!
 *   @brief  Request baseline calibration values for both CO2 and TVOC IAQ
 *           calculations. Places results in parameter memory locaitons.
 *   @param i2c_num:
 *            i2c_port_t identifier about the I2C port to be used.
 *   @param  *eco2_base 
 *           A uint16_t pointer to a uint16_t which we will save the calibration
 *           value to
 *  @param   *tvoc_base 
 *           A uint16_t pointer to a uint16_t which we will save the calibration value to
 * 
 *  @returns esp_err_t value, ESP_OK if command completed successfully, otherwise the value identify the problem. 
 *
 */
esp_err_t spg30_getIAQBaseline(i2c_port_t i2c_num, uint16_t *eco2_base, uint16_t *tvoc_base) {
  esp_err_t err_detail;
  uint8_t command[2];
  command[0] = 0x20;
  command[1] = 0x15;
  uint8_t reply[6];
  if ((err_detail=spg30_execCommand(i2c_num, command, 2, true, 100, reply, 2))!= ESP_OK)
     return err_detail;

  
  //TODO: verify the CRC8 cor eco2_base value (reply[2] <- crc)   
  *eco2_base = (((uint16_t)reply[0])<<8) + reply[1];
  //TODO: verify the CRC8 cor tvoc_base value (reply[5] <- crc) 
  *tvoc_base = (((uint16_t)reply[3])<<8) + reply[4];
  
  // Check CRCs
  uint8_t crc=0;
  crc = spg30_generateCRC(reply,2);
  if(crc != reply[2])
    return ESP_ERR_INVALID_CRC;
  crc = spg30_generateCRC(reply+3,2);
  if(crc != reply[5])
    return ESP_ERR_INVALID_CRC;

  return ESP_OK;
} 




/*!
 *  @brief  Assign baseline calibration values for both CO2 and TVOC IAQ
 *          calculations.
 * 
 *   @param i2c_num:
 *            i2c_port_t identifier about the I2C port to be used.
 *  @param  eco2_base 
 *          A uint16_t which we will save the calibration value from
 *  @param  tvoc_base 
 *          A uint16_t which we will save the calibration value from
 *  
 *  @returns esp_err_t value, ESP_OK if command completed successfully, otherwise the value identify the problem. 
 */
esp_err_t spg30_setIAQBaseline(i2c_port_t i2c_num, uint16_t eco2_base, uint16_t tvoc_base) {
  uint8_t command[8];
  command[0] = 0x20;
  command[1] = 0x1e;
  command[2] = tvoc_base >> 8;
  command[3] = tvoc_base & 0xFF;
  command[4] = spg30_generateCRC(command + 2, 2);
  command[5] = eco2_base >> 8;
  command[6] = eco2_base & 0xFF;
  command[7] = spg30_generateCRC(command + 5, 2);

  return spg30_execCommand(i2c_num, command, 8, true, 100, command, 0);
}


/*!
 *  @brief  Set the absolute humidity value [mg/m^3] for compensation to increase
 *          precision of TVOC and eCO2.
 *   
 *  @param i2c_num:
 *            i2c_port_t identifier about the I2C port to be used.
 *  @param  absolute_humidity 
 *          A uint32_t [mg/m^3] which we will be used for compensation.
 *          If the absolute humidity is set to zero, humidity compensation
 *          will be disabled.
 * 
 *  @return True if command completed successfully, false if something went
 *          wrong!
 */
esp_err_t spg30_setHumidity(i2c_port_t i2c_num, uint32_t absolute_humidity) {
  if (absolute_humidity > 256000) {
    return false;
  }

  uint16_t ah_scaled =
      (uint16_t)(((uint64_t)absolute_humidity * 256 * 16777) >> 24);
  uint8_t command[5];
  command[0] = 0x20;
  command[1] = 0x61;
  command[2] = ah_scaled >> 8;
  command[3] = ah_scaled & 0xFF;
  command[4] = spg30_generateCRC(command + 2, 2);

  return spg30_execCommand(i2c_num, command, 5, true, 100, command, 0);
} 


/*
    TODO: add description
    substitute bool with esp_err_t
   NOTE: including the CRC om readLem
*/
esp_err_t spg30_execCommand(i2c_port_t i2c_num, uint8_t command[], uint8_t commandLength, bool ack_mstr_req, uint16_t delayms, uint16_t *readdata, uint8_t readlen){
    esp_err_t err_detail;
    i2c_cmd_handle_t i2c_cmd_w = i2c_cmd_link_create();
    if ((err_detail=i2c_master_start(i2c_cmd_w)) != ESP_OK)
        return err_detail;

    if ((err_detail=i2c_master_write_byte(i2c_cmd_w, (SGP30_I2CADDR_DEFAULT << 1 | I2C_MASTER_WRITE), ack_mstr_req)) != ESP_OK)
        return err_detail;

    for (int x = 0; x < commandLength; x++){ 
      if ((err_detail=i2c_master_write_byte(i2c_cmd_w, command[x], ack_mstr_req)) != ESP_OK)
          return err_detail;
    }
    if (i2c_master_stop(i2c_cmd_w) != ESP_OK)
        return err_detail;

    if ((err_detail=i2c_master_cmd_begin(i2c_num, i2c_cmd_w, delayms)) != ESP_OK)
        return err_detail;
      
    i2c_cmd_link_delete(i2c_cmd_w);
    if (readlen == 0)
    {return ESP_OK;}

    vTaskDelay(delayms / portTICK_PERIOD_MS);

    //-----------
    //  Handling response
    //------------
    i2c_cmd_handle_t i2c_cmd_r = i2c_cmd_link_create();
    uint8_t replylen = readlen * (SGP30_WORD_LEN + 1); //  2 byte and after the crc

    if((err_detail=i2c_master_start(i2c_cmd_r)) != ESP_OK)
        return err_detail; 
    if((err_detail=i2c_master_write_byte(i2c_cmd_r,(SGP30_I2CADDR_DEFAULT<<1 | I2C_MASTER_READ), ack_mstr_req)) != ESP_OK)
        return err_detail;
    if((err_detail=i2c_master_read(i2c_cmd_r,readdata,replylen,false)) != ESP_OK)
        return err_detail;
    if((err_detail=i2c_master_stop(i2c_cmd_r)) != ESP_OK)
        return err_detail;

    if((err_detail=i2c_master_cmd_begin(i2c_num, i2c_cmd_r, delayms)) != ESP_OK)
        return err_detail;

    
    i2c_cmd_link_delete(i2c_cmd_r);
 
    return ESP_OK;
}

uint8_t spg30_generateCRC(uint8_t *data, uint8_t datalen) {
  // calculates 8-Bit checksum with given polynomial
  uint8_t crc = SGP30_CRC8_INIT;

  for (uint8_t i = 0; i < datalen; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++) {
      if (crc & 0x80)
        crc = (crc << 1) ^ SGP30_CRC8_POLYNOMIAL;
      else
        crc <<= 1;
    }
  }
  return crc;
}