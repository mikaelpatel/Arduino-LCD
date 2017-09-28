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
HD44780 led(io);

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
    led.display_clear();			\
  } while (0)

void setup()
{
  Serial.begin(57600);
  while (!Serial);
}

void loop()
{
  MEASURE(led.begin());
  MEASURE(led.cursor_blink_on(); led.print(F("$ benchmark")); delay(1000));
  MEASURE(led.backlight_off());
  MEASURE(led.backlight_on());
  MEASURE(led.display_off());
  MEASURE(led.display_on());
  MEASURE(led.display_clear());
  MEASURE(led.cursor_home());
  MEASURE(led.cursor_underline_on());
  MEASURE(led.cursor_underline_off());
  MEASURE(led.cursor_blink_off());
  MEASURE(led.set_cursor(0,0));
  MEASURE(led.line_clear());
  MEASURE(led.write('L'));
  MEASURE(led.print('\a'));
  MEASURE(led.print('\b'));
  MEASURE(led.print('\f'));
  MEASURE(led.print('\n'));
  MEASURE(led.print('\r'));
  MEASURE(led.print('\t'));
  MEASURE(led.print("0"));
  MEASURE(led.print("10"));
  MEASURE(led.print("100"));
  MEASURE(led.print("1000"));
  MEASURE(led.print("10000"));
  MEASURE(led.print("Hello World"));
  MEASURE(led.print(F("Hello World")));
  MEASURE(led.print(F("Hello\tWorld")));
  MEASURE(led.print(F("Hello\nWorld")));
  MEASURE(led.print(F("\nHello\tWorld")));
  MEASURE(led.print(0));
  MEASURE(led.print(1));
  MEASURE(led.print(10));
  MEASURE(led.print(100));
  MEASURE(led.print(INT16_MIN));
  MEASURE(led.print(INT16_MAX));
  MEASURE(led.print(INT32_MIN));
  MEASURE(led.print(INT32_MAX));
  MEASURE(led.print(1234.56789, 5));
  MEASURE(led.print(-1234.56789, 5));
  MEASURE(led.print(0x8000, BIN));
  MEASURE(led.print(0x8000, DEC));
  MEASURE(led.print(0x8000, HEX));
  MEASURE(led.end());
  Serial.println();
}

/*
--------------------------------------------------------------------------------
LCD::Port4b<> io;
--------------------------------------------------------------------------------
led.begin(): 54800
led.cursor_blink_on(); led.print(F("$ benchmark")); delay(1000): 1000536
led.backlight_off(): 8
led.backlight_on(): 4
led.display_off(): 40
led.display_on(): 40
led.display_clear(): 1652
led.cursor_home(): 1652
led.cursor_underline_on(): 40
led.cursor_underline_off(): 40
led.cursor_blink_off(): 36
led.set_cursor(0,0): 44
led.line_clear(): 688
led.write('L'): 48
led.print('\a'): 400032
led.print('\b'): 48
led.print('\f'): 1656
led.print('\n'): 784
led.print('\r'): 48
led.print('\t'): 56
led.print("0"): 52
led.print("10"): 96
led.print("100"): 136
led.print("1000"): 184
led.print("10000"): 236
led.print("Hello World"): 488
led.print(F("Hello World")): 484
led.print(F("Hello\tWorld")): 500
led.print(F("Hello\nWorld")): 1224
led.print(F("\nHello\tWorld")): 1276
led.print(0): 96
led.print(1): 104
led.print(10): 180
led.print(100): 260
led.print(INT16_MIN): 472
led.print(INT16_MAX): 428
led.print(INT32_MIN): 880
led.print(INT32_MAX): 840
led.print(1234.56789, 5): 1028
led.print(-1234.56789, 5): 1068
led.print(0x8000, BIN): 1312
led.print(0x8000, DEC): 428
led.print(0x8000, HEX): 340
led.end(): 44
--------------------------------------------------------------------------------
Hardware::TWI twi(100000UL);
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
led.begin(): 58896
led.cursor_blink_on(); led.print(F("$ benchmark")); delay(1000): 1006236
led.backlight_off(): 232
led.backlight_on(): 228
led.display_off(): 516
led.display_on(): 516
led.display_clear(): 2124
led.cursor_home(): 2124
led.cursor_underline_on(): 516
led.cursor_underline_off(): 516
led.cursor_blink_off(): 516
led.set_cursor(0,0): 520
led.line_clear(): 8296
led.write('L'): 520
led.print('\a'): 400468
led.print('\b'): 520
led.print('\f'): 2128
led.print('\n'): 9324
led.print('\r'): 520
led.print('\t'): 528
led.print("0"): 528
led.print("10"): 1052
led.print("100"): 1564
led.print("1000"): 2088
led.print("10000"): 2600
led.print("Hello World"): 5724
led.print(F("Hello World")): 5704
led.print(F("Hello\tWorld")): 5708
led.print(F("Hello\nWorld")): 14508
led.print(F("\nHello\tWorld")): 15044
led.print(0): 568
led.print(1): 568
led.print(10): 1132
led.print(100): 1680
led.print(INT16_MIN): 3312
led.print(INT16_MAX): 2796
led.print(INT32_MIN): 6088
led.print(INT32_MAX): 5588
led.print(1234.56789, 5): 5780
led.print(-1234.56789, 5): 6292
led.print(0x8000, BIN): 8892
led.print(0x8000, DEC): 2792
led.print(0x8000, HEX): 2240
led.end(): 516
--------------------------------------------------------------------------------
Hardware::TWI twi(400000UL);
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
led.begin(): 56056
led.cursor_blink_on(); led.print(F("$ benchmark")); delay(1000): 1002108
led.backlight_off(): 84
led.backlight_on(): 88
led.display_off(): 172
led.display_on(): 172
led.display_clear(): 1776
led.cursor_home(): 1776
led.cursor_underline_on(): 172
led.cursor_underline_off(): 172
led.cursor_blink_off(): 172
led.set_cursor(0,0): 176
led.line_clear(): 2796
led.write('L'): 176
led.print('\a'): 400192
led.print('\b'): 180
led.print('\f'): 1784
led.print('\n'): 3152
led.print('\r'): 176
led.print('\t'): 184
led.print("0"): 184
led.print("10"): 360
led.print("100"): 532
led.print("1000"): 704
led.print("10000"): 876
led.print("Hello World"): 1932
led.print(F("Hello World")): 1920
led.print(F("Hello\tWorld")): 1936
led.print(F("Hello\nWorld")): 4892
led.print(F("\nHello\tWorld")): 5076
led.print(0): 228
led.print(1): 224
led.print(10): 440
led.print(100): 652
led.print(INT16_MIN): 1256
led.print(INT16_MAX): 1076
led.print(INT32_MIN): 2328
led.print(INT32_MAX): 2152
led.print(1234.56789, 5): 2344
led.print(-1234.56789, 5): 2508
led.print(0x8000, BIN): 3408
led.print(0x8000, DEC): 1080
led.print(0x8000, HEX): 860
led.end(): 172
--------------------------------------------------------------------------------
*/
