#include "GPIO.h"
#include "TWI.h"
#include "Hardware/TWI.h"
#include "Software/TWI.h"
#include "LCD.h"
#include "Driver/HD44780.h"
#include "Adapter/Port4b.h"
#include "Adapter/SR3W.h"
#include "Adapter/SR4W.h"
#include "Adapter/MJKDZ.h"
#include "Adapter/GY_IICLCD.h"
#include "Adapter/DFRobot_IIC.h"

// Configure: HD44780 Adapter; Port4b, SR3W, SR4W or TWI PCF8574
// LCD::Debug io;
// LCD::Port4b<> io;
// LCD::SR3W<> io;
LCD::SR4W<> io;
// Hardware::TWI twi(400000UL);
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// LCD::MJKDZ io(twi);
// LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);
HD44780 lcd(io);

#define MEASURE(expr)				\
  do {						\
    Serial.flush();				\
    start = micros();				\
    expr;					\
    stop = micros();				\
    Serial.print(nr++);				\
    Serial.print(F(": " #expr ": "));		\
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
  uint32_t start, stop;
  uint16_t nr = 0;

  // Special functions
  MEASURE(lcd.begin());
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

  // Basic write
  MEASURE(lcd.write('L'));

  // Special characters
  MEASURE(lcd.print('\a'));
  MEASURE(lcd.print('\b'));
  MEASURE(lcd.print('\f'));
  MEASURE(lcd.print('\n'));
  MEASURE(lcd.print('\r'));
  MEASURE(lcd.print('\t'));

  // String print
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

  // Integer print
  MEASURE(lcd.print(0));
  MEASURE(lcd.print(1));
  MEASURE(lcd.print(10));
  MEASURE(lcd.print(100));
  MEASURE(lcd.print(INT16_MIN));
  MEASURE(lcd.print(INT16_MAX));
  MEASURE(lcd.print(INT32_MIN));
  MEASURE(lcd.print(INT32_MAX));

  // Float print
  MEASURE(lcd.print(1234.56789, 5));
  MEASURE(lcd.print(-1234.56789, 5));

  // Integer print in base
  MEASURE(lcd.print(0x8000, BIN));
  MEASURE(lcd.print(0x8000, DEC));
  MEASURE(lcd.print(0x8000, HEX));

  // That all in this benchmark
  MEASURE(lcd.end());
  Serial.println();
}

/*
--------------------------------------------------------------------------------
LCD::Port4b<> io; LCD Keypad Shield, 7 pin GPIO
--------------------------------------------------------------------------------
0: lcd.begin(): 54800
1: lcd.backlight_off(): 4
2: lcd.backlight_on(): 8
3: lcd.display_off(): 40
4: lcd.display_on(): 44
5: lcd.display_clear(): 1652
6: lcd.cursor_home(): 1652
7: lcd.cursor_underline_on(): 36
8: lcd.cursor_underline_off(): 40
9: lcd.cursor_blink_off(): 40
10: lcd.set_cursor(0,0): 44
11: lcd.line_clear(): 688
12: lcd.write('L'): 44
13: lcd.print('\a'): 400028
14: lcd.print('\b'): 48
15: lcd.print('\f'): 1656
16: lcd.print('\n'): 784
17: lcd.print('\r'): 48
18: lcd.print('\t'): 52
19: lcd.print("0"): 48
20: lcd.print("10"): 96
21: lcd.print("100"): 136
22: lcd.print("1000"): 180
23: lcd.print("10000"): 228
24: lcd.print("Hello World"): 488
25: lcd.print(F("Hello World")): 484
26: lcd.print(F("Hello\tWorld")): 496
27: lcd.print(F("Hello\nWorld")): 1224
28: lcd.print(F("\nHello\tWorld")): 1272
29: lcd.print(0): 96
30: lcd.print(1): 96
31: lcd.print(10): 180
32: lcd.print(100): 260
33: lcd.print(INT16_MIN): 472
34: lcd.print(INT16_MAX): 424
35: lcd.print(INT32_MIN): 884
36: lcd.print(INT32_MAX): 840
37: lcd.print(1234.56789, 5): 1028
38: lcd.print(-1234.56789, 5): 1072
39: lcd.print(0x8000, BIN): 1312
40: lcd.print(0x8000, DEC): 420
41: lcd.print(0x8000, HEX): 344
42: lcd.end(): 44
--------------------------------------------------------------------------------
LCD::SR3W<> io; Shift Register (74HC595 or 74HC164), 3 pin GPIO
--------------------------------------------------------------------------------
0: lcd.begin(): 54892
1: lcd.backlight_off(): 4
2: lcd.backlight_on(): 8
3: lcd.display_off(): 56
4: lcd.display_on(): 52
5: lcd.display_clear(): 1664
6: lcd.cursor_home(): 1660
7: lcd.cursor_underline_on(): 56
8: lcd.cursor_underline_off(): 56
9: lcd.cursor_blink_off(): 60
10: lcd.set_cursor(0,0): 56
11: lcd.line_clear(): 928
12: lcd.write('L'): 60
13: lcd.print('\a'): 400028
14: lcd.print('\b'): 60
15: lcd.print('\f'): 1668
16: lcd.print('\n'): 1056
17: lcd.print('\r'): 68
18: lcd.print('\t'): 68
19: lcd.print("0"): 68
20: lcd.print("10"): 132
21: lcd.print("100"): 184
22: lcd.print("1000"): 248
23: lcd.print("10000"): 300
24: lcd.print("Hello World"): 656
25: lcd.print(F("Hello World")): 648
26: lcd.print(F("Hello\tWorld")): 656
27: lcd.print(F("Hello\nWorld")): 1644
28: lcd.print(F("\nHello\tWorld")): 1708
29: lcd.print(0): 112
30: lcd.print(1): 108
31: lcd.print(10): 208
32: lcd.print(100): 304
33: lcd.print(INT16_MIN): 556
34: lcd.print(INT16_MAX): 496
35: lcd.print(INT32_MIN): 1048
36: lcd.print(INT32_MAX): 992
37: lcd.print(1234.56789, 5): 1176
38: lcd.print(-1234.56789, 5): 1232
39: lcd.print(0x8000, BIN): 1560
40: lcd.print(0x8000, DEC): 500
41: lcd.print(0x8000, HEX): 396
42: lcd.end(): 56
--------------------------------------------------------------------------------
LCD::SR4W<> io; Shift Register (74HC595 or 74HC164), 3/4 pin GPIO
--------------------------------------------------------------------------------
0: lcd.begin(): 49864
1: lcd.backlight_off(): 8
2: lcd.backlight_on(): 4
3: lcd.display_off(): 44
4: lcd.display_on(): 44
5: lcd.display_clear(): 1656
6: lcd.cursor_home(): 1656
7: lcd.cursor_underline_on(): 44
8: lcd.cursor_underline_off(): 44
9: lcd.cursor_blink_off(): 44
10: lcd.set_cursor(0,0): 48
11: lcd.line_clear(): 736
12: lcd.write('L'): 48
13: lcd.print('\a'): 400032
14: lcd.print('\b'): 52
15: lcd.print('\f'): 1656
16: lcd.print('\n'): 844
17: lcd.print('\r'): 52
18: lcd.print('\t'): 56
19: lcd.print("0"): 52
20: lcd.print("10"): 100
21: lcd.print("100"): 152
22: lcd.print("1000"): 196
23: lcd.print("10000"): 244
24: lcd.print("Hello World"): 528
25: lcd.print(F("Hello World")): 528
26: lcd.print(F("Hello\tWorld")): 528
27: lcd.print(F("Hello\nWorld")): 1316
28: lcd.print(F("\nHello\tWorld")): 1368
29: lcd.print(0): 100
30: lcd.print(1): 100
31: lcd.print(10): 184
32: lcd.print(100): 276
33: lcd.print(INT16_MIN): 492
34: lcd.print(INT16_MAX): 440
35: lcd.print(INT32_MIN): 920
36: lcd.print(INT32_MAX): 872
37: lcd.print(1234.56789, 5): 1056
38: lcd.print(-1234.56789, 5): 1108
39: lcd.print(0x8000, BIN): 1364
40: lcd.print(0x8000, DEC): 436
41: lcd.print(0x8000, HEX): 356
42: lcd.end(): 44
--------------------------------------------------------------------------------
GY IIC LCD, PCF8574, 2 wire TWI
Software::TWI<BOARD::D18,BOARD::D19> twi;
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
0: lcd.begin(): 58640
1: lcd.backlight_off(): 212
2: lcd.backlight_on(): 208
3: lcd.display_off(): 492
4: lcd.display_on(): 488
5: lcd.display_clear(): 2096
6: lcd.cursor_home(): 2096
7: lcd.cursor_underline_on(): 492
8: lcd.cursor_underline_off(): 484
9: lcd.cursor_blink_off(): 480
10: lcd.set_cursor(0,0): 496
11: lcd.line_clear(): 7860
12: lcd.write('L'): 492
13: lcd.print('\a'): 400444
14: lcd.print('\b'): 492
15: lcd.print('\f'): 2100
16: lcd.print('\n'): 8848
17: lcd.print('\r'): 500
18: lcd.print('\t'): 500
19: lcd.print("0"): 496
20: lcd.print("10"): 992
21: lcd.print("100"): 1480
22: lcd.print("1000"): 1976
23: lcd.print("10000"): 2464
24: lcd.print("Hello World"): 5424
25: lcd.print(F("Hello World")): 5416
26: lcd.print(F("Hello\tWorld")): 5428
27: lcd.print(F("Hello\nWorld")): 13776
28: lcd.print(F("\nHello\tWorld")): 14272
29: lcd.print(0): 544
30: lcd.print(1): 540
31: lcd.print(10): 1076
32: lcd.print(100): 1600
33: lcd.print(INT16_MIN): 3156
34: lcd.print(INT16_MAX): 2668
35: lcd.print(INT32_MIN): 5808
36: lcd.print(INT32_MAX): 5320
37: lcd.print(1234.56789, 5): 5504
38: lcd.print(-1234.56789, 5): 6000
39: lcd.print(0x8000, BIN): 8484
40: lcd.print(0x8000, DEC): 2668
41: lcd.print(0x8000, HEX): 2132
42: lcd.end(): 496
--------------------------------------------------------------------------------
GY IIC LCD, PCF8574, 2 wire TWI
Hardware::TWI twi(100000UL);
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
0: lcd.begin(): 58892
1: lcd.backlight_off(): 228
2: lcd.backlight_on(): 224
3: lcd.display_off(): 512
4: lcd.display_on(): 512
5: lcd.display_clear(): 2120
6: lcd.cursor_home(): 2120
7: lcd.cursor_underline_on(): 512
8: lcd.cursor_underline_off(): 512
9: lcd.cursor_blink_off(): 516
10: lcd.set_cursor(0,0): 516
11: lcd.line_clear(): 8292
12: lcd.write('L'): 520
13: lcd.print('\a'): 400468
14: lcd.print('\b'): 524
15: lcd.print('\f'): 2128
16: lcd.print('\n'): 9324
17: lcd.print('\r'): 524
18: lcd.print('\t'): 528
19: lcd.print("0"): 528
20: lcd.print("10"): 1048
21: lcd.print("100"): 1560
22: lcd.print("1000"): 2080
23: lcd.print("10000"): 2600
24: lcd.print("Hello World"): 5708
25: lcd.print(F("Hello World")): 5700
26: lcd.print(F("Hello\tWorld")): 5708
27: lcd.print(F("Hello\nWorld")): 14500
28: lcd.print(F("\nHello\tWorld")): 15048
29: lcd.print(0): 576
30: lcd.print(1): 572
31: lcd.print(10): 1128
32: lcd.print(100): 1680
33: lcd.print(INT16_MIN): 3312
34: lcd.print(INT16_MAX): 2800
35: lcd.print(INT32_MIN): 6104
36: lcd.print(INT32_MAX): 5576
37: lcd.print(1234.56789, 5): 5780
38: lcd.print(-1234.56789, 5): 6288
39: lcd.print(0x8000, BIN): 8896
40: lcd.print(0x8000, DEC): 2792
41: lcd.print(0x8000, HEX): 2244
42: lcd.end(): 512
--------------------------------------------------------------------------------
GY IIC LCD, PCF8574, 2 wire TWI
Hardware::TWI twi(400000UL);
LCD::GY_IICLCD io(twi);
--------------------------------------------------------------------------------
0: lcd.begin(): 56056
1: lcd.backlight_off(): 88
2: lcd.backlight_on(): 88
3: lcd.display_off(): 172
4: lcd.display_on(): 172
5: lcd.display_clear(): 1776
6: lcd.cursor_home(): 1784
7: lcd.cursor_underline_on(): 172
8: lcd.cursor_underline_off(): 172
9: lcd.cursor_blink_off(): 172
10: lcd.set_cursor(0,0): 172
11: lcd.line_clear(): 2796
12: lcd.write('L'): 176
13: lcd.print('\a'): 400184
14: lcd.print('\b'): 180
15: lcd.print('\f'): 1788
16: lcd.print('\n'): 3160
17: lcd.print('\r'): 180
18: lcd.print('\t'): 184
19: lcd.print("0"): 180
20: lcd.print("10"): 356
21: lcd.print("100"): 532
22: lcd.print("1000"): 708
23: lcd.print("10000"): 876
24: lcd.print("Hello World"): 1936
25: lcd.print(F("Hello World")): 1928
26: lcd.print(F("Hello\tWorld")): 1936
27: lcd.print(F("Hello\nWorld")): 4908
28: lcd.print(F("\nHello\tWorld")): 5072
29: lcd.print(0): 228
30: lcd.print(1): 228
31: lcd.print(10): 444
32: lcd.print(100): 656
33: lcd.print(INT16_MIN): 1256
34: lcd.print(INT16_MAX): 1076
35: lcd.print(INT32_MIN): 2324
36: lcd.print(INT32_MAX): 2144
37: lcd.print(1234.56789, 5): 2340
38: lcd.print(-1234.56789, 5): 2516
39: lcd.print(0x8000, BIN): 3404
40: lcd.print(0x8000, DEC): 1080
41: lcd.print(0x8000, HEX): 860
42: lcd.end(): 172
--------------------------------------------------------------------------------
*/
