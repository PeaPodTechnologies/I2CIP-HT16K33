#ifndef I2CIP_HT16K33_H_
#define I2CIP_HT16K33_H_

#include <Arduino.h>
#include <I2CIP.h>

// MACROS

// Registers
#define HT16K33_REG_BRIGHTNESS  0xE0
#define HT16K33_REG_BLINK       0x80
#define HT16K33_BLINK_DISPLAYON   0x01 // I2C value for steady on
#define HT16K33_BLINK_DISPLAYOFF  0x00 // I2C value for steady off
#define HT16K33_BLINK_2HZ     1
#define HT16K33_BLINK_1HZ     2
#define HT16K33_BLINK_HALFHZ  3

// Commands
#define HT16K33_CMD_OSCILLATOR 0x21 // Turn on oscillator

// #define I2CIP_HT16K33_ADDRESS 0x70 /// default I2C address for HT16K33; interferes with MUX range 0x70-0x77; instead we "fake-out" 0x77 MUX devices
#define I2CIP_HT16K33_ADDRESS 0x77 // I2CIP-supported address (MUX with this addr are "faked-out" by the library)
#define I2CIP_HT16K33_7SEGNUM 4 ///< # Digits in 7-seg displays

// const char i2cip_ht16k33_id_progmem[] PROGMEM = {"HT16K33"};

typedef enum { 7SEG_BLANK, 7SEG_HEX32, 7SEG_UINT, 7SEG_INT, 7SEG_STR4, 7SEG_1F, 7SEG_2F, 7SEG_3F } i2cip_ht16k33_mode_t; // Implies sizes {4 byte uint32, 2 byte uint8_t, 2 byte int16_t, char s[4], float...} with different precisions

// union type for 7-segment display data for each mode
typedef union {
  uint32_t h;
  uint8_t b[4];
  char s[4];
  float f;
} i2cip_ht16k33_data_t; // h, b, s

char hexChar(uint8_t hex) {
  hex &= 0x0F;
  if(hex < 10) return (char)(hex + '0');
  return (char)(hex - 10 + 'A');
}

// Takes 4 
class HT16K33 : public I2CIP::Device, public I2CIP::OutputInterface<i2cip_ht16k33_data_t, i2cip_ht16k33_mode_t> {
  I2CIP_DEVICE_CLASS_BUNDLE(HT16K33);
  I2CIP_OUTPUT_USE_FAILSAFE(i2cip_ht16k33_data_t, i2cip_ht16k33_mode_t);
  private:
    // HT16K33(i2cip_fqa_t fqa) : I2CIP::Device(fqa, i2cip_ht16k33_id_progmem, _id), I2CIP::OutputInterface<uint8_t*, size_t>((I2CIP::Device*)this) { }
    HT16K33(i2cip_fqa_t fqa, const i2cip_id_t& id);

    i2cip_ht16k33_data_t segmentMaps = 0x00000000; // All off 32-bit

    bool initialized = false;

    template <i2cip_ht16k33_mode_t M> void setSegments(i2cip_ht16k33_data_t const& buf, bool overwrite = true);

    i2cip_errorlevel_t clear(bool setbus = false) {
      uint8_t buffer[16] = { 0x00 };
      return writeRegister((uint8_t)0x00, buffer, 16, setbus);
    }

    i2cip_errorlevel_t blink(uint8_t b = HT16K33_BLINK_OFF, bool setbus = false) {
      if (b > HT16K33_BLINK_HALFHZ) return I2CIP::I2CIP_ERR_SOFT;

      return writeByte(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1);, setbus);
    }

    i2cip_errorlevel_t brightness(float _b = 1.0f, bool setbus = false) {
      _b = constrain(b, 0.0f, 1.0f);
      uint8_t b = (uint8_t)(_b * 15.0 + 0.5); // 0-15 byte

      return brightness(b, setbus);
    }

    i2cip_errorlevel_t brightness(uint8_t b = 15, bool setbus = false) {
      if(b > 15) b = 15;

      return writeByte(HT16K33_REG_BRIGHTNESS | b, setbus);
    }

    i2cip_errorlevel_t off(bool setbus = false) {
      return brightness(0x00, setbus);
    }

    i2cip_errorlevel_t begin(bool setbus = false) {
      // Oscillator
      i2cip_errorlevel_t errlev = writeByte(HT16K33_CMD_OSCILLATOR, setbus);
      I2CIP_ERR_BREAK(errlev);

      // Turn On
      brightness();

      setSegments<7SEG_STR4>("    ", true);
    }

  public:
    // HT16K33(i2cip_fqa_t fqa, const i2cip_id_t& id); // Moved to private; See I2CIP_HT16K33_ADDRESS note comment
    HT16K33(uint8_t wire, uint8_t mux, uint8_t bus, const i2cip_id_t& id) : HT16K33(createFQA(wire, mux, bus, I2CIP_HT16K33_ADDRESS), id) { }

    i2cip_errorlevel_t set(i2cip_ht16k33_data_t const& buf, const i2cip_ht16k33_mode_t& mode) override;
};

#endif