/**
 * @file LCD_Keypad.h
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

#ifndef LCD_KEYPAD_H
#define LCD_KEYPAD_H

#include "LCD.h"
#include "GPIO.h"
#include "Keypad.h"
#include "Driver/HD44780.h"
#include "Adapter/PP7W.h"

/**
 * Device driver for the LCD_Keypad Shield with HD44780 16x2 display
 * and five keys (SELECT, UP, DOWN, LEFT, and RIGHT).
 */
class LCD_Keypad : public Keypad<A0>, public HD44780 {
public:
  enum {
    NO_KEY = 0,
    SELECT_KEY,
    LEFT_KEY,
    DOWN_KEY,
    UP_KEY,
    RIGHT_KEY
  } __attribute__((packed));

  /** Construct shield with device driver and keypad handler. **/
  LCD_Keypad() : Keypad(keymap()), HD44780(m_io) {}

protected:
  /** Parallel Port Adapter for shield. */
  LCD::PP7W<BOARD::D4, BOARD::D5, BOARD::D6, BOARD::D7,
	    BOARD::D8, BOARD::D9, BOARD::D10> m_io;

  /**
   * Return key map, program memory vector in descent order.
   * @return key map.
   */
  const uint16_t* keymap()
    __attribute__((always_inline))
  {
    static const uint16_t map[] PROGMEM = {
      1000, 700, 400, 300, 100, 0
    };
    return (map);
  }
};

#endif
