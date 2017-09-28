/**
 * @file LCD.h
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

#ifndef LCD_H
#define LCD_H

namespace LCD {
/**
 * Common interface for LCD; LCD::Device as the base class for device drivers.
 */
class Device : public Print {
public:
  /**
   * Text display mode.
   */
  enum {
    NORMAL_TEXT_MODE = 0x00,
    INVERTED_TEXT_MODE = 0xff
  } __attribute__((packed));

  /**
   * Initiate generic LCD device driver.
   */
  Device() :
    Print(),
    m_x(0),
    m_y(0),
    m_tab(4),
    m_mode(0)
  {}

  /**
   * @override{LCD::Device}
   * Start display for text output. Returns true if successful
   * otherwise false.
   * @return boolean.
   */
  virtual bool begin() = 0;

  /**
   * @override{LCD::Device}
   * Stop display and power down. Returns true if successful
   * otherwise false.
   */
  virtual bool end() = 0;

  /**
   * @override{LCD::Device}
   * Turn display backlight on.
   */
  virtual void backlight_on() {}

  /**
   * @override{LCD::Device}
   * Turn display backlight off.
   */
  virtual void backlight_off() {}

  /**
   * @override{LCD::Device}
   * Set display contrast level.
   * @param[in] level to set.
   */
  virtual void display_contrast(uint8_t level)
  {
    (void) level;
  }

  /**
   * @override{LCD::Device}
   * Turn display on.
   */
  virtual void display_on() = 0;

  /**
   * @override{LCD::Device}
   * Turn display off.
   */
  virtual void display_off() = 0;

  /**
   * @override{LCD::Device}
   * Display normal mode.
   */
  virtual void display_normal() {}

  /**
   * @override{LCD::Device}
   * Display inverse mode.
   */
  virtual void display_inverse() {}

  /**
   * @override{LCD::Device}
   * Clear display and move cursor to home.
   */
  virtual void display_clear() = 0;

  /**
   * Get current cursor position.
   * @param[out] x.
   * @param[out] y.
   */
  void get_cursor(uint8_t& x, uint8_t& y) const
    __attribute__((always_inline))
  {
    x = m_x;
    y = m_y;
  }

  /**
   * @override{LCD:}
   * Set cursor position to given position.
   * @param[in] x.
   * @param[in] y.
   */
  virtual void set_cursor(uint8_t x, uint8_t y) = 0;

  /**
   * Get tab step.
   * @return tab step.
   */
  uint8_t tab_step() const
    __attribute__((always_inline))
  {
    return (m_tab);
  }

  /**
   * Set tab step to given value.
   * @param[in] step tab.
   */
  void tab_step(uint8_t step)
    __attribute__((always_inline))
  {
    m_tab = step;
  }

  /**
   * Get text mode.
   */
  uint8_t text_mode() const
    __attribute__((always_inline))
  {
    return (m_mode);
  }

  /**
   * Set text mode.
   * @param[in] mode new text mode.
   */
  void text_mode(uint8_t mode)
    __attribute__((always_inline))
  {
    m_mode = mode;
  }

protected:
  uint8_t m_x;			//!< Cursor position x.
  uint8_t m_y;			//!< Cursor position y.
  uint8_t m_tab;		//!< Tab step.
  uint8_t m_mode;		//!< Text mode.
};
};
#endif
