#include "GPIO.h"
#include "LCD.h"
#include "Driver/HD44780.h"
#include "Adapter/Port4b.h"
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
LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);
HD44780 lcd(io);

uint32_t start, stop;

#define MEASURE(expr)				\
  do {						\
    Serial.flush();				\
    start = micros();				\
    expr;					\
    stop = micros();				\
    Serial.print(F(#expr ": "));		\
    Serial.println(stop - start);		\
    delay(500);					\
    lcd.display_clear();			\
  } while (0)

void setup()
{
  Serial.begin(57600);
  while (!Serial);
}

void loop()
{
  MEASURE(lcd.begin());
  MEASURE(lcd.cursor_blink_on(); lcd.print(F("$ benchmark")); delay(1000));
  MEASURE(lcd.backlight_off());
  MEASURE(lcd.backlight_on());
  MEASURE(lcd.display_off());
  MEASURE(lcd.display_on());
  MEASURE(lcd.display_clear());
  MEASURE(lcd.cursor_home());
  MEASURE(lcd.cursor_underline_on());
  MEASURE(lcd.cursor_underline_off());
  MEASURE(lcd.cursor_blink_off());
  MEASURE(lcd.set_cursor(0,0));
  MEASURE(lcd.line_clear());
  MEASURE(lcd.write('L'));
  MEASURE(lcd.print('\a'));
  MEASURE(lcd.print('\b'));
  MEASURE(lcd.print('\f'));
  MEASURE(lcd.print('\n'));
  MEASURE(lcd.print('\r'));
  MEASURE(lcd.print('\t'));
  MEASURE(lcd.print("0"));
  MEASURE(lcd.print("10"));
  MEASURE(lcd.print("100"));
  MEASURE(lcd.print("1000"));
  MEASURE(lcd.print("10000"));
  MEASURE(lcd.print("Hello World"));
  MEASURE(lcd.print(F("Hello World")));
  MEASURE(lcd.print(F("Hello\tWorld")));
  MEASURE(lcd.print(F("Hello\nWorld")));
  MEASURE(lcd.print(F("\nHello\tWorld")));
  MEASURE(lcd.print(0));
  MEASURE(lcd.print(1));
  MEASURE(lcd.print(10));
  MEASURE(lcd.print(100));
  MEASURE(lcd.print(INT16_MIN));
  MEASURE(lcd.print(INT16_MAX));
  MEASURE(lcd.print(INT32_MIN));
  MEASURE(lcd.print(INT32_MAX));
  MEASURE(lcd.print(1234.56789, 5));
  MEASURE(lcd.print(-1234.56789, 5));
  MEASURE(lcd.print(0x8000, BIN));
  MEASURE(lcd.print(0x8000, DEC));
  MEASURE(lcd.print(0x8000, HEX));
  MEASURE(lcd.end());
  Serial.println();
}

/*
--------------------------------------------------------------------------------
LCD::Port4b<> io;
--------------------------------------------------------------------------------
lcd.begin(): 54800
lcd.cursor_blink_on(); lcd.print(F("$ benchmark")); delay(1000): 1000536
lcd.backlight_off(): 8
lcd.backlight_on(): 4
lcd.display_off(): 40
lcd.display_on(): 40
lcd.display_clear(): 1652
lcd.cursor_home(): 1652
lcd.cursor_underline_on(): 40
lcd.cursor_underline_off(): 40
lcd.cursor_blink_off(): 36
lcd.set_cursor(0,0): 44
lcd.line_clear(): 688
lcd.write('L'): 48
lcd.print('\a'): 400032
lcd.print('\b'): 48
lcd.print('\f'): 1656
lcd.print('\n'): 784
lcd.print('\r'): 48
lcd.print('\t'): 56
lcd.print("0"): 52
lcd.print("10"): 96
lcd.print("100"): 136
lcd.print("1000"): 184
lcd.print("10000"): 236
lcd.print("Hello World"): 488
lcd.print(F("Hello World")): 484
lcd.print(F("Hello\tWorld")): 500
lcd.print(F("Hello\nWorld")): 1224
lcd.print(F("\nHello\tWorld")): 1276
lcd.print(0): 96
lcd.print(1): 104
lcd.print(10): 180
lcd.print(100): 260
lcd.print(INT16_MIN): 472
lcd.print(INT16_MAX): 428
lcd.print(INT32_MIN): 880
lcd.print(INT32_MAX): 840
lcd.print(1234.56789, 5): 1028
lcd.print(-1234.56789, 5): 1068
lcd.print(0x8000, BIN): 1312
lcd.print(0x8000, DEC): 428
lcd.print(0x8000, HEX): 340
lcd.end(): 44
--------------------------------------------------------------------------------
Hardware::TWI twi(100000UL);
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
lcd.begin(): 58896
lcd.cursor_blink_on(); lcd.print(F("$ benchmark")); delay(1000): 1006236
lcd.backlight_off(): 232
lcd.backlight_on(): 228
lcd.display_off(): 516
lcd.display_on(): 516
lcd.display_clear(): 2124
lcd.cursor_home(): 2124
lcd.cursor_underline_on(): 516
lcd.cursor_underline_off(): 516
lcd.cursor_blink_off(): 516
lcd.set_cursor(0,0): 520
lcd.line_clear(): 8296
lcd.write('L'): 520
lcd.print('\a'): 400468
lcd.print('\b'): 520
lcd.print('\f'): 2128
lcd.print('\n'): 9324
lcd.print('\r'): 520
lcd.print('\t'): 528
lcd.print("0"): 528
lcd.print("10"): 1052
lcd.print("100"): 1564
lcd.print("1000"): 2088
lcd.print("10000"): 2600
lcd.print("Hello World"): 5724
lcd.print(F("Hello World")): 5704
lcd.print(F("Hello\tWorld")): 5708
lcd.print(F("Hello\nWorld")): 14508
lcd.print(F("\nHello\tWorld")): 15044
lcd.print(0): 568
lcd.print(1): 568
lcd.print(10): 1132
lcd.print(100): 1680
lcd.print(INT16_MIN): 3312
lcd.print(INT16_MAX): 2796
lcd.print(INT32_MIN): 6088
lcd.print(INT32_MAX): 5588
lcd.print(1234.56789, 5): 5780
lcd.print(-1234.56789, 5): 6292
lcd.print(0x8000, BIN): 8892
lcd.print(0x8000, DEC): 2792
lcd.print(0x8000, HEX): 2240
lcd.end(): 516
--------------------------------------------------------------------------------
Hardware::TWI twi(400000UL);
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
lcd.begin(): 56056
lcd.cursor_blink_on(); lcd.print(F("$ benchmark")); delay(1000): 1002108
lcd.backlight_off(): 84
lcd.backlight_on(): 88
lcd.display_off(): 172
lcd.display_on(): 172
lcd.display_clear(): 1776
lcd.cursor_home(): 1776
lcd.cursor_underline_on(): 172
lcd.cursor_underline_off(): 172
lcd.cursor_blink_off(): 172
lcd.set_cursor(0,0): 176
lcd.line_clear(): 2796
lcd.write('L'): 176
lcd.print('\a'): 400192
lcd.print('\b'): 180
lcd.print('\f'): 1784
lcd.print('\n'): 3152
lcd.print('\r'): 176
lcd.print('\t'): 184
lcd.print("0"): 184
lcd.print("10"): 360
lcd.print("100"): 532
lcd.print("1000"): 704
lcd.print("10000"): 876
lcd.print("Hello World"): 1932
lcd.print(F("Hello World")): 1920
lcd.print(F("Hello\tWorld")): 1936
lcd.print(F("Hello\nWorld")): 4892
lcd.print(F("\nHello\tWorld")): 5076
lcd.print(0): 228
lcd.print(1): 224
lcd.print(10): 440
lcd.print(100): 652
lcd.print(INT16_MIN): 1256
lcd.print(INT16_MAX): 1076
lcd.print(INT32_MIN): 2328
lcd.print(INT32_MAX): 2152
lcd.print(1234.56789, 5): 2344
lcd.print(-1234.56789, 5): 2508
lcd.print(0x8000, BIN): 3408
lcd.print(0x8000, DEC): 1080
lcd.print(0x8000, HEX): 860
lcd.end(): 172
--------------------------------------------------------------------------------
*/
