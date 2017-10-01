#include "GPIO.h"
#include "RTC.h"
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

// Configure: HD44780 Adapter; Debug, PP7W, SR3W or TWI PCF8574
// LCD::Debug io;
LCD::PP7W<> io;
// LCD::SR3W<> io;
// LCD::SR4W<> io;
// Hardware::TWI twi(400000UL);
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// LCD::MJKDZ io(twi);
// LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);
HD44780 lcd(io);

// MAX72XX<BOARD::D10, BOARD::D11, BOARD::D13> lcd;
// PCD8544<> lcd;
RTC rtc;

void setup()
{
  // Set the real-time clock
  struct tm now(WEDNESDAY, 2017, SEPTEMBER, 27, 22, 57, 00);
  rtc.set_time(now);

  // Initiate the lcd display
  lcd.begin();

  // Print some different data types
  lcd.print(lcd.WIDTH > 8 ? F("Hello World") : F("HELLO"));
  delay(2000);
  lcd.display_clear();

  for (int i = 0; i < lcd.WIDTH; i++)
    lcd.print((char) ('A' + i));
  delay(2000);
  for (int i = 0; i < lcd.WIDTH; i++) {
    lcd.print('\b');
    delay(200);
  }
  lcd.display_clear();

  lcd.print(-10);
  lcd.print(F(" C"));
  if (lcd.HEIGHT > 1) {
    lcd.cursor_set(0, 1);
    lcd.print(42.5);
    lcd.print(F(" %RH"));
  }
  else {
    delay(2000);
    lcd.display_clear();
    lcd.print(42.5);
    lcd.print(F(" H"));
  }
  delay(2000);
  lcd.display_clear();

  uint16_t upper = (1 << lcd.WIDTH) - 1;
  uint16_t lower = (1 << (lcd.WIDTH - 1));
  lower = (upper - lower) < 1000 ? lower : upper - 1000;
  for (uint16_t i = upper; i > lower; i--) {
    lcd.cursor_home();
    lcd.print(i, BIN);
    delay(10);
  }
  delay(2000);
  lcd.display_clear();

  float value = 3.14159265359;
  for (int i = 0; i < 1000; i++) {
    lcd.cursor_home();
    lcd.print(value, 7);
    value += 0.000001;
    delay(10);
  }
  delay(2000);
  lcd.display_clear();
}

void loop()
{
  if (!rtc.tick()) return;
  lcd.display_clear();

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
}
