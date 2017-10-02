/**
 * @file LCD4884.h
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

#ifndef LCD4884_H
#define LCD4884_H

#include "LCD.h"
#include "GPIO.h"
#include "Driver/PCD8544.h"

/**
 * Device driver for LCD4884 Shield with PCD8544 display and analog joy stick.
 * Note: The Joy Stick and varible backlight is not yet implemented.
 */
class LCD4884 : public PCD8544<BOARD::D5, BOARD::D4, BOARD::D3, BOARD::D2> {
  typedef PCD8544<BOARD::D5, BOARD::D4, BOARD::D3, BOARD::D2> Display;
public:
  /**
   * Construct display device driver and initiate pins. The parameter
   * should a 5x7 font in program memory or NULL for the PCD8544 default
   * font.
   * @param[in] font in program memory (Default NULL).
   */
  LCD4884(const uint8_t* font = NULL) :
    Display(font)
  {
    m_rst.output();
    m_rst.low();
    m_bl.output();
    m_bl.low();
  }

  /**
   * @override{LCD::Device}
   * Start interaction with shield.
   * @return true(1) if successful otherwise false(0)
   */
  virtual bool begin()
  {
    m_rst.high();
    return (Display::begin());
  }

  /**
   * @override{LCD::Device}
   * Turn display backlight on.
   */
  virtual void backlight_on()
  {
    m_bl.high();
  }

  /**
   * @override{LCD::Device}
   * Turn display backlight off.
   */
  virtual void backlight_off()
  {
    m_bl.low();
  }

protected:
  /** Reset pin. */
  GPIO<BOARD::D6> m_rst;

  /** Backlight pin. */
  GPIO<BOARD::D7> m_bl;
};

#endif
