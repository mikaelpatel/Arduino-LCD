/**
 * @file HD44780.h
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

#ifndef HD44780_H
#define HD44780_H

#include "LCD.h"

/**
 * LCD Device Driver for HD44780 (LCD-II) Dot Matix Liquid Crystal
 * Display Controller/Driver. Supports simple text scroll, cursor, and
 * handling of special characters such as carriage-return, form-feed,
 * back-space, horizontal tab and new-line.
 *
 * @section References
 * 1. Product Specification, Hitachi, HD4478U, ADE-207-272(Z), '99.9, Rev. 0.0.
 */
class HD44780 : public LCD::Device {
public:
  /**
   * Abstract HD44780 Adapter class; isolate communication specific
   * functions and allow access over parallel and serial interfaces.
   */
  class Adapter {
  public:
    /**
     * @override{HD44780::Adapter}
     * Initiate IO port. Called by HD44780::begin(). Should return true(1)
     * for 8-bit mode otherwise false for 4-bit mode. Default is 4-bit mode.
     * @return bool.
     */
    virtual bool setup()
    {
      return (false);
    }

    /**
     * @override{HD44780::Adapter}
     * Write LSB nibble (4bit) to display.
     * @param[in] data (4b) to write.
     */
    virtual void write4b(uint8_t data) = 0;

    /**
     * @override{HD44780::Adapter}
     * Write byte (8bit) to display.
     * @param[in] data (8b) to write.
     */
    virtual void write8b(uint8_t data)
    {
      write4b(data >> 4);
      write4b(data);
    }

    /**
     * @override{HD44780::Adapter}
     * Write character/command buffer to display.
     * @param[in] buf pointer to buffer.
     * @param[in] size number of bytes in buffer.
     */
    virtual void write8n(const void* buf, size_t size)
    {
      const uint8_t* bp = (const uint8_t*) buf;
      while (size--) write8b(*bp++);
    }

    /**
     * @override{HD44780::Adapter}
     * Set data/command mode; zero(0) for command,
     * non-zero(1) for data mode.
     * @param[in] flag.
     */
    virtual void set_mode(uint8_t flag) = 0;

    /**
     * @override{HD44780::Adapter}
     * Set backlight on/off.
     * @param[in] flag.
     */
    virtual void set_backlight(uint8_t flag) = 0;
  };

  /** Max size of custom character font bitmap. */
  static const uint8_t BITMAP_MAX = 8;

  /** Display width (characters per line). */
  const uint8_t WIDTH;

  /** Display height (lines). */
  const uint8_t HEIGHT;

  /**
   * Construct HD44780 LCD connected to given adapter. The display is
   * initiated by calling begin(). Default display is 1602.
   * @param[in] io port adapter.
   * @param[in] width of display, characters per line (Default 16).
   * @param[in] height of display, number of lines (Default 2).
   */
  HD44780(Adapter& io, uint8_t width = 16, uint8_t height = 2) :
    LCD::Device(),
    WIDTH(width),
    HEIGHT(height),
    m_io(io),
    m_mode(ENTRY_MODE_SET | INCREMENT),
    m_cntl(CONTROL_SET),
    m_func(FUNCTION_SET | DATA_LENGTH_4BITS | NR_LINES_2 | FONT_5X8DOTS),
    m_offset(NULL)
  {}

  /**
   * @override{LCD::Device}
   * Start display for text output. Returns true(1) if successful
   * otherwise false(0).
   * @return bool.
   */
  virtual bool begin()
  {
    // Initiate display; See fig. 24, 4-bit interface, pp. 46.
    // http://web.alfredstate.edu/weimandn/lcd/lcd_initialization/-
    // LCD%204-bit%20Initialization%20v06.pdf
    static const uint8_t offset0[] PROGMEM = { 0x00, 0x40, 0x14, 0x54 };
    static const uint8_t offset1[] PROGMEM = { 0x00, 0x40, 0x10, 0x50 };
    m_offset = ((HEIGHT == 4) && (WIDTH == 16) ? offset1 : offset0);
    const uint8_t FS0 = (FUNCTION_SET | DATA_LENGTH_8BITS);
    const uint8_t FS1 = (FUNCTION_SET | DATA_LENGTH_4BITS);
    bool mode = m_io.setup();
    delay(POWER_ON_TIME);
    // 4-bit initialization mode
    if (!mode) {
      m_io.write4b(FS0 >> 4);
      delayMicroseconds(INIT0_TIME);
      m_io.write4b(FS0 >> 4);
      delayMicroseconds(INIT1_TIME);
      m_io.write4b(FS0 >> 4);
      delayMicroseconds(INIT1_TIME);
      m_io.write4b(FS1 >> 4);
      delayMicroseconds(INIT1_TIME);
    }
    // 8-bit initialization mode
    else {
      m_io.write8b(m_func |= DATA_LENGTH_8BITS);
    }

    // Initialization with the function, control and mode setting
    m_io.write8b(m_func);
    m_io.write8b(m_cntl);

    // Initialization completed. Turn on the display and backlight
    text_normal_mode();
    display_clear();
    display_on();
    backlight_on();
    return (true);
  }

  /**
   * @override{LCD::Device}
   * Turn display backlight on.
   */
  virtual void backlight_on()
  {
    m_io.set_backlight(true);
  }

  /**
   * @override{LCD::Device}
   * Turn display backlight off.
   */
  virtual void backlight_off()
  {
    m_io.set_backlight(false);
  }

  /**
   * @override{LCD::Device}
   * Turn display on.
   */
  virtual void display_on()
  {
    m_io.write8b(m_cntl |= DISPLAY_ON);
  }

  /**
   * @override{LCD::Device}
   * Turn display off.
   */
  virtual void display_off()
  {
    m_io.write8b(m_cntl &= ~DISPLAY_ON);
  }

  /**
   * @override{LCD::Device}
   * Clear display and move cursor to home(0, 0).
   */
  virtual void display_clear()
  {
    m_io.write8b(CLEAR_DISPLAY);
    m_x = 0;
    m_y = 0;
    m_mode |= INCREMENT;
    delayMicroseconds(LONG_EXEC_TIME);
  }

  /**
   * @override{LCD::Device}
   * Turn cursor blink on.
   */
  virtual void cursor_blink_on()
  {
    m_io.write8b(m_cntl |= BLINK_ON);
  }

  /**
   * @override{LCD::Device}
   * Turn cursor blink off.
   */
  virtual void cursor_blink_off()
  {
    m_io.write8b(m_cntl &= ~BLINK_ON);
  }

  /**
   * @override{LCD::Device}
   * Set cursor position to given position.
   * @param[in] x.
   * @param[in] y.
   */
  virtual void cursor_set(uint8_t x, uint8_t y)
  {
    if (x >= WIDTH) x = 0;
    if (y >= HEIGHT) y = 0;
    uint8_t offset = (uint8_t) pgm_read_byte(&m_offset[y]);
    m_io.write8b(SET_DDRAM_ADDR | ((x + offset) & SET_DDRAM_MASK));
    m_x = x;
    m_y = y;
  }

  /**
   * @override{LCD::Device}
   * Move cursor to home position(0, 0) .
   */
  virtual void cursor_home()
  {
    m_io.write8b(RETURN_HOME);
    m_x = 0;
    m_y = 0;
    delayMicroseconds(LONG_EXEC_TIME);
  }

  /**
   * Set display scrolling left.
   */
  void display_scroll_left()
    __attribute__((always_inline))
  {
    m_io.write8b(SHIFT_SET | DISPLAY_MOVE | MOVE_LEFT);
  }

  /**
   * Set display scrolling right.
   */
  void display_scroll_right()
    __attribute__((always_inline))
  {
    m_io.write8b(SHIFT_SET | DISPLAY_MOVE | MOVE_RIGHT);
  }

  /**
   * Turn underline cursor on.
   */
  void cursor_underline_on()
    __attribute__((always_inline))
  {
    m_io.write8b(m_cntl |= CURSOR_ON);
  }

  /**
   * Turn underline cursor off.
   */
  void cursor_underline_off()
    __attribute__((always_inline))
  {
    m_io.write8b(m_cntl &= ~CURSOR_ON);
  }


  /**
   * Set text flow right-to-left.
   */
  void text_flow_right_to_left()
    __attribute__((always_inline))
  {
    m_io.write8b(m_cntl &= ~INCREMENT);
  }

  /**
   * Set text scroll left adjust.
   */
  void text_scroll_left_adjust()
    __attribute__((always_inline))
  {
    m_io.write8b(m_cntl |= DISPLAY_SHIFT);
  }

  /**
   * Set text scroll right adjust.
   */
  void text_scroll_right_adjust()
    __attribute__((always_inline))
  {
    m_io.write8b(m_cntl &= ~DISPLAY_SHIFT);
  }

  /**
   * Set custom character bitmap for given identity (0..7).
   * @param[in] id character.
   * @param[in] bitmap pointer to bitmap.
   */
  void set_custom_char(uint8_t id, const uint8_t* bitmap)
  {
    m_io.write8b(SET_CGRAM_ADDR | ((id << 3) & SET_CGRAM_MASK));
    m_io.set_mode(true);
    for (uint8_t i = 0; i < BITMAP_MAX; i++)
      m_io.write8b(*bitmap++);
    m_io.set_mode(false);
  }

  /**
   * Set custom character bitmap to given identity (0..7).
   * The bitmap should be stored in program memory.
   * @param[in] id character.
   * @param[in] bitmap pointer to program memory bitmap.
   */
  void set_custom_char_P(uint8_t id, const uint8_t* bitmap)
  {
    m_io.write8b(SET_CGRAM_ADDR | ((id << 3) & SET_CGRAM_MASK));
    m_io.set_mode(true);
    for (uint8_t i = 0; i < BITMAP_MAX; i++, bitmap++)
      m_io.write8b(pgm_read_byte(bitmap));
    m_io.set_mode(false);
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
      case '\a': // Alert: invert text mode
	m_mode = ~m_mode;
	return (1);
      case '\b': // Back-space: move cursor back one step (if possible)
	cursor_set(m_x - 1, m_y);
	write(' ');
	cursor_set(m_x - 1, m_y);
	return (1);
      case '\f': // Form-feed: clear the display
	display_clear();
	return (1);
      case '\n': // New-line: clear line
	{
	  cursor_set(0, m_y + 1);
	  m_io.set_mode(true);
	  for (uint8_t i = 0; i < WIDTH; i++) write(' ');
	  m_io.set_mode(false);
	  cursor_set(m_x, m_y);
	}
	return (1);
      case '\r': // Carriage-return: move to start of line
	cursor_set(0, m_y);
	return (1);
      case '\t': // Horizontal tab
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

    // Write character
    if (m_x == WIDTH) write('\n');
    m_x += 1;
    m_io.set_mode(true);
    m_io.write8b(c);
    m_io.set_mode(false);
    return (c & 0xff);
  }

protected:
  /**
   * Bus Timing Characteristics (in micro-seconds), fig. 25, pp. 50.
   */
  static const uint16_t LONG_EXEC_TIME = 1600;
  static const uint16_t POWER_ON_TIME = 48;
  static const uint16_t INIT0_TIME = 4500;
  static const uint16_t INIT1_TIME = 150;

  /**
   * Instructions (Table 6, pp. 24), RS(0), RW(0).
   */
  enum {
    CLEAR_DISPLAY = 0x01,    	//!< Clears entrire display and return home.
    RETURN_HOME = 0x02,	     	//!< Sets DDRAM 0 in address counter.
    ENTRY_MODE_SET = 0x04,	//!< Sets cursor move direction and display shift.
    CONTROL_SET = 0x08,	 	//!< Set display, cursor and blinking controls.
    SHIFT_SET = 0x10,		//!< Set cursor and shifts display.
    FUNCTION_SET = 0x20,	//!< Sets interface data length, line and font.
    SET_CGRAM_ADDR = 0x40,	//!< Sets CGRAM address.
    SET_CGRAM_MASK = 0x3f,	//!< - Mask (6-bit).
    SET_DDRAM_ADDR = 0x80,	//!< Sets DDRAM address.
    SET_DDRAM_MASK = 0x7f,	//!< - Mask (7-bit).
    BIAS_RESISTOR_SET = 0x04,	//!< Bias resistor select.
    BIAS_RESISTOR_MASK = 0x03,	//!< - Mask (2-bit).
    COM_SEG_SET = 0x40,		//!< COM SEG direction select.
    COM_SET_MASK = 0x0f,	//!< - mask (4 bit).
    SET_DDATA_LENGTH = 0x80,	//!< Set display data length.
    SET_DDATA_MASK = 0x7f	//!< - mask (7 bit, 0..79 => 1..80).
  } __attribute__((packed));

  /**
   * ENTRY_MODE_SET attributes.
   */
  enum {
    DISPLAY_SHIFT = 0x01,	//!< Shift the entire display not cursor.
    INCREMENT = 0x02,		//!< Increment (right) on write.
    DECREMENT = 0x00		//!< Decrement (left) on write.
  } __attribute__((packed));

  /**
   * CONTROL_SET attributes.
   */
  enum {
    BLINK_ON = 0x01,		//!< The character indicated by cursor blinks.
    CURSOR_ON = 0x02,		//!< The cursor is displayed.
    DISPLAY_ON = 0x04,		//!< The display is on.
  } __attribute__((packed));

  /**
   * SHIFT_SET attributes.
   */
  enum {
    MOVE_LEFT = 0x00,		//!< Moves cursor and shifts display.
    MOVE_RIGHT = 0x04,		//!< without changing DDRAM contents.
    CURSOR_MODE = 0x00,
    DISPLAY_MOVE = 0x08
  } __attribute__((packed));

  /**
   * FUNCTION_SET attributes.
   */
  enum {
    DATA_LENGTH_4BITS = 0x00,	//!< Sets the interface data length, 4-bit or.
    DATA_LENGTH_8BITS = 0x10,	//!< - 8-bit.
    NR_LINES_1 = 0x00,		//!< Sets the number of display lines, 1 or.
    NR_LINES_2 = 0x08,		//!< - 2.
    FONT_5X8DOTS = 0x00,	//!< Sets the character font, 5X8 dots or.
    FONT_5X10DOTS = 0x04,	//!< - 5X10 dots.
    BASIC_SET = 0x00,		//!< Sets basic instruction set.
    EXTENDED_SET = 0x04		//!< - extended instruction set.
  } __attribute__((packed));

  /** Display pins and state (mirror of device registers). */
  Adapter& m_io;		//!< IO port adapter.
  uint8_t m_mode;		//!< Entry mode.
  uint8_t m_cntl;		//!< Control.
  uint8_t m_func;		//!< Function set.
  const uint8_t* m_offset;	//!< Row offset table.
};
#endif
