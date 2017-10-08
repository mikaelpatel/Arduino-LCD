#include "GPIO.h"
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
// LCD::PP7W<BOARD::D4, BOARD::D5, BOARD::D6, BOARD::D7, BOARD::D8, BOARD::D9, BOARD::D10> io;
// LCD::SR3W<BOARD::D7, BOARD::D6, BOARD::D5> io;
// LCD::SR4W<BOARD::D7, BOARD::D6, BOARD::D5, BOARD::D4> io;
// Software::TWI<BOARD::D8, BOARD::D9> twi;
// Software::TWI<BOARD::D18, BOARD::D19> twi;
Hardware::TWI twi(100000UL);
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

void setup()
{
  // Initiate the lcd display
  lcd.begin();
}

void loop()
{
  // Print a classical greeting
  lcd.print(lcd.WIDTH > 8 ? F("Hello World") : F("HELLO"));
  delay(2000);
  lcd.display_clear();

  // Print a full line
  for (int i = 0; i < lcd.WIDTH; i++)
    lcd.print((char) ('A' + i));
  delay(2000);

  // Print backspace and delay show the effect
  for (int i = 0; i < lcd.WIDTH; i++) {
    lcd.print('\b');
    delay(200);
  }
  lcd.display_clear();

  // Print integer and floating point number
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

  // Print integer in binary
  uint16_t upper = (1 << lcd.WIDTH) - 1;
  uint16_t lower = (1 << (lcd.WIDTH - 1));
  lower = (upper - lower) < 1000 ? lower : upper - 1000;
  for (uint16_t i = upper; i > lower; i--) {
    lcd.cursor_home();
    lcd.print(i, BIN);
    delay(10);
  }
  lcd.display_clear();

  // Print floating pointer number
  float value = 3.14159265359;
  for (int i = 0; i < 1000; i++) {
    lcd.cursor_home();
    lcd.print(value, 7);
    value += 0.000001;
    delay(10);
  }
  lcd.display_clear();
}
