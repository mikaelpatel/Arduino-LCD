/**
 * @file LCD/Adapter/Debug.h
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
#ifndef LCD_ADAPTER_DEBUG_H
#define LCD_ADAPTER_DEBUG_H

#include "LCD.h"
#include "Driver/HD44780.h"

/**
 * Debug Adapter for HD44780 (LCD-II) Dot Matix Liquid Crystal Display
 * Controller/Driver. Prints driver commands as sent to the device.
 */
namespace LCD {
class Debug : public HD44780::Adapter {
public:
  virtual bool setup()
  {
    Serial.print(millis());
    Serial.println(F(":setup:"));
    return (false);
  }

  virtual void write4b(uint8_t data)
  {
    Serial.print(millis());
    Serial.print(F(":write4b:"));
    Serial.println(data & 0xf, HEX);
  }

  virtual void write8b(uint8_t data)
  {
    Serial.print(millis());
    Serial.print(F(":write8b:"));
    if (data < 0x10) Serial.print('0');
    Serial.println(data, HEX);
  }

  virtual void set_mode(uint8_t flag)
  {
    Serial.print(millis());
    Serial.print(F(":set_mode:"));
    if (flag < 0x10) Serial.print('0');
    Serial.println(flag, HEX);
  }

  virtual void set_backlight(uint8_t flag)
  {
    Serial.print(millis());
    Serial.print(F(":set_backlight:"));
    if (flag < 0x10) Serial.print('0');
    Serial.println(flag, HEX);
  }
};
};
#endif
