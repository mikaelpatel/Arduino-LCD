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
#include "Driver/HD44780.h"
#include "Adapter/PP7W.h"

/**
 * Device driver for the LCD_Keypad Shield with HD44780 16x2 display
 * and five keys (select, up, down, left, right).
 */
class LCD_Keypad : public HD44780 {
public:
  LCD_Keypad() : HD44780(m_io) {}

protected:
  LCD::PP7W<BOARD::D4, BOARD::D5, BOARD::D6, BOARD::D7,
	    BOARD::D8, BOARD::D9, BOARD::D10> m_io;
};

#endif
