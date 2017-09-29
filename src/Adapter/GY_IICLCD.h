/**
 * @file LCD/Adapter/GY_IICLCD.h
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

#ifndef LCD_ADAPTER_GY_IICLCD_H
#define LCD_ADAPTER_GY_IICLCD_H

#include "LCD.h"
#include "Driver/HD44780.h"
#include "TWI.h"
#include "MJKDZ.h"
/**
 * Adapter for HD44780 (LCD-II) Dot Matix Liquid Crystal Display
 * Controller/Driver when using the GYIICLCD IO expander board based
 * on PCF8574 I2C IO expander device driver. Has the same port
 * connection as MJKDZ. The difference is the default TWI
 * sub-address.
 *
 * @section Circuit
 * @code
 *                      PCF8574/GY-IICLCD
 *                       +-----U------+
 * (GND)---[X]---------1-|A0       VCC|-16--------------(VCC)
 * (GND)---[X]---------2-|A1       SDA|-15-----------(SDA/A4)
 * (GND)---[X]---------3-|A2       SCL|-14-----------(SCL/A5)
 * (LCD/D4)------------4-|P0       INT|-13
 * (LCD/D5)------------5-|P1        P7|-12-----------(LCD/BT)
 * (LCD/D6)------------6-|P2        P6|-11-----------(LCD/RS)
 * (LCD/D7)------------7-|P3        P5|-10-----------(LCD/RW)
 * (GND)---------------8-|GND       P4|-9------------(LCD/EN)
 *                       +------------+
 * @endcode
 */
namespace LCD {
class GY_IICLCD : public MJKDZ {
public:
  /**
   * Construct HD44780 IO port handler using the GY-IICLCD I2C/TWI
   * I/O expander with given sub-address (A0..A2).
   * @param[in] twi bus manager.
   * @param[in] subaddr sub-address (0..7, default 0).
   */
  GY_IICLCD(TWI& twi, uint8_t subaddr = 0) : MJKDZ(twi, subaddr) {}
};
};

#endif
