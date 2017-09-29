/**
 * @file LCD/Adapter/Port4b.h
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
#ifndef LCD_ADAPTER_PORT4B_H
#define LCD_ADAPTER_PORT4B_H

#include "LCD.h"
#include "Driver/HD44780.h"

/**
 * Adapter for HD44780 (LCD-II) Dot Matix Liquid Crystal Display
 * Controller/Driver when using GPIO pins. Default pins support the
 * LCD Keypad Shield.
 * @param[in] D0_PIN data pin (default BOARD::D4).
 * @param[in] D1_PIN data pin (default BOARD::D5).
 * @param[in] D2_PIN data pin (default BOARD::D6).
 * @param[in] D2_PIN data pin (default BOARD::D7).
 * @param[in] RS_PIN command/data select pin (default BOARD::D8).
 * @param[in] EN_PIN enable pin (default BOARD::D9).
 * @param[in] BT_PIN backlight pin (default BOARD::D10).
 *
 * @section Circuit
 * @code
 *                           HD44780
 *                       +------------+
 * (GND)---------------1-|VSS         |
 * (VCC)---------------2-|VDD         |
 *                     3-|VO          |
 * (D8)----------------4-|RS          |
 *                     5-|RW          |
 * (D9)----------------6-|EN          |
 *                     7-|D0          |
 *                     8-|D1          |
 *                     9-|D2          |
 *                    10-|D3          |
 * (D4)---------------11-|D4          |
 * (D5)---------------12-|D5          |
 * (D6)---------------13-|D6          |
 * (D7)---------------14-|D7          |
 * (VCC)-+------------15-|A           |
 *       ¡         +--16-|K           |
 *     [4K7]       |     +------------+
 *       |         |
 * (D10)-+-[10K]-|< NPN 9013
 * (/BT)           v
 *                 |
 * (GND)-----------+
 * @endcode
 */
namespace LCD {
template<BOARD::pin_t D0_PIN = BOARD::D4,
	 BOARD::pin_t D1_PIN = BOARD::D5,
	 BOARD::pin_t D2_PIN = BOARD::D6,
	 BOARD::pin_t D3_PIN = BOARD::D7,
	 BOARD::pin_t RS_PIN = BOARD::D8,
	 BOARD::pin_t EN_PIN = BOARD::D9,
	 BOARD::pin_t BT_PIN = BOARD::D10>
class Port4b : public HD44780::Adapter {
public:
  /**
   * Construct HD44780 4-bit parallel port connected to given command,
   * enable and backlight pin.
   */
  Port4b() : HD44780::Adapter()
  {
    m_d0.output();
    m_d1.output();
    m_d2.output();
    m_d3.output();
    m_rs.output();
    m_en.output();
    m_bt.output();
    m_bt.high();
  }

  /**
   * @override{HD44780::Adapter}
   * Initiate 4-bit parallel port. Returns false.
   * @return bool.
   */
  virtual bool setup()
  {
    return (false);
  }

  /**
   * @override{HD44780::Adapter}
   * Write LSB nibble to display data pins.
   * @param[in] data (4b) to write.
   */
  virtual void write4b(uint8_t data)
  {
    m_d0.write(data & 0x01);
    m_d1.write(data & 0x02);
    m_d2.write(data & 0x04);
    m_d3.write(data & 0x08);
    m_en.toggle();
    m_en.toggle();
  }

  /**
   * @override{HD44780::Adapter}
   * Write byte (8bit) to display.
   * @param[in] data (8b) to write.
   */
  virtual void write8b(uint8_t data)
  {
    m_d0.write(data & 0x10);
    m_d1.write(data & 0x20);
    m_d2.write(data & 0x40);
    m_d3.write(data & 0x80);
    m_en.toggle();
    m_en.toggle();
    m_d0.write(data & 0x01);
    m_d1.write(data & 0x02);
    m_d2.write(data & 0x04);
    m_d3.write(data & 0x08);
    m_en.toggle();
    m_en.toggle();
    delayMicroseconds(SHORT_EXEC_TIME);
  }

  /**
   * @override{HD44780::Adapter}
   * Set instruction/data mode using given rs pin; zero for
   * instruction, non-zero for data mode.
   * @param[in] flag.
   */
  virtual void set_mode(uint8_t flag)
  {
    m_rs.write(flag);
  }

  /**
   * @override{HD44780::Adapter}
   * Set backlight on/off using bt pin.
   * @param[in] flag.
   */
  virtual void set_backlight(uint8_t flag)
  {
    m_bt.write(flag);
  }

protected:
  /** Execution time delay (us). */
  static const uint16_t SHORT_EXEC_TIME = 32;

  GPIO<D0_PIN> m_d0;		//!< Data pin; d0.
  GPIO<D1_PIN> m_d1;		//!< Data pin; d1.
  GPIO<D2_PIN> m_d2;		//!< Data pin; d2.
  GPIO<D3_PIN> m_d3;		//!< Data pin; d3.
  GPIO<RS_PIN> m_rs;		//!< Register select (0/instruction, 1/data).
  GPIO<EN_PIN> m_en;		//!< Starts data read/write.
  GPIO<BT_PIN> m_bt;		//!< Back-light control (0/on, 1/off).
};
};
#endif
