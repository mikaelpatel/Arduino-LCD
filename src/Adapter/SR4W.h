/**
 * @file LCD/Adapter/SR4W.h
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
#ifndef LCD_ADAPTER_SR4W_H
#define LCD_ADAPTER_SR4W_H

#include "LCD.h"
#include "Driver/HD44780.h"
#include "GPIO.h"

/**
 * HD44780 (LCD-II) Dot Matix Liquid Crystal Display Controller/Driver
 * Shift Register 4-Wire/8-bit Port, 74HC595 (SR[pin]), with digital
 * output pins.
 * @param[in] SDA_PIN serial data/rs pin.
 * @param[in] SCL_PIN serial clock pin.
 * @param[in] EN_PIN enable pulse.
 * @param[in] BT_PIN backlight control.
 *
 * @section Circuit
 * @code
 *                         74HC595    (VCC)
 *                       +----U----+    |
 * (LCD D1)------------1-|Q1    VCC|-16-+
 * (LCD D2)------------2-|Q2     Q0|-15--------(LCD D0)
 * (LCD D3)------------3-|Q3    /OE|-13-----------(GND)
 * (LCD D4)------------4-|Q4    SER|-14-------(SDA_PIN)
 * (LCD D5)------------5-|Q5   RCLK|-12--------(EN_PIN)
 * (LCD D6)------------6-|Q6   SCLK|-11--------SCL_PIN)
 * (LCD D7)------------7-|Q7    /MR|-10-----------(VCC)
 *                   +-8-|GND   Q6'|-9
 *                   |   +---------+
 *                   |      0.1uF
 *                 (GND)-----||----(VCC)
 *
 * (LCD RS)-----------------------------------(SDA_PIN)
 * (LCD EN)------------------------------------(EN_PIN)
 * (LCD BT)------------------------------------(BT_PIN)
 * (LCD RW)---------------------------------------(GND)
 * (LCD K)----------------------------------------(GND)
 * (LCD A)-----------------[330]------------------(VCC)
 * @endcode
 *
 * Alternative circuit with 74HC164.
 * @code
 *                         74HC164    (VCC)
 *                       +----U----+    |
 * (SDA_PIN)---------+-1-|DSA   VCC|-14-+
 *                   +-2-|DSB    Q7|-13--------(LCD D7)
 * (LCD D4)------------3-|Q0     Q6|-12--------(LCD D6)
 * (LCD D5)------------4-|Q1     Q5|-11--------(LCD D5)
 * (LCD D6)------------5-|Q2     Q4|-10--------(LCD D4)
 * (LCD D7)------------6-|Q3    /MR|--9-----------(VCC)
 *                   +-7-|GND    CP|--8-------(SCL_PIN)
 *                   |   +---------+
 *                   |      0.1uF
 *                 (GND)-----||----(VCC)
 *
 * (LCD RS)-----------------------------------(SDA_PIN)
 * (LCD EN)------------------------------------(EN_PIN)
 * (LCD BT)------------------------------------(BT_PIN)
 * (LCD RW)---------------------------------------(GND)
 * (LCD K)----------------------------------------(GND)
 * (LCD A)-----------------[330]------------------(VCC)
 * @endcode
 *
 * @section Acknowledgements
 * Inspired by AVR2LCD, a solution by Frank Henriquez. The original
 * 74HC164 based design is by Stefan Heinzmann and Marc Simons.
 * http://frank.bol.ucla.edu/avr2lcd.htm
 * http://web.archive.org/web/20100210142839/
 * http://home.iae.nl/users/pouweha/lcd/lcd_examp.shtml#_3
 */
namespace LCD {
template<BOARD::pin_t SDA_PIN,
	 BOARD::pin_t SCL_PIN,
	 BOARD::pin_t EN_PIN,
	 BOARD::pin_t BT_PIN>
class SR4W : public HD44780::Adapter {
public:
  /**
   * Construct HD44780 4-wire/8-bit serial port connected to given
   * data (rs), clock, enable and backlight control pins.
   */
  SR4W() :
    HD44780::Adapter(),
    m_rs(0)
  {
    m_sda.output();
    m_scl.output();
    m_en.output();
    m_bt.output();
  }

  /**
   * @override{HD44780::Adapter}
   * Initiate port for 8-bit serial mode.
   * @return true(1).
   */
  virtual bool setup()
  {
    return (true);
  }

  /**
   * @override{HD44780::Adapter}
   * Write LSB nibble to display using serial port.
   * @param[in] data (4b) to write.
   */
  virtual void write4b(uint8_t data)
  {
    write8b(data);
  }

  /**
   * @override{HD44780::Adapter}
   * Write byte (8bit) to display.
   * @param[in] data (8b) to write.
   */
  virtual void write8b(uint8_t data)
  {
    uint8_t mask = 0x80;
    do {
      m_sda = data & mask;
      m_scl.toggle();
      mask >>= 1;
      m_scl.toggle();
    } while (mask);
    m_sda = m_rs;
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
    m_rs = flag;
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
  static const uint16_t SHORT_EXEC_TIME = 30;

  GPIO<SDA_PIN> m_sda;		//!< Serial data output.
  GPIO<SCL_PIN> m_scl;		//!< Serial clock.
  GPIO<EN_PIN> m_en;		//!< Starts data read/write.
  GPIO<BT_PIN> m_bt;		//!< Backlight control.
  uint8_t m_rs;			//!< Command/Data select.
};

};
#endif
