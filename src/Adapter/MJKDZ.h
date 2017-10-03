/**
 * @file LCD/Adapter/MJKDZ.h
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

#ifndef LCD_ADAPTER_MJKDZ_H
#define LCD_ADAPTER_MJKDZ_H

#include "LCD.h"
#include "Driver/HD44780.h"
#include "TWI.h"
#include "Driver/PCF8574.h"

/**
 * Adapter for HD44780 (LCD-II) Dot Matix Liquid Crystal Display
 * Controller/Driver when using the MJKDZ IO expander board based on
 * PCF8574 I2C IO expander device driver.
 *
 * @section Circuit
 * @code
 *                       PCF8574/MJKDZ
 *                       +-----U------+
 * (GND)---[ ]---------1-|A0       VCC|-16--------------(VCC)
 * (GND)---[ ]---------2-|A1       SDA|-15-----------(SDA/A4)
 * (GND)---[ ]---------3-|A2       SCL|-14-----------(SCL/A5)
 * (LCD D4)------------4-|P0       INT|-13
 * (LCD D5)------------5-|P1        P7|-12-----------(LCD BT)
 * (LCD D6)------------6-|P2        P6|-11-----------(LCD RS)
 * (LCD D7)------------7-|P3        P5|-10-----------(LCD RW)
 * (GND)---------------8-|GND       P4|-9------------(LCD EN)
 *                       +------------+
 * @endcode
 */
namespace LCD {
class MJKDZ : public HD44780::Adapter, private PCF8574 {
public:
  /**
   * Construct HD44780 adapter when using the MJKDZ I2C/TWI
   * I/O expander with given sub-address (A0..A2).
   * @param[in] subaddr sub-address (0..7, default 7).
   */
  MJKDZ(TWI& twi, uint8_t subaddr = 7) :
    PCF8574(twi, subaddr),
    m_port()
  {}

  /**
   * @override{HD44780::Adapter}
   * Initiate TWI interface. Returns false.
   * @return bool.
   */
  virtual bool setup()
  {
    ddr(0);
    return (false);
  }

  /**
   * @override{HD44780::Adapter}
   * Write nibble to display using TWI interface.
   * @param[in] data (4b) to write.
   */
  virtual void write4b(uint8_t data)
  {
    uint8_t buf[2];
    m_port.data = data;
    m_port.en = 1;
    buf[0] = m_port;
    m_port.en = 0;
    buf[1] = m_port;
    write(buf, sizeof(buf));
  }

  /**
   * @override{HD44780::Adapter}
   * Write byte (8bit) to display.
   * @param[in] data (8b) to write.
   */
  virtual void write8b(uint8_t data)
  {
    uint8_t buf[4];
    m_port.data = (data >> 4);
    m_port.en = 1;
    buf[0] = m_port;
    m_port.en = 0;
    buf[1] = m_port;
    m_port.data = data;
    m_port.en = 1;
    buf[2] = m_port;
    m_port.en = 0;
    buf[3] = m_port;
    write(buf, sizeof(buf));
  }

  /**
   * @override{HD44780::Adapter}
   * Write character buffer to display.
   * @param[in] buf pointer to buffer.
   * @param[in] size number of bytes in buffer.
   */
  virtual void write8n(const void* buf, size_t size)
  {
    const uint8_t* bp = (const uint8_t*) buf;
    while (size != 0) {
      uint8_t tmp[32];
      uint8_t n = (size > sizeof(tmp) / 4 ? sizeof(tmp) / 4 : size);
      size -= n;
      uint8_t m = n * 4;
      for (uint8_t i = 0; i < m;) {
	uint8_t data = *bp++;
	m_port.data = (data >> 4);
	m_port.en = 1;
	tmp[i++] = m_port;
	m_port.en = 0;
	tmp[i++] = m_port;
	m_port.data = data;
	m_port.en = 1;
	tmp[i++] = m_port;
	m_port.en = 0;
	tmp[i++] = m_port;
      }
      write(tmp, m);
    }
  }

  /**
   * @override{HD44780::Adapter}
   * Set instruction/data mode; zero for instruction,
   * non-zero for data mode.
   * @param[in] flag.
   */
  virtual void set_mode(uint8_t flag)
  {
    m_port.rs = flag;
  }

  /**
   * @override{HD44780::Adapter}
   * Set backlight on/off.
   * @param[in] flag.
   */
  virtual void set_backlight(uint8_t flag)
  {
    m_port.bt = !flag;
    write(m_port.as_uint8);
  }

protected:
  /** Expander port bit fields; little endian */
  union port_t {
    uint8_t as_uint8;		//!< Unsigned byte access.
    struct {
      uint8_t data:4;		//!< Data port (P0..P3).
      uint8_t en:1;		//!< Enable pulse (P4).
      uint8_t rw:1;		//!< Read/Write (P5).
      uint8_t rs:1;		//!< Command/Data select (P6).
      uint8_t bt:1;		//!< Back-light (P7).
    };
    operator uint8_t()
    {
      return (as_uint8);
    }
    port_t()
    {
      as_uint8 = 0;
    }
  };
  port_t m_port;		//!< Port setting.
};
};

#endif
