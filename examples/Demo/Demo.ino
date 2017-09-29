#include "GPIO.h"
#include "RTC.h"
#include "TWI.h"
#include "Hardware/TWI.h"
#include "Software/TWI.h"
#include "LCD.h"
#include "Driver/MAX72XX.h"
#include "Driver/HD44780.h"
#include "Adapter/Debug.h"
#include "Adapter/Port4b.h"
#include "Adapter/SR3W.h"
#include "Adapter/SR4W.h"
#include "Adapter/MJKDZ.h"
#include "Adapter/GY_IICLCD.h"
#include "Adapter/DFRobot_IIC.h"

// Configure: HD44780 Adapter; Debug, Port4b, SR3W or TWI PCF8574
// LCD::Debug io;
// LCD::Port4b<> io;
// LCD::SR3W<> io;
LCD::SR4W<> io;
// Hardware::TWI twi(400000UL);
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// LCD::MJKDZ io(twi);
// LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);

// MAX72XX<BOARD::D10, BOARD::D11, BOARD::D13> lcd;
HD44780 lcd(io);
RTC rtc;

void setup()
{
  Serial.begin(57600);
  while (!Serial);

  // Set the real-time clock
  struct tm now(WEDNESDAY, 2017, SEPTEMBER, 27, 22, 57, 00);
  rtc.set_time(now);

  // Initiate the lcd display
  lcd.begin();

  // Print some different data types
  if (lcd.WIDTH > 8)
    lcd.print(F("Hello world"));
  else
    lcd.print(F("HELLO"));
  delay(2000);
  lcd.display_clear();

  lcd.print(-10);
  lcd.print(F(" C"));
  if (lcd.HEIGHT > 1) {
    lcd.set_cursor(0, 1);
    lcd.print(42.5);
    lcd.print(F(" RH"));
  }
  delay(2000);
  lcd.display_clear();

  lcd.print(3.14159265359, 7);
  delay(2000);
  lcd.display_clear();
}

void loop()
{
  if (!rtc.tick()) return;

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
    lcd.set_cursor(0, 0);
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
    lcd.set_cursor(0, 0);
    lcd.print(buf);
    lcd.set_cursor(0, 1);
    lcd.print(buf + 11);
  }
}
