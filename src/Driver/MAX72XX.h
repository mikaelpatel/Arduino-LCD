/**
 * @file MAX72XX.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2017, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef MAX72XX_H
#define MAX72XX_H

#include "LCD.h"
#include "GPIO.h"
#include "SRPO.h"

/**
 * Device driver for MAX72XX Serially Interfaced, 8-Digit LED Display
 * Drivers.
 *
 * The MAX7219/MAX7221 are compact, serial input/output common-cathode
 * display drivers that interface microprocessors (μPs) to 7-segment
 * numeric LED displays of up to 8 digits, bar-graph displays, or 64
 * individual LEDs. Included on-chip are a BCD code-B decoder,
 * multiplex scan circuitry, segment and digit drivers, and an 8x8
 * static RAM that stores each digit.
 *
 * @section References
 * 1. Product Specification, MAXIM, MAX7219/7221, 19-5542, Rev.4, 7/03.
 * http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
 *
 * @param[in] SCE_PIN screen chip enable pin.
 * @param[in] SDIN_PIN screen data pin.
 * @param[in] SCLK_PIN screen clock pin.
 */
template<BOARD::pin_t SCE_PIN, BOARD::pin_t SDIN_PIN, BOARD::pin_t SCLK_PIN>
class MAX72XX : public LCD::Device {
public:
  /** Display size. */
  static const uint8_t WIDTH = 8;
  static const uint8_t HEIGHT = 1;
  static const uint8_t LINES = 1;

  /**
   * Construct display device driver with given io adapter and font.
   * The font should be a character to segment mapping table in
   * program memory for character codes SPACE(0x20) to DEL(0x7f).
   * No-Decode Mode Data Bits and Corresponding Segment Lines (Table
   * 6, pp 8.).
   * @code
   *      A
   *    +---+
   *   F| G |B
   *    +---+	Standard 7-Segment LED
   *   E|   |C
   *    +---+ DP
   *      D
   *
   *  D7 D6 D5 D4 D3 D2 D1 D0
   *  DP  A  B  C  D  E  F  G
   * @endcode
   *
   * @param[in] io adapter, SPI or in/output pin based.
   * @param[in] font program memory (Default NULL).
   */
  MAX72XX(const uint8_t* font = NULL) : LCD::Device()
  {
    /** Default font table; SPACE(0x20) to DEL(0x7f). */
    static const uint8_t default_font[] PROGMEM = {
      0b00000000, // (space)
      0b10000000, // !
      0b00100010, // "
      0b10000000, // #
      0b10000000, // $
      0b10000000, // %
      0b10000000, // &
      0b00000010, // '
      0b01001110, // (
      0b01111000, // )
      0b10000000, // *
      0b10000000, // +
      0b10000000, // ,
      0b00000001, // -
      0b10000000, // .
      0b00100100, // /
      0b01111110, // 0
      0b00110000, // 1
      0b01101101, // 2
      0b01111001, // 3
      0b00110011, // 4
      0b01011011, // 5
      0b01011111, // 6
      0b01110000, // 7
      0b01111111, // 8
      0b01111011, // 9
      0b10000000, // :
      0b10000000, // ;
      0b10000000, // <
      0b00001001, // =
      0b10000000, // >
      0b10000000, // ?
      0b10000000, // @
      0b01110111, // A
      0b10000000, // B
      0b01001110, // C
      0b10000000, // D
      0b01001111, // E
      0b01000111, // F
      0b01011110, // G
      0b00110111, // H
      0b00110000, // I
      0b00111000, // J
      0b10000000, // K
      0b00001110, // L
      0b10000000, // M
      0b11110110, // N
      0b01111110, // O
      0b01100111, // P
      0b10000000, // Q
      0b10000000, // R
      0b01011011, // S
      0b10000000, // T
      0b00111110, // U
      0b10000000, // V
      0b10000000, // W
      0b00110111, // X
      0b00110011, // Y
      0b01101101, // Z
      0b01001110, // [
      0b00010011, // "\"
      0b01111000, // ]
      0b11100010, // ^
      0b00001000, // _
      0b10000000, // `
      0b10000000, // a
      0b00011111, // b
      0b00001101, // c
      0b00111101, // d
      0b10000000, // e
      0b01000111, // f
      0b01111011, // g
      0b00010111, // h
      0b00010000, // i
      0b00111000, // j
      0b10000000, // k
      0b00110000, // l
      0b10000000, // m
      0b00010101, // n
      0b00011101, // o
      0b01100111, // p
      0b01110011, // q
      0b00000101, // r
      0b10000000, // s
      0b10000000, // t
      0b00011100, // u
      0b10000000, // v
      0b10000000, // w
      0b10000000, // x
      0b10000000, // y
      0b10000000, // z
      0b00110001, // {
      0b00110000, // |
      0b00000111, // }
      0b00100101, // ~
      0b00010011  // DEL
    };
    m_font = font != NULL ? font : default_font;
    m_sce.output();
    m_sce.high();
  }

  /**
   * @override{LCD::Device}
   * Start interaction with display. Turns display on, clears and sets
   * the contrast/intensity to mid-level(7).
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool begin()
  {
    display_off();
    set(DECODE_MODE, NO_DECODE);
    set(SCAN_LIMIT, 7);
    display_contrast(7);
    display_clear();
    display_on();
    return (true);
  }

  /**
   * @override{LCD::Device}
   * Stop sequence of interaction with device. Puts the display in
   * shutdown mode.
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool end()
  {
    set(DISPLAY_MODE, SHUTDOWN_MODE);
    return (true);
  }

  /**
   * @override{LCD::Device}
   * Set display contrast/intensity level (0..15).
   * @param[in] contrast level.
   */
  virtual void display_contrast(uint8_t level)
  {
    set(INTENSITY, level & 0xF);
  }

  /**
   * @override{LCD::Device}
   * Turn display on.
   */
  virtual void display_on()
  {
    set(DISPLAY_MODE, NORMAL_OPERATION);
  }

  /**
   * @override{LCD::Device}
   * Turn display off.
   */
  virtual void display_off()
  {
    set(DISPLAY_MODE, SHUTDOWN_MODE);
  }

  /**
   * @override{LCD::Device}
   * Clear display and move cursor to home (0, 0).
   */
  virtual void display_clear()
  {
    for (uint8_t reg = DIGIT0; reg <= DIGIT7; reg++)
      set((Register) reg, 0x00);
    set_cursor(0, 0);
  }

  /**
   * @override{LCD::Device}
   * Set cursor to given position.
   * @param[in] x pixel position (0..WIDTH-1).
   * @param[in] y line position (0..LINES-1).
   */
  virtual void set_cursor(uint8_t x, uint8_t y)
  {
    if (x >= WIDTH) x = 0;
    if (y >= LINES) y = 0;
    m_x = x;
    m_y = y;
  }

  /**
   * @override{Print}
   * Write character to display. Handles carriage-return-line-feed,
   * backspace, alert, horizontal tab and form-feed. The period
   * character is translated to the 7-segment LED decimal point of the
   * previous written character. Returns number of characters(1) or
   * zero(0) on error.
   * @param[in] c character to write.
   * @return number of characters written(1) or zero(0) for error.
   */
  virtual size_t write(uint8_t c)
  {
    // Check for illegal characters
    if (c > 128) return (0);

    // Check for special characters
    if (c < ' ') {
      switch (c) {
      case '\r': // Carriage-return: move to start of line
	set_cursor(0, m_y);
	break;
      case '\f': // Form-feed: clear the display or line-feed: clear line
      case '\n':
	display_clear();
	break;
      case '\t': // Horizontal tab
	{
	  uint8_t x = m_x + m_tab - (m_x % m_tab);
	  uint8_t y = m_y + (x >= WIDTH);
	  set_cursor(x, y);
	}
	break;
      case '\b': // Back-space: move cursor back one step (if possible)
	set_cursor(m_x - 1, m_y);
	break;
      case '\a': // Alert: blink the backlight
	display_off();
	delay(32);
	display_on();
	break;
      default:
	return (0);
      }
      return (1);
    }

    // Write character; compress dot with latest character
    uint8_t segments;
    if (c == '.') {
      c = m_latest;
      segments = pgm_read_byte(m_font + c - ' ') | 0x80;
    }
    else {
      if (m_x == WIDTH) putchar('\n');
      m_x += 1;
      m_latest = c;
      segments = pgm_read_byte(m_font + c - ' ');
    }

    // Set the segments of the current digit
    set((Register) m_x, segments);
    return (1);
  }

protected:
  /**
   * Register Address Map (Table 2, pp 7).
   */
  enum Register {
    NOP = 0x00,			//!< No-operation.
    DIGIT0 = 0x01,		//!< Digit 0 (encode or segment data).
    DIGIT1 = 0x02,		//!< Digit 1 (encode or segment data).
    DIGIT2 = 0x03,		//!< Digit 2 (encode or segment data).
    DIGIT3 = 0x04,		//!< Digit 3 (encode or segment data).
    DIGIT4 = 0x05,		//!< Digit 4 (encode or segment data).
    DIGIT5 = 0x06,		//!< Digit 5 (encode or segment data).
    DIGIT6 = 0x07,		//!< Digit 6 (encode or segment data).
    DIGIT7 = 0x08,		//!< Digit 7 (encode or segment data).
    DECODE_MODE = 0x09,		//!< Decode Mode (0..255, digit bitset).
    INTENSITY = 0x0a,		//!< Intensity (0..15, level).
    SCAN_LIMIT = 0x0b,		//!< Scan Limit (0..7, digits 1..8).
    DISPLAY_MODE = 0x0c,	//!< Display Mode (shutdown, normal).
    DISPLAY_TEST = 0x0f		//!< Display Test (0..1, on/off).
  } __attribute__((packed));

  /**
   * Set register to the given value.
   * @param[in] reg register address.
   * @param[in] value.
   */
  void set(Register reg, uint8_t value)
  {
    m_sce.toggle();
    m_srpo.write(reg);
    m_srpo.write(value);
    m_sce.toggle();
  }

  /**
   * Shutdown Register Format (Table 3, pp. 7).
   */
  enum {
    SHUTDOWN_MODE = 0x00,	//!< Shutdown mode.
    NORMAL_OPERATION = 0x01	//!< Normal operation.
  } __attribute__((packed));

  /**
   * Decode-Mode Register (Table 4, pp. 7).
   */
  enum {
    NO_DECODE = 0x00,		//!< No decode for digits 7-0.
    ALL_DECODE = 0xff		//!< Code B decode for digits 7-0.
  } __attribute__((packed));

  GPIO<SCE_PIN> m_sce;			     	//!< Chip enable pin.
  SRPO<MSBFIRST, SDIN_PIN, SCLK_PIN> m_srpo; 	//!< Serial output.
  const uint8_t* m_font;			//!< Font in program memory.
  char m_latest;				//!< Latest character code.

};
#endif
