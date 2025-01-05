#ifndef I2CIP_HT16K33_H_
#define I2CIP_HT16K33_H_

#include <Arduino.h>
#include <I2CIP.h>

// MACROS

// Registers
#define HT16K33_REG_BRIGHTNESS  0xE0
#define HT16K33_REG_BLINK       0x80
#define HT16K33_DISPLAY_ON    0x01
#define HT16K33_BLINK_OFF     0x00 // No blink; all on
#define HT16K33_BLINK_2HZ     1
#define HT16K33_BLINK_1HZ     2
#define HT16K33_BLINK_HALFHZ  3

// Commands
#define HT16K33_CMD_OSCILLATOR 0x21 // Turn on oscillator

// #define I2CIP_HT16K33_ADDRESS 0x70 /// default I2C address for HT16K33; interferes with MUX range 0x70-0x77; instead we "fake-out" 0x77 MUX devices
#define I2CIP_HT16K33_ADDRESS 119 // 0x77; I2CIP-supported address (MUX with this addr are "faked-out" by the library)
#define I2CIP_HT16K33_7SEGNUM 4 ///< # Digits in 7-seg displays

// const char i2cip_ht16k33_id_progmem[] PROGMEM = {"HT16K33"};

typedef enum i2cip_ht16k33_mode_s {
  SEG_BLANK = 0,
  SEG_HEX16, // 4 byte uint32
  SEG_UINT, // Max 2 byte uint16_t
  SEG_INT, // Max 2 byte uint16_t
  SEG_ASCII, // char s[4]; no null-terminator
  SEG_1F, // float, XXX.X
  SEG_2F, // XX.XX
  SEG_3F, // XXX.X
  SEG_SNAKE // Easter egg
} i2cip_ht16k33_mode_t; // Implies sizes {, , , char s[4], float...} with different precisions

// union type for 7-segment display data for each mode
typedef union {
  uint32_t h; // 4 bytes; cast to int32_t
  float f; // Also 4 bytes
} i2cip_ht16k33_data_t; // h, b, s

// Takes 4 
class HT16K33 : public I2CIP::Device, public I2CIP::OutputInterface<i2cip_ht16k33_data_t, i2cip_ht16k33_mode_t> {
  I2CIP_DEVICE_CLASS_BUNDLE(HT16K33);
  I2CIP_OUTPUT_USE_FAILSAFE(i2cip_ht16k33_data_t, i2cip_ht16k33_mode_t);
  private:
    // HT16K33(i2cip_fqa_t fqa) : I2CIP::Device(fqa, i2cip_ht16k33_id_progmem, _id), I2CIP::OutputInterface<uint8_t*, size_t>((I2CIP::Device*)this) { }
    HT16K33(i2cip_fqa_t fqa, const i2cip_id_t& id);

    uint32_t segmentMaps = 0; // All off

    template <i2cip_ht16k33_mode_t M> void setSegments(i2cip_ht16k33_data_t buf, bool overwrite = true);

    i2cip_errorlevel_t blink(const uint8_t& b = HT16K33_BLINK_OFF, bool setbus = false);

    i2cip_errorlevel_t brightness(const uint8_t& b = 15, bool setbus = false);

    i2cip_errorlevel_t begin(bool setbus = true) override; // virtual Device::begin

    i2cip_errorlevel_t writeSegments(bool setbus = false);

  public:
    // HT16K33(i2cip_fqa_t fqa, const i2cip_id_t& id); // Moved to private; See I2CIP_HT16K33_ADDRESS note comment
    HT16K33(const uint8_t& wire, const uint8_t& mux, const uint8_t& bus, const i2cip_id_t& id) : HT16K33(I2CIP::createFQA(wire, mux, bus, I2CIP_HT16K33_ADDRESS), id) { }
    HT16K33(const uint8_t& wire, const i2cip_id_t& id) : HT16K33(I2CIP::createFQA(wire, I2CIP_MUX_NUM_FAKE, I2CIP_MUX_BUS_FAKE, I2CIP_HT16K33_ADDRESS), id) { }

    i2cip_errorlevel_t set(i2cip_ht16k33_data_t const& buf, const i2cip_ht16k33_mode_t& mode) override;

    // Static API
    static i2cip_errorlevel_t _begin(const i2cip_fqa_t& fqa, bool setbus); // Passed to Device::Device()
    static i2cip_errorlevel_t _writeSegments(const i2cip_fqa_t& fqa, const uint32_t& segments, bool setbus);
    static i2cip_errorlevel_t _brightness(const i2cip_fqa_t& fqa, const uint8_t& b, bool setbus);
    static i2cip_errorlevel_t _blink(const i2cip_fqa_t& fqa, const uint8_t& b, bool setbus);
};

#endif