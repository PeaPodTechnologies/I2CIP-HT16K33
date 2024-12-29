#include <HT16K33.h>

#define SEVENSEG_OFFSET_NUMERICAL 16
#define SEVENSEG_OFFSET_ASCII -32

static const PROGMEM uint8_t ascii[] = {

    0b00000000, // (space) - Ascii 32 at index 0; See SEVENSEG_OFFSET_ASCII
    0b10000110, // !
    0b00100010, // "
    0b01111110, // #
    0b01101101, // $
    0b11010010, // %
    0b01000110, // &
    0b00100000, // '
    0b00101001, // (
    0b00001011, // )
    0b00100001, // *
    0b01110000, // +
    0b00010000, // ,
    0b01000000, // -
    0b10000000, // .
    0b01010010, // /
    0b00111111, // 0 - First numerical; See SEVENSEG_OFFSET_NUMERICAL
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111, // 9
    0b00001001, // :
    0b00001101, // ;
    0b01100001, // <
    0b01001000, // =
    0b01000011, // >
    0b11010011, // ?
    0b01011111, // @
    0b01110111, // A
    0b01111100, // B
    0b00111001, // C
    0b01011110, // D
    0b01111001, // E
    0b01110001, // F
    0b00111101, // G
    0b01110110, // H
    0b00110000, // I
    0b00011110, // J
    0b01110101, // K
    0b00111000, // L
    0b00010101, // M
    0b00110111, // N
    0b00111111, // O
    0b01110011, // P
    0b01101011, // Q
    0b00110011, // R
    0b01101101, // S
    0b01111000, // T
    0b00111110, // U
    0b00111110, // V
    0b00101010, // W
    0b01110110, // X
    0b01101110, // Y
    0b01011011, // Z
    0b00111001, // [
    0b01100100, //
    0b00001111, // ]
    0b00100011, // ^
    0b00001000, // _
    0b00000010, // `
    0b01011111, // a
    0b01111100, // b
    0b01011000, // c
    0b01011110, // d
    0b01111011, // e
    0b01110001, // f
    0b01101111, // g
    0b01110100, // h
    0b00010000, // i
    0b00001100, // j
    0b01110101, // k
    0b00110000, // l
    0b00010100, // m
    0b01010100, // n
    0b01011100, // o
    0b01110011, // p
    0b01100111, // q
    0b01010000, // r
    0b01101101, // s
    0b01111000, // t
    0b00011100, // u
    0b00011100, // v
    0b00010100, // w
    0b01110110, // x
    0b01101110, // y
    0b01011011, // z
    0b01000110, // {
    0b00110000, // |
    0b01110000, // }
    0b00000001, // ~
    0b00000000, // del
};

#define SEVENSEG_ASCII(c) pgm_read_byte(ascii + c + SEVENSEG_OFFSET_ASCII);
#define SEVENSEG_NUMBER(x) pgm_read_byte(ascii + x + SEVENSEG_OFFSET_NUMERICAL);
#define SEVENSEG_SET_ASCII(n, c, o) {if(o) { segmentMaps.b[n] = SEVENSEG_ASCII(c); } else { segmentMaps.b[n] |= SEVENSEG_ASCII(c); }}
#define SEVENSEG_SET_NUMBER(n, x, o) {if(o) { segmentMaps.b[n] = SEVENSEG_NUMBER(x); } else { segmentMaps.b[n] |= SEVENSEG_NUMBER(x); }}
#define SEVENSEG_BUFBREAK(buf) {if(buf == nullptr) { setSegments<7SEG_BLANK>(buf, true); return; }}

I2CIP_DEVICE_INIT_STATIC_ID(HT16K33);
I2CIP_OUTPUT_INIT_FAILSAFE(HT16K33, i2cip_ht16k33_data_t, ({'F','A','I','L'}), i2cip_ht16k33_mode_t, 7SEG_STR4);
// I2CIP_OUTPUT_INIT_FAILSAFE(HT16K33, i2cip_ht16k33_data_t, 0, i2cip_ht16k33_mode_t, 7SEG_BLANK);

HT16K33::HT16K33(i2cip_fqa_t fqa, const i2cip_id_t& id) : I2CIP::Device(fqa, id), I2CIP::OutputInterface<i2cip_ht16k33_data_t, i2cip_ht16k33_mode_t>((I2CIP::Device*)this) { }

// The first four are distinct (Hex, Leading-Spaces Signed, Leading-Zeroes Unsigned, ASCII String)
// The last three handle floating point numbers with varying degrees of precision

template <> void HT16K33::setSegments<7SEG_BLANK>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  if(!overwrite) return;
  SEVENSEG_SET_ASCII(0, ' ', true);
  SEVENSEG_SET_ASCII(1, ' ', true);
  SEVENSEG_SET_ASCII(2, ' ', true);
  SEVENSEG_SET_ASCII(3, ' ', true);
}

template <> void HT16K33::setSegments<7SEG_HEX32>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  for(uint8_t i = 0; i < 4; i++) {
    uint8_t hex = (buf->h >> (i * 8)) & 0xFF;
    if(hex > 9) {
      SEVENSEG_SET_ASCII(i, 'A' + hex - 10);
    } else {
      SEVENSEG_SET_NUMBER(i, hex);
    }
  }
}

template <> void HT16K33::setSegments<7SEG_UINT>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  uint32_t _val = buf->h;
  // uint16_t val = (uint16_t)(_val > 9999 ? 1337 : _val); // Constrain
  if(_val > 9999) {
    SEVENSEG_SET_ASCII(0, '+', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    return;
  }

  uint8_t thousands = val > 1000 ? val / 1000 : 0;
  uint8_t hundreds = val > 100 ? val / 100 : 0;
  uint8_t tens = val > 10 ? (val - (hundreds * 100)) / 10 : 0;
  uint8_t ones = val - hundreds * 100 - tens * 10;

  // if the second byte is non-zero, display a minus sign - because why not? - otherwise a space
  SEVENSEG_SET_NUMBER(0, thousands, overwrite);
  SEVENSEG_SET_NUMBER(1, hundreds, overwrite);
  SEVENSEG_SET_NUMBER(2, tens, overwrite);
  SEVENSEG_SET_NUMBER(3, ones, overwrite);
}

template <> void HT16K33::setSegments<7SEG_INT>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  int32_t _val = buf->h; // Immediate sign conversion
  bool sign = _val < 0;
  if(!sign) { setSegments<7SEG_UINT>(buf, overwrite); return; } // No sign, just recurse and print as unsigned

  if(_val < -999) {
    SEVENSEG_SET_ASCII(0, '-', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    return;
  }

  uint8_t hundreds = val > 100 ? val / 100 : 0;
  uint8_t tens = val > 10 ? (val - (hundreds * 100)) / 10 : 0;
  uint8_t ones = val - hundreds * 100 - tens * 10;

  // if the second byte is non-zero, display a minus sign - because why not? - otherwise a space
  uint8_t p = 0; // Sign placement
  if(hundreds == 0) {
    SEVENSEG_SET_ASCII(0, ' ', overwrite); // Sign won't be there
    p++; // Advance sign
  } else SEVENSEG_SET_NUMBER(1, hundreds, overwrite);

  if(hundreds == 0 && tens == 0) {
    SEVENSEG_SET_ASCII(1, ' ', overwrite); // Sign won't be there
    p++; // Advance sign
  } else SEVENSEG_SET_NUMBER(2, tens, overwrite);

  SEVENSEG_SET_NUMBER(3, ones, overwrite); // Always set ones

  SEVENSEG_SET_ASCII(p, '-', overwrite); // Set sign
}

template <> void HT16K33::setSegments<7SEG_4STR>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  for(uint8_t i = 0; i < 4; i++) {
    char c = buf->asci[i];
    if(c == '\0') {
      SEVENSEG_SET_ASCII(i, ' ', overwrite);
    } else {
      SEVENSEG_SET_ASCII(i, c, overwrite);
    }
  }
}

template <> void HT16K33::setSegments<7SEG_1F>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  float f = buf->f;
  if(f == NaN) {
    SEVENSEG_SET_ASCII(0, 'N', overwrite);
    SEVENSEG_SET_ASCII(1, 'U', overwrite);
    SEVENSEG_SET_ASCII(2, 'L', overwrite);
    SEVENSEG_SET_ASCII(3, 'L', overwrite);
  } else if(f >= 1000.0) {
    SEVENSEG_SET_ASCII(0, '+', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    segmentMaps.b[2] |= SEVENSEG_ASCII('.'); // Add decimal point
  } else if(f <= -100.0) {
    SEVENSEG_SET_ASCII(0, '-', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    segmentMaps.b[2] |= SEVENSEG_ASCII('.'); // Add decimal point
  } else {
    // Good to go, use <int> to print the number
    uint32_t _val = (int16_t)(f * 10); // 1 decimal place
    segmentMaps.b[2] = SEVENSEG_ASCII('.'); // Add decimal point
    setSegments<7SEG_INT>(_val, false); // No overwrite, leaves in the decimal point
  }
}

template <> void HT16K33::setSegments<7SEG_2F>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  float f = buf->f;
  if(f == NaN) {
    SEVENSEG_SET_ASCII(0, 'N', overwrite);
    SEVENSEG_SET_ASCII(1, 'U', overwrite);
    SEVENSEG_SET_ASCII(2, 'L', overwrite);
    SEVENSEG_SET_ASCII(3, 'L', overwrite);
  } else if(f >= 100.0) {
    SEVENSEG_SET_ASCII(0, '+', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    segmentMaps.b[1] |= SEVENSEG_ASCII('.'); // Add decimal point
  } else if(f <= -10.0) {
    SEVENSEG_SET_ASCII(0, '-', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    segmentMaps.b[1] |= SEVENSEG_ASCII('.'); // Add decimal point
  } else {
    // Good to go, use <int> to print the number
    uint32_t _val = (int16_t)(f * 100); // 2 decimal places
    segmentMaps.b[1] = SEVENSEG_ASCII('.'); // Add decimal point
    setSegments<7SEG_INT>(_val, false); // No overwrite, leaves in the decimal point
  }
}

template <> void HT16K33::setSegments<7SEG_3F>(i2cip_ht16k33_data_t const& buf, bool overwrite) {
  SEVENSEG_BUFBREAK(buf);
  float f = buf->f;
  if(f == NaN) {
    SEVENSEG_SET_ASCII(0, 'N', overwrite);
    SEVENSEG_SET_ASCII(1, 'U', overwrite);
    SEVENSEG_SET_ASCII(2, 'L', overwrite);
    SEVENSEG_SET_ASCII(3, 'L', overwrite);
  } else if(f >= 10.0) {
    SEVENSEG_SET_ASCII(0, '+', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    segmentMaps.b[0] |= SEVENSEG_ASCII('.'); // Add decimal point
  } else if(f <= -1.0) {
    SEVENSEG_SET_ASCII(0, '-', overwrite);
    SEVENSEG_SET_ASCII(1, 'E', overwrite);
    SEVENSEG_SET_ASCII(2, 'R', overwrite);
    SEVENSEG_SET_ASCII(3, 'R', overwrite);
    segmentMaps.b[0] |= SEVENSEG_ASCII('.'); // Add decimal point
  } else {
    // Good to go, use <int> to print the number
    uint32_t _val = (int16_t)(f * 1000); // 3 decimal places
    setSegments<7SEG_INT>(_val, overwrite); // No overwrite, leaves in the decimal point
    segmentMaps.b[0] = SEVENSEG_ASCII('.'); // Add decimal point
  }
}

i2cip_errorlevel_t HT16K33::set(i2cip_ht16k33_data_t const& buf, const i2cip_ht16k33_mode_t& mode) {
  if(!initialized) {
    // i2cip_errorlevel_t errlev = begin(true);
    i2cip_errorlevel_t errlev = begin();
    I2CIP_ERR_BREAK(errlev);
    initialized = true;
  }

  if(buf == nullptr) return brightness(0x00);

  // 7SEG_HEX32, 7SEG_UINT, 7SEG_INT, 7SEG_STR4, 7SEG_1F, 7SEG_2F, 7SEG_3F
  // u32 h, u8 b[4], char s[4]
  setSegments<mode>(buf);
  // writeDigitRaw(d, font | (dot << 7));
  // displaybuffer[d] = (font | ((uint8_t)dot << 7));

  // Convert to 
  uint8_t buffer[8] = { segmentMaps.b[0], 0x00, segmentMaps.b[1], 0x00, segmentMaps.b[2], 0x00, segmentMaps.b[3], 0x00 };
  return writeRegister((uint8_t)(0x00), buffer, 8, false);

  // uint8_t buffer[4] = { segmentMaps.b[0], segmentMaps.b[1], segmentMaps.b[2], segmentMaps.b[3] };
  // return writeRegister((uint8_t)(0x00), buffer, 4, false);
}