#ifndef LTC2357_H
#define LTC2357_H

#include <Arduino.h>
#include <stdint.h>
#include <SPI.h>



class LTC2357 {
public:
    LTC2357(uint8_t cs_pin, uint8_t cnv_pin, uint8_t busy_pin);
    void begin();
    float* get_adc_values();  // New public function to get ADC values

private:

    #define VREF 5
    #define POW2_18 262144
    #define POW2_17 131072
    #define NUM_OF_CHANNELS 4
    #define num_of_bits 18

    // Define channel constants
    #define CHANNEL_NUMBER 0x38
    #define SOFTSPAN 0x07

    // ADC Data and channel config
    union LT_union_int32_4bytes {
      uint32_t LT_uint32;    // To access as a single 32-bit integer
      uint8_t LT_byte[4];    // To access as an array of bytes
    };

    // Declare instances of the union
    LT_union_int32_4bytes configuration_bits;
    LT_union_int32_4bytes data;

    // Array to hold ADC values
    float ADC_VALUES[NUM_OF_CHANNELS];

    uint8_t _cs_pin;
    uint8_t _cnv_pin;
    uint8_t _busy_pin;
    void create_config_word(uint8_t channel, uint8_t config_number, uint32_t *config_word);
    void read();
    float decode_values(uint32_t channel_data);
    float voltage_calculator(uint32_t data, uint8_t channel_configuration);
    void spi_transfer_block(uint8_t *tx, uint8_t *rx, uint8_t length);
};

#endif // LTC2357_H