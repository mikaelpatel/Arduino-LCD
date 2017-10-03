#include "GPIO.h"

#include "OWI.h"
#include "Software/OWI.h"
#include "Driver/DS18B20.h"

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
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// Hardware::TWI twi(100000UL);
Hardware::TWI twi(400000UL);
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// LCD::MJKDZ io(twi);
LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);

// Configure: LCD; HD44780, MAX72XX, or PCD8544, LCD4884 or LCD_Keypad
HD44780 lcd(io);
// MAX72XX<BOARD::D10, BOARD::D11, BOARD::D13> lcd;
// PCD8544<BAORD::D5, BOARD::D4, BOARD::D3, BOARD::D2> lcd;
// LCD4884 lcd;
// LCD_Keypad lcd;

Software::OWI<BOARD::D2> owi;
DS18B20 sensor(owi);

void setup()
{
  lcd.begin();
}

void loop()
{
  uint8_t rom[owi.ROM_MAX] = { 0 };
  int8_t last = owi.FIRST;
  int i = 0;

  // Broadcast a convert request to all thermometer sensors
  if (!sensor.convert_request(true)) return;

  // Display sensor identity and temperture
  do {
    delay(2000);
    last = owi.search_rom(sensor.FAMILY_CODE, rom, last);
    if (last == owi.ERROR) break;
    bool res = sensor.read_scratchpad(false);
    lcd.display_clear();

    // Display rom identity or sensor sequence number
    if (lcd.HEIGHT > 1 && lcd.WIDTH > 15) {
      for (size_t i = 0; i < sizeof(rom); i++) {
	if (rom[i] < 0x10) lcd.print('0');
	lcd.print(rom[i], HEX);
      }
    }
    else {
      lcd.print(i++);
      lcd.print(' ');
    }

    // Display temperature if correctly read otherwise error
    if (res) {
      lcd.print(sensor.temperature());
    }
    else {
      lcd.print(F("???"));
    }
    lcd.print(F(" C"));
  } while (last != owi.LAST);
}
