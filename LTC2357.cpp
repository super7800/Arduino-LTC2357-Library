#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>
#include "LTC2357.h"

LTC2357::LTC2357(uint8_t cs_pin, uint8_t cnv_pin, uint8_t busy_pin) {

  _cs_pin = cs_pin;
  _cnv_pin = cnv_pin;
  _busy_pin = busy_pin;

}

void LTC2357::begin() {


    pinMode(_cnv_pin, OUTPUT);
    pinMode(_busy_pin, INPUT);
    pinMode(_cs_pin, OUTPUT);
    digitalWrite(_cs_pin, HIGH);

    uint32_t config_word = 0;
    uint8_t num = 7;

    for (int i = 0; i < NUM_OF_CHANNELS; ++i) {
        create_config_word(i, num, &config_word);
    }

    configuration_bits.LT_uint32 = config_word;
}

void LTC2357::create_config_word(uint8_t channel, uint8_t config_number, uint32_t *config_word) {
  *config_word = *config_word | (uint32_t(config_number & 0x07) << (channel * 3));
}

int32_t sign_extend_17(uint32_t data)
{
  uint8_t sign;
  uint32_t mask = 0x20000;
  int32_t data_signed = data;
  sign = (data & mask) >> 17;
  if (sign)
    data_signed = data_signed | 0xFFFC0000;
  return data_signed;
}

// Calculates the voltage from ADC output data depending on the channel configuration
float LTC2357::voltage_calculator(uint32_t data, uint8_t channel_configuration) {

  float voltage;
  int32_t data_signed;
  switch (channel_configuration)
  {
    case 0:
      voltage = 0;
      break;   // Disable Channel
    case 1:
      voltage = (float)data * (1.25 * VREF / 1.000) / POW2_18;
      break;
    case 2:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (1.25 * VREF / 1.024) / POW2_17;
      break;
    case 3:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (1.25 * VREF / 1.000) / POW2_17;
      break;
    case 4:
      voltage = (float)data * (2.50 * VREF / 1.024) / POW2_18;
      break;
    case 5:
      voltage = (float)data * (2.50 * VREF / 1.000) / POW2_18;
      break;
    case 6:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (2.50 * VREF / 1.024) / POW2_17;
      break;
    case 7:
      data_signed = sign_extend_17(data);
      voltage = (float)data_signed * (2.50 * VREF ) / POW2_17;
      break;
  }
  return voltage;
}

void LTC2357::read() {

  int i, j;
  uint8_t tx_array[24];

  uint8_t Result[24];
  uint8_t pos = 23;
  data.LT_uint32 = 0;

  // Start conversion by toggling CNV pin
  digitalWrite(_cnv_pin, HIGH);   // Set CNV high to initiate conversion
  delayMicroseconds(10);          // Short delay (CNV pulse width)
  digitalWrite(_cnv_pin, LOW);    // Set CNV low to end the pulse

  tx_array[23] = (uint8_t)(configuration_bits.LT_uint32 >> 16);
  tx_array[22] = (uint8_t)(configuration_bits.LT_uint32 >> 8);
  tx_array[21] = (uint8_t)(configuration_bits.LT_uint32);
  for (i = 20; i >= 0; --i)
  {
    tx_array[i] = 0;
  }
  spi_transfer_block(tx_array, Result, 24);

  for (int i = 0; i < NUM_OF_CHANNELS; ++i) {
    data.LT_byte[2] = Result[pos--];
    data.LT_byte[1] = Result[pos--];
    data.LT_byte[0] = Result[pos--];

    ADC_VALUES[i] = decode_values(data.LT_uint32);
  }

}

// Reads and sends a byte array
void LTC2357::spi_transfer_block(uint8_t *tx, uint8_t *rx, uint8_t length) {
  int8_t i;

  digitalWrite(_cs_pin, LOW);                 //! 1) Pull CS low

  for (i=(length-1);  i >= 0; i--)
    rx[i] = SPI.transfer(tx[i]);    //! 2) Read and send byte array

  digitalWrite(_cs_pin, HIGH);                //! 3) Pull CS high
}

float LTC2357::decode_values(uint32_t channel_data) {
    uint8_t channel = (channel_data & CHANNEL_NUMBER) >> 3;
    uint8_t config_number = channel_data & SOFTSPAN;
    uint32_t code = (channel_data & 0xFFFFC0) >> 6;
    return voltage_calculator(code, config_number);
}

float* LTC2357::get_adc_values() {
    read();  // Call the private read method
    return ADC_VALUES;  // Return the array of ADC values
}




/*

CONFIGURATION SETTINGS Vref = 4.096V
|Config Number| SS2 | SS1 | SS0 | ANALOG INPUT RANGE      | DIGITAL COMPRESSION | RESULT BINARY FORMAT |
|-------------|-----------------|-------------------------|---------------------|----------------------|
|      0      |  0  |  0  |  0  | Disable Channel         | N/A                 | All Zeros            |
|      1      |  0  |  0  |  1  | 0 - 1.25 Vref           | 1                   | Straight Binary      |
|      2      |  0  |  1  |  0  | -1.25 Vref - +1.25 Vref | 1/1.024             | Two's Complement     |
|      3      |  0  |  1  |  1  | -1.25 Vref - +1.25 Vref | 1                   | Two's Complement     |
|      4      |  1  |  0  |  0  | 0 - 2.5 Vref            | 1/1.024             | Straight Binary      |
|      5      |  1  |  0  |  1  | 0 - 2.5 Vref            | 1                   | Straight Binary      |
|      6      |  1  |  1  |  0  | -2.5 Vref - +2.5 Vref   | 1/1.024             | Two's Complement     |
|      7      |  1  |  1  |  1  | -2.5 Vref - +2.5 Vref   | 1                   | Two's Complement     |

*/







