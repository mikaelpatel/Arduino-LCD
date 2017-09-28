#include "GPIO.h"
#include "RTC.h"
#include "LCD.h"

// Configure: Debug, MAX72XX or HD44780
// #include "Adapter/Debug.h"
// #include "Driver/MAX72XX.h"
#include "Driver/HD44780.h"

// #include "Adapter/Port4b.h"
#include "TWI.h"
#include "Hardware/TWI.h"
#include "Software/TWI.h"
#include "Adapter/MJKDZ.h"
#include "Adapter/GY_IICLCD.h"
#include "Adapter/DFRobot_IIC.h"

// Configure: HD44780 Adapter; Port4b or PCF8574
// LCD::Debug io;
// LCD::Port4b<> io;
Hardware::TWI twi(400000UL);
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// LCD::MJKDZ io(twi);
// LCD::GY_IICLCD io(twi);

// MAX72XX<BOARD::D10, BOARD::D11, BOARD::D13> led;
LCD::DFRobot_IIC io(twi);
HD44780 led(io);

RTC rtc;

void setup()
{
  Serial.begin(57600);
  while (!Serial);

  // Set the real-time clock
  struct tm now(WEDNESDAY, 2017, SEPTEMBER, 27, 22, 57, 00);
  rtc.set_time(now);

  // Initiate the led display
  led.begin();

  // Print some different data types
  if (led.WIDTH > 8)
    led.print(F("Hello world"));
  else
    led.print(F("HELLO"));
  delay(2000);
  led.display_clear();

  led.print(-10);
  led.print(F(" C"));
  if (led.HEIGHT > 1) {
    led.set_cursor(0, 1);
    led.print(42.5);
    led.print(F(" RH"));
  }
  delay(2000);
  led.display_clear();

  led.print(3.14159265359, 7);
  delay(2000);
  led.display_clear();
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
  if (led.HEIGHT == 1) {
    static int n = 1;
    led.set_cursor(0, 0);
    if (n == 4) {
      led.print(buf + 2);
      n = 0;
    }
    else {
      led.print(buf + 11);
      n += 1;
    }
  }
  else {
    led.set_cursor(0, 0);
    led.print(buf);
    led.set_cursor(0, 1);
    led.print(buf + 11);
  }
}
