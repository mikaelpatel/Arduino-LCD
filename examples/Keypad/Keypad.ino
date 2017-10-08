#include "GPIO.h"
#include "LCD.h"
#include "Shield/LCD_Keypad.h"
#include "Shield/LCD4884.h"

// Configure: LCD4884 or LCD_Keypad Shield
LCD4884 lcd;
// LCD_Keypad lcd;

void setup()
{
  lcd.begin();
  lcd.print(F("Keypad: started"));
}

void loop()
{
  // Check that a key pad change
  if (!lcd.ischanged()) return;

  // Print the key name
  lcd.display_clear();
  switch (lcd.key()) {
  case lcd.NO_KEY:
    lcd.print(F("NO_KEY"));
    break;
  case lcd.SELECT_KEY:
    lcd.print(F("SELECT_KEY"));
    break;
  case lcd.LEFT_KEY:
    lcd.print(F("LEFT_KEY"));
    break;
  case lcd.DOWN_KEY:
    lcd.print(F("DOWN_KEY"));
    break;
  case lcd.UP_KEY:
    lcd.print(F("UP_KEY"));
    break;
  case lcd.RIGHT_KEY:
    lcd.print(F("RIGHT_KEY"));
    break;
  default:
    lcd.print(lcd.key());
  }

  // Print key number and timestamp when pressed
  lcd.cursor_set(0, 1);
  lcd.print(lcd.key());
  lcd.print(':');
  lcd.print(lcd.timestamp());
}
