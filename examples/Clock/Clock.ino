#include "GPIO.h"

#include "RTC.h"
#include "Driver/DS1302.h"
#include "Driver/DS1307.h"

#include "TWI.h"
#include "Hardware/TWI.h"
#include "Software/TWI.h"

#include "LCD.h"
#include "Driver/MAX72XX.h"
#include "Driver/PCD8544.h"
#include "Driver/HD44780.h"
#include "Adapter/Debug.h"
#include "Adapter/PP7W.h"
#include "Adapter/SR3W.h"
#include "Adapter/SR4W.h"
#include "Adapter/MJKDZ.h"
#include "Adapter/GY_IICLCD.h"
#include "Adapter/DFRobot_IIC.h"
#include "Shield/LCD4884.h"
#include "Shield/LCD_Keypad.h"

// Configure: HD44780 Adapter; Debug, PP7W, SR3W or TWI PCF8574
// LCD::Debug io;
// LCD::PP7W<BOARD::D4, BOARD::D5, BOARD::D6, BOARD::D7, BOARD::D8, BOARD::D9, BOARD::D10> io;
// LCD::SR3W<BOARD::D7, BOARD::D6, BOARD::D5> io;
// LCD::SR4W<BOARD::D7, BOARD::D6, BOARD::D5, BOARD::D4> io;
// Software::TWI<BOARD::D8, BOARD::D9> twi;
Software::TWI<BOARD::D18, BOARD::D19> twi;
// Hardware::TWI twi(100000UL);
// Hardware::TWI twi(400000UL);
// LCD::MJKDZ io(twi);
LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);

// Configure: LCD; HD44780, MAX72XX, or PCD8544, LCD4884 or LCD_Keypad
HD44780 lcd(io);
// MAX72XX<BOARD::D10, BOARD::D11, BOARD::D13> lcd;
// PCD8544<BAORD::D5, BOARD::D4, BOARD::D3, BOARD::D2> lcd;
// LCD4884 lcd;
// LCD_Keypad lcd;

// Configure: RTC; DS1302 or DS1307
// DS1302<BOARD::D11, BOARD::D12, BOARD::D13> rtc;
DS1307 rtc(twi);

void setup()
{
  // Initiate display
  lcd.begin();

  // Set the real-time clock
  struct tm now(SUNDAY, 2017, DECEMBER, 31, 23, 59, 00);
  rtc.set_time(now);
}

void loop()
{
  // Get current time
  struct tm now;
  rtc.get_time(now);

  // Convert to iso datetime string
  char buf[32];
  isotime_r(&now, buf);

  // Split datetime string and replace colon
  buf[10] = 0;
  if (lcd.HEIGHT == 1) {
    static int n = 1;
    lcd.cursor_home();
    if (n == 4) {
      lcd.print(buf + 2);
      n = 0;
    }
    else {
      lcd.print(buf + 11);
      n += 1;
    }
  }
  else {
    lcd.cursor_home();
    lcd.print(buf);
    lcd.cursor_set(0, 1);
    lcd.print(buf + 11);
  }
  delay(1000);
  lcd.display_clear();
}
