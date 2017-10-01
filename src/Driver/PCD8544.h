/**
 * @file PCD8544.h
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

#ifndef PCD8544_H
#define PCD8544_H

#include "LCD.h"
#include "GPIO.h"
#include "SRPO.h"

#ifndef CHARBITS
#define CHARBITS 8
#endif

/**
 * Device driver for PCD8544 48x84 pixels matrix LCD controller/driver.
 * Default pins are for LCD4884 Shield.
 * @param[in] RST_PIN reset pin (default BOARD::D6).
 * @param[in] SCE_PIN screen chip enable pin (default BOARD::D5).
 * @param[in] DC_PIN data/command select pin (default BOARD::D4).
 * @param[in] SDIN_PIN screen data pin (default BOARD::D3).
 * @param[in] SCLK_PIN screen clock pin (default BOARD::D2).
 *
 * @section Circuit
 * PCD8544 is a low voltage device (3V3) and signals require level
 * shifter (74HC4050 or 10K resistor).
  * @code
 *                         PCD8544/LCD
 *                       +------------+
 * (D6)---------[ > ]--1-|RST         |
 * (D5)---------[ > ]--2-|SCE         |
 * (D4)---------[ > ]--3-|DC          |
 * (D3)---------[ > ]--4-|SDIN        |
 * (D2)---------[ > ]--5-|SCLK        |
 * (3V3)---------------6-|VCC         |
 * (GND)--------[220]--7-|LED         |
 * (GND)---------------8-|GND         |
 *                       +------------+
 * (D7)------------------BL (optional)
 * @endcode
 *
 * @section References
 * 1. Product Specification, Philips Semiconductors, 1999 Apr 12.
 * https://www.sparkfun.com/datasheets/LCD/Monochrome/Nokia5110.pdf
 */
template<BOARD::pin_t RST_PIN = BOARD::D6,
	 BOARD::pin_t SCE_PIN = BOARD::D5,
	 BOARD::pin_t DC_PIN = BOARD::D4,
	 BOARD::pin_t SDIN_PIN = BOARD::D3,
	 BOARD::pin_t SCLK_PIN = BOARD::D2>
class PCD8544 : public LCD::Device {
public:
  /** Display size. */
  static const uint8_t FONT_WIDTH = 6;
  static const uint8_t FONT_HEIGHT = 8;
  static const uint8_t WIDTH = 84 / FONT_WIDTH;
  static const uint8_t HEIGHT = 48 / FONT_HEIGHT;

  /**
   * Construct display device driver and initiate pins. The parameter
   * should a 5x7 font in program memory or NULL for the default
   * font.
   * @param[in] font in program memory (Default NULL).
   */
  PCD8544(const uint8_t* font = NULL) :
    LCD::Device()
  {
    static const uint8_t default_font[] PROGMEM = {
      0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x5F, 0x00, 0x00,
      0x00, 0x07, 0x00, 0x07, 0x00,
      0x14, 0x7F, 0x14, 0x7F, 0x14,
      0x24, 0x2A, 0x7F, 0x2A, 0x12,
      0x23, 0x13, 0x08, 0x64, 0x62,
      0x36, 0x49, 0x56, 0x20, 0x50,
      0x00, 0x08, 0x07, 0x03, 0x00,
      0x00, 0x1C, 0x22, 0x41, 0x00,
      0x00, 0x41, 0x22, 0x1C, 0x00,
      0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
      0x08, 0x08, 0x3E, 0x08, 0x08,
      0x00, 0x80, 0x70, 0x30, 0x00,
      0x08, 0x08, 0x08, 0x08, 0x08,
      0x00, 0x00, 0x60, 0x60, 0x00,
      0x20, 0x10, 0x08, 0x04, 0x02,
      0x3E, 0x51, 0x49, 0x45, 0x3E,
      0x00, 0x42, 0x7F, 0x40, 0x00,
      0x72, 0x49, 0x49, 0x49, 0x46,
      0x21, 0x41, 0x49, 0x4D, 0x33,
      0x18, 0x14, 0x12, 0x7F, 0x10,
      0x27, 0x45, 0x45, 0x45, 0x39,
      0x3C, 0x4A, 0x49, 0x49, 0x31,
      0x41, 0x21, 0x11, 0x09, 0x07,
      0x36, 0x49, 0x49, 0x49, 0x36,
      0x46, 0x49, 0x49, 0x29, 0x1E,
      0x00, 0x00, 0x14, 0x00, 0x00,
      0x00, 0x40, 0x34, 0x00, 0x00,
      0x00, 0x08, 0x14, 0x22, 0x41,
      0x14, 0x14, 0x14, 0x14, 0x14,
      0x00, 0x41, 0x22, 0x14, 0x08,
      0x02, 0x01, 0x59, 0x09, 0x06,
      0x3E, 0x41, 0x5D, 0x59, 0x4E,
      0x7C, 0x12, 0x11, 0x12, 0x7C,
      0x7F, 0x49, 0x49, 0x49, 0x36,
      0x3E, 0x41, 0x41, 0x41, 0x22,
      0x7F, 0x41, 0x41, 0x41, 0x3E,
      0x7F, 0x49, 0x49, 0x49, 0x41,
      0x7F, 0x09, 0x09, 0x09, 0x01,
      0x3E, 0x41, 0x41, 0x51, 0x73,
      0x7F, 0x08, 0x08, 0x08, 0x7F,
      0x00, 0x41, 0x7F, 0x41, 0x00,
      0x20, 0x40, 0x41, 0x3F, 0x01,
      0x7F, 0x08, 0x14, 0x22, 0x41,
      0x7F, 0x40, 0x40, 0x40, 0x40,
      0x7F, 0x02, 0x1C, 0x02, 0x7F,
      0x7F, 0x04, 0x08, 0x10, 0x7F,
      0x3E, 0x41, 0x41, 0x41, 0x3E,
      0x7F, 0x09, 0x09, 0x09, 0x06,
      0x3E, 0x41, 0x51, 0x21, 0x5E,
      0x7F, 0x09, 0x19, 0x29, 0x46,
      0x26, 0x49, 0x49, 0x49, 0x32,
      0x03, 0x01, 0x7F, 0x01, 0x03,
      0x3F, 0x40, 0x40, 0x40, 0x3F,
      0x1F, 0x20, 0x40, 0x20, 0x1F,
      0x3F, 0x40, 0x38, 0x40, 0x3F,
      0x63, 0x14, 0x08, 0x14, 0x63,
      0x03, 0x04, 0x78, 0x04, 0x03,
      0x61, 0x59, 0x49, 0x4D, 0x43,
      0x00, 0x7F, 0x41, 0x41, 0x41,
      0x02, 0x04, 0x08, 0x10, 0x20,
      0x00, 0x41, 0x41, 0x41, 0x7F,
      0x04, 0x02, 0x01, 0x02, 0x04,
      0x40, 0x40, 0x40, 0x40, 0x40,
      0x00, 0x03, 0x07, 0x08, 0x00,
      0x20, 0x54, 0x54, 0x78, 0x40,
      0x7F, 0x28, 0x44, 0x44, 0x38,
      0x38, 0x44, 0x44, 0x44, 0x28,
      0x38, 0x44, 0x44, 0x28, 0x7F,
      0x38, 0x54, 0x54, 0x54, 0x18,
      0x00, 0x08, 0x7E, 0x09, 0x02,
      0x18, 0xA4, 0xA4, 0x9C, 0x78,
      0x7F, 0x08, 0x04, 0x04, 0x78,
      0x00, 0x44, 0x7D, 0x40, 0x00,
      0x20, 0x40, 0x40, 0x3D, 0x00,
      0x7F, 0x10, 0x28, 0x44, 0x00,
      0x00, 0x41, 0x7F, 0x40, 0x00,
      0x7C, 0x04, 0x78, 0x04, 0x78,
      0x7C, 0x08, 0x04, 0x04, 0x78,
      0x38, 0x44, 0x44, 0x44, 0x38,
      0xFC, 0x18, 0x24, 0x24, 0x18,
      0x18, 0x24, 0x24, 0x18, 0xFC,
      0x7C, 0x08, 0x04, 0x04, 0x08,
      0x48, 0x54, 0x54, 0x54, 0x24,
      0x04, 0x04, 0x3F, 0x44, 0x24,
      0x3C, 0x40, 0x40, 0x20, 0x7C,
      0x1C, 0x20, 0x40, 0x20, 0x1C,
      0x3C, 0x40, 0x30, 0x40, 0x3C,
      0x44, 0x28, 0x10, 0x28, 0x44,
      0x4C, 0x90, 0x90, 0x90, 0x7C,
      0x44, 0x64, 0x54, 0x4C, 0x44,
      0x00, 0x08, 0x36, 0x41, 0x00,
      0x00, 0x00, 0x77, 0x00, 0x00,
      0x00, 0x41, 0x36, 0x08, 0x00,
      0x02, 0x01, 0x02, 0x04, 0x02,
      0x3C, 0x26, 0x23, 0x26, 0x3C,
    };
    m_font = font != NULL ? font : default_font;

    // Initiate reset pulse
    m_rst.output();
    m_rst.low();
    m_sce.output();
    m_sce.low();
    m_dc.output();
    m_dc.low();
  }

  /**
   * @override{LCD::Device}
   * Start interaction with display.
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool begin()
  {
    // Display setup script
    static const uint8_t script[] PROGMEM = {
      SET_FUNC       | EXTENDED_INST,
      SET_VOP  	     | 0x3f,
      SET_TEMP_COEFF | 0x00,
      SET_BIAS_SYS   | 0x02,
      SET_FUNC       | BASIC_INST   | HORIZONTAL_ADDR,
      DISPLAY_CNTL   | NORMAL_MODE
    };

    // Complete reset pulse and initiate display setting
    m_rst.high();
    m_sce.high();
    write_command_P(script, sizeof(script));
    text_normal_mode();
    display_clear();
    return (true);
  }

  /**
   * @override{LCD::Device}
   * Stop interaction with device and power down.
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool end()
  {
    display_clear();
    write_command(SET_FUNC | BASIC_INST | POWER_DOWN_MODE);
    return (true);
  }

  /**
   * @override{LCD::Device}
   * Set display contrast (0..127).
   * @param[in] contrast level.
   */
  virtual void display_contrast(uint8_t level)
  {
    write_command(SET_FUNC | EXTENDED_INST);
    write_command(SET_VOP  | (level & VOP_MASK));
    write_command(SET_FUNC | BASIC_INST | HORIZONTAL_ADDR);
  }

  /**
   * @override{LCD::Device}
   * Turn display on.
   */
  virtual void display_on()
  {
    write_command(DISPLAY_CNTL | NORMAL_MODE);
  }

  /**
   * @override{LCD::Device}
   * Turn display off.
   */
  virtual void display_off()
  {
    write_command(DISPLAY_CNTL | DISPLAY_OFF);
  }

  /**
   * @override{LCD::Device}
   * Display normal mode.
   */
  virtual void display_normal()
  {
    write_command(DISPLAY_CNTL | NORMAL_MODE);
  }

  /**
   * @override{LCD::Device}
   * Display inverse mode.
   */
  virtual void display_inverse()
  {
    write_command(DISPLAY_CNTL | INVERSE_MODE);
  }

  /**
   * @override{LCD::Device}
   * Clear display and move cursor to home.
   */
  virtual void display_clear()
  {
    cursor_home();
    write_data(BACKGROUND, WIDTH * FONT_WIDTH * HEIGHT);
    cursor_home();
  }

  /**
   * @override{LCD::Device}
   * Set cursor to given position.
   * @param[in] x pixel position (0..WIDTH-1).
   * @param[in] y line position (0..LINES-1).
   */
  virtual void cursor_set(uint8_t x, uint8_t y)
  {
    if (x >= WIDTH) x = 0;
    if (y >= HEIGHT) y = 0;
    write_command(SET_X_ADDR | ((x * FONT_WIDTH) & X_ADDR_MASK));
    write_command(SET_Y_ADDR | (y & Y_ADDR_MASK));
    m_x = x;
    m_y = y;
  }

  /**
   * @override{Arduino::Print}
   * Write character to display. Handles carriage-return, line-feed,
   * backspace, alert, horizontal tab and form-feed. Returns number of
   * characters(1) or zero(0) on error.
   * @param[in] c character to write.
   * @return number of characters written(1) or zero(0) for error.
   */
  virtual size_t write(uint8_t c)
  {
    // Check for special characters
    if (c < ' ') {
      switch (c) {
      case '\a': // Check for special character: alert
	m_mode = ~m_mode;
	return (1);
      case '\b': // Check for special character: back-space
	cursor_set(m_x - 1, m_y);
	write_data(BACKGROUND, FONT_WIDTH);
	return (1);
      case '\f': // Check for special character: form-feed
	display_clear();
	return (1);
      case '\n': // Check for line-feed: clear new line
	cursor_set(0, m_y + 1);
	write_data(BACKGROUND, FONT_WIDTH * WIDTH);
      case '\r': // Carriage-return: move to start of line
	cursor_set(0, m_y);
	return (1);
      case '\t': // Check for horizontal tab
	{
	  uint8_t x = m_x + m_tab - (m_x % m_tab);
	  uint8_t y = m_y + (x >= WIDTH);
	  cursor_set(x, y);
	}
	return (1);
      default:
	return (0);
      }
    }

    // Check that the character is not clipped
    if (m_x == WIDTH) write('\n');
    m_x += 1;

    // Access font for character width and bitmap
    uint8_t width = FONT_WIDTH - 1;
    const uint8_t* fp = m_font + ((c - ' ') * width);

    // Write character to the display memory and an extra byte
    do write_data(m_mode ^ pgm_read_byte(fp++)); while (--width);
    write_data(m_mode);
    return (1);
  }

protected:
  /**
   * Instruction set (table 1, pp. 14).
   */
  enum {
    NOP = 0x00,			//!< No operation.
    SET_FUNC = 0x20,		//!< Set function.
      BASIC_INST = 0x00,	//!< Basic instruction set.
      EXTENDED_INST = 0x01,	//!< Extended instruction set control.
      HORIZONTAL_ADDR = 0x00,	//!< Horizontal addressing.
      VERTICAL_ADDR = 0x02,	//!< Vertical addressing.
      POWER_UP_MODE = 0x00,	//!< Power up mode.
      POWER_DOWN_MODE = 0x04,	//!< Power down mode.
    DISPLAY_CNTL = 0x08,	//!< Display control.
      DISPLAY_OFF = 0x00,	//!< Turn display off.
      DISPLAY_ON = 0x01,	//!< Turn display on.
      NORMAL_MODE = 0x04,	//!< Normal display mode.
      INVERSE_MODE = 0x05,	//!< Inverse display mode.
    SET_Y_ADDR = 0x40,		//!< Sets Y-address of RAM (0..5).
      Y_ADDR_MASK = 0x07,	//!< Mask Y-address.
    SET_X_ADDR = 0x80,		//!< Sets X-address of RAM (0..83).
      X_ADDR_MASK = 0x7f,	//!< Mask X-addres.
    SET_TEMP_COEFF = 0x04,	//!< Set temperature coefficient (0..3).
    SET_BIAS_SYS = 0x10,	//!< Set Bias System (0..7).
    SET_VOP = 0x80,		//!< Write Vop to register (0..127).
      VOP_MASK = 0x7f,		//!< Mask Vop.
    SCRIPT_END = 0xff		//!< Init script end.
  } __attribute__((packed));

  /** Background pattern. */
  static const uint8_t BACKGROUND = 0x00;

  /** Reset pin. */
  GPIO<RST_PIN> m_rst;

  /** Screen chip enable pin. */
  GPIO<SCE_PIN> m_sce;

  /** Data/control select pin. */
  GPIO<DC_PIN> m_dc;

  /** Screen data input and clock pins; Serial Ouput. */
  SRPO<MSBFIRST, SDIN_PIN, SCLK_PIN> m_srpo;

  /** Font (5x7), program memory pointer. */
  const uint8_t* m_font;

  void write_data(uint8_t value)
  {
    m_sce.low();
    m_srpo.write(value);
    m_sce.high();
  }

  void write_data(uint8_t value, size_t count)
  {
    if (count == 0) return;
    m_sce.low();
    do m_srpo.write(value); while (--count);
    m_sce.high();
  }

  void write_command(uint8_t value)
  {
    m_dc.low();
    m_sce.low();
    m_srpo.write(value);
    m_sce.high();
    m_dc.high();
  }

  void write_command_P(const uint8_t* buf, size_t count)
  {
    if (count == 0) return;
    m_dc.low();
    m_sce.low();
    do m_srpo.write(pgm_read_byte(buf++)); while (--count);
    m_sce.high();
    m_dc.high();
  }
};

#endif
