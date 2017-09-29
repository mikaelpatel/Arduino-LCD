/**
 * @file LCD/Adapter/SR3W.h
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
#ifndef LCD_ADAPTER_SR3W_H
#define LCD_ADAPTER_SR3W_H

#include "LCD.h"
#include "Driver/HD44780.h"

/**
 * HD44780 (LCD-II) Dot Matix Liquid Crystal Display Controller/Driver
 * Shift Register 3-Wire Port (SR3W), 74HC595/74HC164 (SR[pin]),
 * with digital output pins.
 *
 * @param[in] SDA_PIN serial data pin (Default BOARD::D7)
 * @param[in] SCL_PIN serial clock pin (Default BOARD::D6)
 * @param[in] EN_PIN enable pulse (Default BOARD::D5)
 *
 * @section Circuit
 * @code
 *                         74HC595    (VCC)
 *                       +----U----+    |
 * (LCD D5)------------1-|Q1    VCC|-16-+
 * (LCD D6)------------2-|Q2     Q0|-15-----------(LCD D4)
 * (LCD D7)------------3-|Q3    SER|-14-----------(SDA/D7)
 * (LCD RS)------------4-|Q4    /OE|-13-----------(GND)
 * (LCD BT)------------5-|Q5   RCLK|-12-----------(EN/D5)
 *                     6-|Q6   SCLK|-11-----------(SCL/D6)
 *                     7-|Q7    /MR|-10-----------(VCC)
 *                   +-8-|GND   Q6'|--9
 *                   |   +---------+
 *                   |      0.1uF
 *                 (GND)-----||----(VCC)
 * (LCD EN)---------------------------------------(EN/D5)
 * (LCD RW)---------------------------------------(GND)
 * (LCD K)----------------------------------------(GND)
 * (LCD A)-----------------[330]------------------(VCC)
 * @endcode
 *
 * Alternative circuit with 74HC164.
 * @code
 *                         74HC164    (VCC)
 *                       +----U----+    |
 * (SDA/D7)----------+-1-|DSA   VCC|-14-+
 *                   +-2-|DSB    Q7|-13
 * (LCD D4)------------3-|Q0     Q6|-12
 * (LCD D5)------------4-|Q1     Q5|-11--------(LCD BT)
 * (LCD D6)------------5-|Q2     Q4|-10--------(LCD RS)
 * (LCD D7)------------6-|Q3    /MR|--9-----------(VCC)
 *                   +-7-|GND    CP|--8--------(SCL/D6)
 *                   |   +---------+
 *                   |      0.1uF
 *                 (GND)-----||----(VCC)
 * (LCD EN)---------------------------------------(EN/D5)
 * (LCD RW)---------------------------------------(GND)
 * (LCD K)----------------------------------------(GND)
 * (LCD A)-----------------[330]------------------(VCC)
 * @endcode
 */
namespace LCD {
template<BOARD::pin_t SDA_PIN = BOARD::D7,
	 BOARD::pin_t SCL_PIN = BOARD::D6,
	 BOARD::pin_t EN_PIN = BOARD::D5>
class SR3W : public HD44780::Adapter {
public:
  /**
   * Construct HD44780 3-wire serial adapter and initiate pins.
   */
  SR3W() :
    HD44780::Adapter(),
    m_port()
  {
    m_sda.output();
    m_scl.output();
    m_en.output();
  }

  /**
   * @override{HD44780::Adapter}
   * Use 4-bit mode. Returns false.
   * @return bool.
   */
  virtual bool setup()
  {
    return (false);
  }

  /**
   * @override{HD44780::Adapter}
   * Write 4-bit data to display using shift register.
   * @param[in] data (4b) to write.
   */
  virtual void write4b(uint8_t data)
  {
    m_port.data = data;
    uint8_t value = m_port;
    for (uint8_t mask = 0x20; mask; mask >>= 1) {
      m_sda.write(value & mask);
      m_scl.toggle();
      m_scl.toggle();
    }
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
    write4b(data >> 4);
    write4b(data);
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
    m_port.rs = flag;
  }

  /**
   * @override{HD44780::Adapter}
   * Set backlight on/off using bt pin.
   * @param[in] flag.
   */
  virtual void set_backlight(uint8_t flag)
  {
    m_port.bt = flag;
  }

protected:
  /** Execution time delay (us). */
  static const uint16_t SHORT_EXEC_TIME = 32;

  /** Shift register port bit fields; little endian. */
  union port_t {
    uint8_t as_uint8;		//!< Unsigned byte access.
    struct {
      uint8_t data:4;		//!< Data port (Q0..Q3).
      uint8_t rs:1;		//!< Command/Data select (Q4).
      uint8_t bt:1;		//!< Back-light control (Q5).
      uint8_t app2:1;		//!< Application bit#2 (Q6).
      uint8_t app1:1;		//!< Application bit#1 (Q7).
    };
    operator uint8_t() { return (as_uint8); }
    port_t() { as_uint8 = 0; }
  };

  port_t m_port;		//!< Port setting.
  GPIO<SDA_PIN> m_sda;		//!< Serial data output.
  GPIO<SCL_PIN> m_scl;		//!< Serial clock.
  GPIO<EN_PIN> m_en;		//!< Starts data read/write.
};
};
#endif
