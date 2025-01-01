#include "HT16K33.hpp"

// #include <debug.h>

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

char parseSevenSegments(uint8_t b) {
  switch(b) {
    // case 0b00000000: return ' ';
    case 0b00000000: return '*'; // Space
    case 0b10000110: return '!';
    case 0b00100010: return '"';
    case 0b01111110: return '#';
    // case 0b01101101: return '$';
    case 0b11010010: return '%';
    case 0b01000110: return '&';
    case 0b00100000: return '\'';
    case 0b00101001: return '(';
    case 0b00001011: return ')';
    case 0b00100001: return '*';
    case 0b01110000: return '+';
    case 0b00010000: return ',';
    case 0b01000000: return '-';
    case 0b10000000: return '.';
    case 0b01010010: return '/';
    case 0b00111111: return '0';
    case 0b00000110: return '1';
    case 0b01011011: return '2';
    case 0b01001111: return '3';
    case 0b01100110: return '4';
    case 0b01101101: return '5';
    case 0b01111101: return '6';
    case 0b00000111: return '7';
    case 0b01111111: return '8';
    case 0b01101111: return '9';
    case 0b00001001: return ':';
    case 0b00001101: return ';';
    case 0b01100001: return '<';
    case 0b01001000: return '=';
    case 0b01000011: return '>';
    case 0b11010011: return '?';
    // case 0b01011111: return '@';
    case 0b01110111: return 'A';
    case 0b01111100: return 'B';
    case 0b00111001: return 'C';
    case 0b01011110: return 'D';
    case 0b01111001: return 'E';
    case 0b01110001: return 'F';
    case 0b00111101: return 'G';
    case 0b01110110: return 'H';
    case 0b00110000: return 'I';
    case 0b00011110: return 'J';
    case 0b01110101: return 'K';
    case 0b00111000: return 'L';
    case 0b00010101: return 'M';
    case 0b00110111: return 'N';
    // case 0b00111111: return 'O'; // Leetify
    case 0b01110011: return 'P';
    case 0b01101011: return 'Q';
    case 0b00110011: return 'R';
    // case 0b01101101: return 'S';
    case 0b01111000: return 'T';
    // case 0b00111110: return 'U'; // Romanize
    case 0b00111110: return 'V';
    case 0b00101010: return 'W';
    // case 0b01110110: return 'X';
    case 0b01101110: return 'Y';
    // case 0b01011011: return 'Z';
    // case 0b00111001: return '[';
    case 0b01100100: return '\\';
    case 0b00001111: return ']';
    case 0b00100011: return '^';
    case 0b00001000: return '_';
    case 0b00000010: return '`';
    case 0b01011111: return 'a';
    // case 0b01111100: return 'b';
    case 0b01011000: return 'c';
    // case 0b01011110: return 'd';
    case 0b01111011: return 'e';
    // case 0b01110001: return 'f';
    // case 0b01101111: return 'g';
    case 0b01110100: return 'h';
    // case 0b00010000: return 'i';
    case 0b00001100: return 'j';
    // case 0b01110101: return 'k';
    // case 0b00110000: return 'l';
    case 0b00010100: return 'm';
    case 0b01010100: return 'n';
    case 0b01011100: return 'o';
    // case 0b01110011: return 'p';
    case 0b01100111: return 'q';
    case 0b01010000: return 'r';
    // case 0b01101101: return 's';
    // case 0b01111000: return 't';
    // case 0b00011100: return 'u';
    case 0b00011100: return 'v';
    // case 0b00010100: return 'w';
    // case 0b01110110: return 'x';
    // case 0b01101110: return 'y';
    // case 0b01011011: return 'z';
    // case 0b01000110: return '{';
    // case 0b00110000: return '|';
    // case 0b01110000: return '}';
    case 0b00000001: return '~';
    default: return '\0';
  }
}

#define SEVENSEG_COUNT 4
#define SEVENSEG_OFFSET_NUMERICAL 16
#define SEVENSEG_OFFSET_ASCII -32
#define SEVENSEG_SET_BYTE(n, b) { if(n >= 0 && n < SEVENSEG_COUNT) { segmentMaps |= ((uint32_t)b << (n * 8)); } }

#define SEVENSEG_SET_BLANK(n) { if(n >= 0 && n < SEVENSEG_COUNT) { segmentMaps &= ~((uint32_t)0x000000FF << (n * 8)); } } // Shift FF, invert, and & to clear the byte

#define SEVENSEG_SET_ASCII(n, c, o) \
  if(o) { SEVENSEG_SET_BLANK((n)); } \
  SEVENSEG_SET_BYTE((n), (pgm_read_byte_near(ascii + (c) + (SEVENSEG_OFFSET_ASCII))));
  
#define SEVENSEG_SET_NUMBER(n, x, o) \
  if(o) { SEVENSEG_SET_BLANK((n)); } \
  SEVENSEG_SET_BYTE((n), (pgm_read_byte_near(ascii + (x) + (SEVENSEG_OFFSET_NUMERICAL))));

// #define SEVENSEG_FAILSAFE {0x4C494146} // "FAIL" without null terminator
#define SEVENSEG_FAILSAFE { .c = {'F', 'A', 'I', 'L'}} // "FAIL" without null terminator

I2CIP_DEVICE_INIT_STATIC_ID(HT16K33);
I2CIP_OUTPUT_INIT_FAILSAFE(HT16K33, i2cip_ht16k33_data_t, SEVENSEG_FAILSAFE, i2cip_ht16k33_mode_t, SEG_ASCII);

using namespace I2CIP;

HT16K33::HT16K33(i2cip_fqa_t fqa, const i2cip_id_t& id) : Device(fqa, id), OutputInterface<i2cip_ht16k33_data_t, i2cip_ht16k33_mode_t>((Device*)this) { }

// char hexChar(uint8_t hex) {
//   hex &= 0x0F;
//   if(hex < 10) return (char)(hex + '0');
//   return (char)(hex - 10 + 'A');
// }

// The first four are distinct (Hex, Leading-Spaces Signed, Leading-Zeroes Unsigned, ASCII String)
// The last three handle floating point numbers with varying degrees of precision

template <> void HT16K33::setSegments<SEG_BLANK>(i2cip_ht16k33_data_t buf, bool overwrite) {
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println("BLANK");
  #endif
  if(!overwrite) return;
  SEVENSEG_SET_BLANK(0);
  SEVENSEG_SET_BLANK(1);
  SEVENSEG_SET_BLANK(2);
  SEVENSEG_SET_BLANK(3);
}

template <> void HT16K33::setSegments<SEG_HEX16>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  uint32_t val = buf.h;
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.print("0x");
  #endif
  for(uint8_t i = 0; i < 4; i++) {
    uint8_t hex = (val >> (i * 4)) & 0xF;
    #ifdef I2CIP_DEBUG_SERIAL
      I2CIP_DEBUG_SERIAL.print(hex, HEX);
    #endif
    if(hex > 9) {
      SEVENSEG_SET_ASCII(3-i, ('A' - 0xA) + hex, overwrite);
    } else {
      SEVENSEG_SET_NUMBER(3-i, hex, overwrite);
    }
  }
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println();
  #endif
}

template <> void HT16K33::setSegments<SEG_UINT>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  uint32_t val = buf.h;
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println(val, DEC);
  #endif
  // uint16_t val = (uint16_t)(_val > 9999 ? 1337 : _val); // Constrain
  if(val > 9999) {
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

template <> void HT16K33::setSegments<SEG_INT>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  int32_t val = buf.h; // Immediate sign conversion
  bool sign = buf.h & (1 << 31); // Check sign bit
  // if(!sign) { setSegments<SEG_UINT>(buf, overwrite); return; } // No sign, just recurse and print as unsigned

  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println(val, DEC);
  #endif

  if(val < -999) {
    SEVENSEG_SET_ASCII(0, '-', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
    return;
  } else if(val > 9999) {
    SEVENSEG_SET_ASCII(0, '+', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
    return;
  }

  val = abs(val);

  uint8_t thousands = sign ? 0 : (val > 1000 ? val / 1000 : 0);
  uint8_t hundreds = val > 100 ? val / 100 : 0;
  uint8_t tens = val > 10 ? (val - (hundreds * 100)) / 10 : 0;
  uint8_t ones = val - hundreds * 100 - tens * 10;

  // if the second byte is non-zero, display a minus sign - because why not? - otherwise a space
  uint8_t p = 0; // Sign placement
  if(thousands != 0) { SEVENSEG_SET_NUMBER(0, thousands, overwrite); }
  if(thousands == 0 && hundreds == 0) {
    SEVENSEG_SET_ASCII(0, ' ', overwrite); // Sign won't be there
    p++; // Advance signs
  } else { SEVENSEG_SET_NUMBER(1, hundreds, overwrite); }

  if(thousands == 0 && hundreds == 0 && tens == 0) {
    SEVENSEG_SET_ASCII(1, ' ', overwrite); // Sign won't be there
    p++; // Advance sign
  } else { SEVENSEG_SET_NUMBER(2, tens, overwrite); }

  SEVENSEG_SET_NUMBER(3, ones, overwrite); // Always set ones

  if(sign) { SEVENSEG_SET_ASCII(p, '-', overwrite); } // Set sign
}

template <> void HT16K33::setSegments<SEG_ASCII>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  for(uint8_t i = 0; i < 4; i++) {
    // char c = ((buf.h >> (i * 8)) & 0xFF); // Extract byte
    char c = buf.c[i];
    #ifdef I2CIP_DEBUG_SERIAL
      I2CIP_DEBUG_SERIAL.print(c);
    #endif
    c = constrain(c, ' ', '~'); // Constrain to printable ASCII
    SEVENSEG_SET_ASCII(i, c, overwrite);
  }
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println();
  #endif
}

template <> void HT16K33::setSegments<SEG_1F>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  float f = buf.f;
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println(f, 1);
  #endif
  if(isnan(f)) {
    SEVENSEG_SET_ASCII(0, 'N', true);
    SEVENSEG_SET_ASCII(1, 'U', true);
    SEVENSEG_SET_ASCII(2, 'L', true);
    SEVENSEG_SET_ASCII(3, 'L', true);
  } else if(f >= 1000.0) {
    SEVENSEG_SET_ASCII(0, '+', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
  } else if(f <= -100.0) {
    SEVENSEG_SET_ASCII(0, '-', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
  } else {
    // Good to go, use <int> to print the number
    int32_t val = (f * 10); // Shift 1 decimal place
    i2cip_ht16k33_data_t _buf = {(uint32_t)val};
    setSegments<SEG_INT>(_buf, overwrite); // No overwrite, leaves in the decimal point
  }
  SEVENSEG_SET_ASCII(2, '.', false); // Add decimal point
}

template <> void HT16K33::setSegments<SEG_2F>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  float f = buf.f;
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println(f, 2);
  #endif
  if(f == NAN) {
    SEVENSEG_SET_ASCII(0, 'N', true);
    SEVENSEG_SET_ASCII(1, 'U', true);
    SEVENSEG_SET_ASCII(2, 'L', true);
    SEVENSEG_SET_ASCII(3, 'L', true);
  } else if(f >= 100.0) {
    SEVENSEG_SET_ASCII(0, '+', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
  } else if(f <= -10.0) {
    SEVENSEG_SET_ASCII(0, '-', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
  } else {
    // Good to go, use <int> to print the number
    int32_t val = (f * 100); // Shift 1 decimal place
    i2cip_ht16k33_data_t _buf = {(uint32_t)val};
    setSegments<SEG_INT>(_buf, overwrite); // No overwrite, leaves in the decimal point
  }
  SEVENSEG_SET_ASCII(1, '.', false); // Add decimal point
}

template <> void HT16K33::setSegments<SEG_3F>(i2cip_ht16k33_data_t buf, bool overwrite) {
  // SEVENSEG_BUFBREAK(buf);
  float f = buf.f;
  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.println(f, 2);
  #endif
  if(f == NAN) {
    SEVENSEG_SET_ASCII(0, 'N', true);
    SEVENSEG_SET_ASCII(1, 'U', true);
    SEVENSEG_SET_ASCII(2, 'L', true);
    SEVENSEG_SET_ASCII(3, 'L', true);
  } else if(f >= 10.0) {
    SEVENSEG_SET_ASCII(0, '+', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
  } else if(f <= -1.0) {
    SEVENSEG_SET_ASCII(0, '-', true);
    SEVENSEG_SET_ASCII(1, 'E', true);
    SEVENSEG_SET_ASCII(2, 'R', true);
    SEVENSEG_SET_ASCII(3, 'R', true);
  } else {
    // Good to go, use <int> to print the number
    int32_t val = (f * 1000); // Shift 1 decimal place
    i2cip_ht16k33_data_t _buf = {(uint32_t)val};
    setSegments<SEG_INT>(_buf, overwrite); // No overwrite, leaves in the decimal point
  }
  SEVENSEG_SET_ASCII(0, '.', false); // Add decimal point
}

i2cip_errorlevel_t HT16K33::blink(uint8_t b, bool setbus) {
  if (b > HT16K33_BLINK_HALFHZ) return I2CIP_ERR_SOFT;

  return writeByte(HT16K33_REG_BLINK | HT16K33_DISPLAY_ON | (b << 1), setbus);
}

i2cip_errorlevel_t HT16K33::brightness(uint8_t b, bool setbus) {
  if(b > 15) b = 15;

  return writeByte(HT16K33_REG_BRIGHTNESS | b, setbus);
}

i2cip_errorlevel_t HT16K33::begin(bool setbus) {
  // Serial.println(F("7SEG BEGIN"));

  // Oscillator
  i2cip_errorlevel_t errlev = writeByte(HT16K33_CMD_OSCILLATOR, setbus); // 0x21 = 0b00100001
  I2CIP_ERR_BREAK(errlev);

  // Attempt blank TX
  setSegments<SEG_BLANK>({0}, true);
  errlev = writeSegments();
  I2CIP_ERR_BREAK(errlev);

  // Blink Off
  errlev = blink(HT16K33_BLINK_OFF);
  I2CIP_ERR_BREAK(errlev);

  // Turn On
  return brightness();
}

i2cip_errorlevel_t HT16K33::writeSegments(bool setbus) {
  // Typically, segmentMaps would be uint16_t[8] and we'd store in the 8 LSB of the first 4 words
  // Then, when writing, we'd write all 16 bytes to the device
  // As so:
  /**
   * buffer[2 * i] = displaybuffer[i] & 0xFF;
   * buffer[2 * i + 1] = displaybuffer[i] >> 8;
   */
  // So here, we interleave zeroes in the uint16_t MSB positions: uint8_t [0, 2, ...]
  // and place our bytes in the LSB positions: uint8_t [1, 3, ...]
  uint8_t buffer[0xF] = { 0x00 };
  buffer[0] = (uint8_t)(this->segmentMaps & 0xFF);
  // buffer[1] = (uint8_t)(this->segmentMaps & 0xFF);
  buffer[2] = (uint8_t)((this->segmentMaps >> 8) & 0xFF);
  // buffer[3] = (uint8_t)((this->segmentMaps >> 8) & 0xFF);
  buffer[6] = (uint8_t)((this->segmentMaps >> 16) & 0xFF);
  // buffer[5] = (uint8_t)((this->segmentMaps >> 16) & 0xFF);
  buffer[8] = (uint8_t)((this->segmentMaps >> 24) & 0xFF);
  // buffer[7] = (uint8_t)((this->segmentMaps >> 24) & 0xFF);
  return writeRegister((uint8_t)(0x00), buffer, 0xF, setbus);
}

i2cip_errorlevel_t HT16K33::set(i2cip_ht16k33_data_t const& buf, const i2cip_ht16k33_mode_t& mode) {
  i2cip_errorlevel_t errlev = I2CIP_ERR_NONE;
  if(this->initialized == false) {
    // i2cip_errorlevel_t errlev = begin(true);
    errlev = this->begin();
    I2CIP_ERR_BREAK(errlev);
    this->initialized = true;
  }

  #ifdef I2CIP_DEBUG_SERIAL
    I2CIP_DEBUG_SERIAL.print(F("7SEG SET @0x"));
    I2CIP_DEBUG_SERIAL.print((uintptr_t)&buf, HEX);
    I2CIP_DEBUG_SERIAL.print(" MODE ");
    switch(mode) {
      case SEG_HEX16: I2CIP_DEBUG_SERIAL.print("HEX[4] "); break;
      case SEG_UINT: I2CIP_DEBUG_SERIAL.print("UINT "); break;
      case SEG_INT: I2CIP_DEBUG_SERIAL.print("INT "); break;
      case SEG_ASCII: I2CIP_DEBUG_SERIAL.print("STRING "); break;
      case SEG_1F: I2CIP_DEBUG_SERIAL.print(".1FLOAT "); break;
      case SEG_2F: I2CIP_DEBUG_SERIAL.print(".2FLOAT "); break;
      case SEG_3F: I2CIP_DEBUG_SERIAL.print(".3FLOAT "); break;
      default: I2CIP_DEBUG_SERIAL.print("BLANK "); break;
    }
  #endif

  // if(buf == nullptr) return brightness(0x00);

  // SEG_HEX16, SEG_UINT, SEG_INT, SEG_ASCII, SEG_1F, SEG_2F, SEG_3F
  // u32 h, u8 b[4], char s[4], float f
  switch(mode) {
    case SEG_HEX16: setSegments<SEG_HEX16>(buf, true); break;
    case SEG_UINT: setSegments<SEG_UINT>(buf, true); break;
    case SEG_INT: setSegments<SEG_INT>(buf, true); break;
    case SEG_ASCII: setSegments<SEG_ASCII>(buf, true); break;
    case SEG_1F: setSegments<SEG_1F>(buf, true); break;
    case SEG_2F: setSegments<SEG_2F>(buf, true); break;
    case SEG_3F: setSegments<SEG_3F>(buf, true); break;
    default: setSegments<SEG_BLANK>({0}, true); break;
  }
  // writeDigitRaw(d, font | (dot << 7));
  // displaybuffer[d] = (font | ((uint8_t)dot << 7));

  errlev = writeSegments(false);
  if(errlev != I2CIP_ERR_NONE) { this->initialized = false; return errlev; }

  return blink(HT16K33_BLINK_OFF, false);
}