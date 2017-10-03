#include "GPIO.h"
#include "TWI.h"
#include "Hardware/TWI.h"
#include "Software/TWI.h"
#include "LCD.h"
#include "Driver/PCD8544.h"
#include "Driver/HD44780.h"
#include "Adapter/PP7W.h"
#include "Adapter/SR3W.h"
#include "Adapter/SR4W.h"
#include "Adapter/MJKDZ.h"
#include "Adapter/GY_IICLCD.h"
#include "Adapter/DFRobot_IIC.h"
#include "Shield/LCD4884.h"
#include "Shield/LCD_Keypad.h"

// Configure: HD44780 Adapter; PP7W, SR3W, SR4W or TWI PCF8574
// LCD::PP7W<BOARD::D4, BOARD::D5, BOARD::D6, BOARD::D7, BOARD::D8, BOARD::D9, BOARD::D10> io;
// LCD::SR3W<BOARD::D7, BOARD::D6, BOARD::D5> io;
// LCD::SR4W<BOARD::D7, BOARD::D6, BOARD::D5, BOARD::D4> io;
// Software::TWI<BOARD::D18, BOARD::D19> twi;
// Hardware::TWI twi(100000UL);
// Hardware::TWI twi(400000UL);
// LCD::MJKDZ io(twi);
// LCD::GY_IICLCD io(twi);
// LCD::DFRobot_IIC io(twi);

// Configure: HD44780, PCD8544, LCD4884 or LCD Keypad
// HD44780 lcd(io);
// PCD8544<BOARD::D5, BOARD::D4, BOARD::D3, BOARD::D2> lcd;
LCD4884 lcd;
// LCD_Keypad lcd;

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

void fill_screen()
{
  for (int h = 0; h < lcd.HEIGHT; h++)
    for (int w = 0; w < lcd.WIDTH; w++)
      lcd.print((char) ('A' + w));
}

void setup()
{
  Serial.begin(57600);
  while (!Serial);
}

void loop()
{
  uint32_t start, stop;
  uint16_t nr = 0;

  // Character and String print
  MEASURE(lcd.begin());
  MEASURE(lcd.write('0'));
  MEASURE(lcd.print('0'));
  MEASURE(lcd.print("0"));
  MEASURE(lcd.print("10"));
  MEASURE(lcd.print("100"));
  MEASURE(lcd.print("1000"));
  MEASURE(lcd.print("10000"));
  MEASURE(lcd.print("Hello World"));
  MEASURE(lcd.print(F("Hello World")));
  MEASURE(lcd.print(F("\aHello World\a")));
  MEASURE(lcd.print(F("Hello\tWorld")));
  MEASURE(lcd.print(F("Hello\nWorld")));
  MEASURE(lcd.print(F("\nHello\tWorld")));
  MEASURE(fill_screen());

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

  // Special functions
  MEASURE(lcd.backlight_off());
  MEASURE(lcd.backlight_on());
  MEASURE(lcd.display_off());
  MEASURE(lcd.display_on());
  MEASURE(lcd.display_normal());
  MEASURE(lcd.display_inverse());
  MEASURE(lcd.display_clear());
  MEASURE(lcd.cursor_blink_on());
  MEASURE(lcd.cursor_blink_off());
  MEASURE(lcd.cursor_set(0,0));
  MEASURE(lcd.cursor_home());

  // Special characters
  MEASURE(lcd.print('\a'));
  MEASURE(lcd.print('\b'));
  MEASURE(lcd.print('\f'));
  MEASURE(lcd.print('\n'));
  MEASURE(lcd.print('\r'));
  MEASURE(lcd.print('\t'));

  // HD44780 specific
#if !defined(PCD8544_H)
  MEASURE(lcd.cursor_underline_on());
  MEASURE(lcd.cursor_underline_off());
  MEASURE(lcd.cursor_blink_on());
  MEASURE(lcd.cursor_blink_off());
#endif

  // That all in this benchmark
  MEASURE(lcd.end());
  Serial.println();
}

/*
--------------------------------------------------------------------------------
Arduino Leonardo
LCD::PP7W<> io;
HD44780 lcd(io);
--------------------------------------------------------------------------------
0: lcd.begin(): 54820
1: lcd.write('0'): 44
2: lcd.print('0'): 44
3: lcd.print("0"): 52
4: lcd.print("10"): 92
5: lcd.print("100"): 136
6: lcd.print("1000"): 180
7: lcd.print("10000"): 228
8: lcd.print("Hello World"): 496
9: lcd.print(F("Hello World")): 496
10: lcd.print(F("\aHello World\a")): 500
11: lcd.print(F("Hello\tWorld")): 492
12: lcd.print(F("Hello\nWorld")): 1216
13: lcd.print(F("\nHello\tWorld")): 1276
14: fill_screen(): 2176
15: lcd.print(0): 96
16: lcd.print(1): 96
17: lcd.print(10): 176
18: lcd.print(100): 268
19: lcd.print(INT16_MIN): 464
20: lcd.print(INT16_MAX): 432
21: lcd.print(INT32_MIN): 884
22: lcd.print(INT32_MAX): 844
23: lcd.print(1234.56789, 5): 1028
24: lcd.print(-1234.56789, 5): 1068
25: lcd.print(0x8000, BIN): 1320
26: lcd.print(0x8000, DEC): 424
27: lcd.print(0x8000, HEX): 340
28: lcd.backlight_off(): 4
29: lcd.backlight_on(): 8
30: lcd.display_off(): 40
31: lcd.display_on(): 40
32: lcd.display_normal(): 0
33: lcd.display_inverse(): 4
34: lcd.display_clear(): 1668
35: lcd.cursor_blink_on(): 40
36: lcd.cursor_blink_off(): 40
37: lcd.cursor_set(0,0): 44
38: lcd.cursor_home(): 1660
39: lcd.print('\a'): 8
40: lcd.print('\b'): 132
41: lcd.print('\f'): 1656
42: lcd.print('\n'): 784
43: lcd.print('\r'): 48
44: lcd.print('\t'): 52
45: lcd.end(): 40
--------------------------------------------------------------------------------
Arduino Pro Mini
LCD::SR4W<> io;
HD44780 lcd(io);
--------------------------------------------------------------------------------
0: lcd.begin(): 49828
1: lcd.write('0'): 56
2: lcd.print('0'): 48
3: lcd.print("0"): 56
4: lcd.print("10"): 100
5: lcd.print("100"): 148
6: lcd.print("1000"): 196
7: lcd.print("10000"): 248
8: lcd.print("Hello World"): 520
9: lcd.print(F("Hello World")): 516
10: lcd.print(F("\aHello World\a")): 520
11: lcd.print(F("Hello\tWorld")): 520
12: lcd.print(F("Hello\nWorld")): 1304
13: lcd.print(F("\nHello\tWorld")): 1352
14: fill_screen(): 2324
15: lcd.print(0): 100
16: lcd.print(1): 96
17: lcd.print(10): 184
18: lcd.print(100): 268
19: lcd.print(INT16_MIN): 492
20: lcd.print(INT16_MAX): 436
21: lcd.print(INT32_MIN): 912
22: lcd.print(INT32_MAX): 868
23: lcd.print(1234.56789, 5): 1052
24: lcd.print(-1234.56789, 5): 1100
25: lcd.print(0x8000, BIN): 1360
26: lcd.print(0x8000, DEC): 432
27: lcd.print(0x8000, HEX): 348
28: lcd.backlight_off(): 8
29: lcd.backlight_on(): 4
30: lcd.display_off(): 44
31: lcd.display_on(): 44
32: lcd.display_normal(): 4
33: lcd.display_inverse(): 4
34: lcd.display_clear(): 1648
35: lcd.cursor_blink_on(): 44
36: lcd.cursor_blink_off(): 44
37: lcd.cursor_set(0,0): 48
38: lcd.cursor_home(): 1648
39: lcd.print('\a'): 4
40: lcd.print('\b'): 144
41: lcd.print('\f'): 1656
42: lcd.print('\n'): 836
43: lcd.print('\r'): 52
44: lcd.print('\t'): 56
45: lcd.end(): 44
--------------------------------------------------------------------------------
Arduino Pro Mini
LCD::SR3W<> io;
HD44780 lcd(io);
--------------------------------------------------------------------------------
0: lcd.begin(): 54840
1: lcd.write('0'): 60
2: lcd.print('0'): 60
3: lcd.print("0"): 68
4: lcd.print("10"): 124
5: lcd.print("100"): 184
6: lcd.print("1000"): 240
7: lcd.print("10000"): 296
8: lcd.print("Hello World"): 656
9: lcd.print(F("Hello World")): 648
10: lcd.print(F("\aHello World\a")): 656
11: lcd.print(F("Hello\tWorld")): 660
12: lcd.print(F("Hello\nWorld")): 1636
13: lcd.print(F("\nHello\tWorld")): 1700
14: fill_screen(): 2920
15: lcd.print(0): 116
16: lcd.print(1): 112
17: lcd.print(10): 208
18: lcd.print(100): 300
19: lcd.print(INT16_MIN): 560
20: lcd.print(INT16_MAX): 504
21: lcd.print(INT32_MIN): 1044
22: lcd.print(INT32_MAX): 984
23: lcd.print(1234.56789, 5): 1168
24: lcd.print(-1234.56789, 5): 1232
25: lcd.print(0x8000, BIN): 1548
26: lcd.print(0x8000, DEC): 492
27: lcd.print(0x8000, HEX): 396
28: lcd.backlight_off(): 4
29: lcd.backlight_on(): 4
30: lcd.display_off(): 56
31: lcd.display_on(): 56
32: lcd.display_normal(): 4
33: lcd.display_inverse(): 4
34: lcd.display_clear(): 1668
35: lcd.cursor_blink_on(): 52
36: lcd.cursor_blink_off(): 56
37: lcd.cursor_set(0,0): 56
38: lcd.cursor_home(): 1668
39: lcd.print('\a'): 4
40: lcd.print('\b'): 184
41: lcd.print('\f'): 1672
42: lcd.print('\n'): 1044
43: lcd.print('\r'): 60
44: lcd.print('\t'): 68
45: lcd.end(): 56
--------------------------------------------------------------------------------
Arduino Uno
Software::TWI<BOARD::D18, BOARD::D19> twi;
LCD::GY_IICLCD io(twi);
HD44780 lcd(io);
--------------------------------------------------------------------------------
0: lcd.begin(): 58160
1: lcd.write('0'): 496
2: lcd.print('0'): 496
3: lcd.print("0"): 496
4: lcd.print("10"): 992
5: lcd.print("100"): 1484
6: lcd.print("1000"): 1976
7: lcd.print("10000"): 2468
8: lcd.print("Hello World"): 5424
9: lcd.print(F("Hello World")): 5408
10: lcd.print(F("\aHello World\a")): 5420
11: lcd.print(F("Hello\tWorld")): 5420
12: lcd.print(F("Hello\nWorld")): 13756
13: lcd.print(F("\nHello\tWorld")): 14260
14: fill_screen(): 24568
15: lcd.print(0): 540
16: lcd.print(1): 548
17: lcd.print(10): 1072
18: lcd.print(100): 1608
19: lcd.print(INT16_MIN): 3156
20: lcd.print(INT16_MAX): 2668
21: lcd.print(INT32_MIN): 5808
22: lcd.print(INT32_MAX): 5320
23: lcd.print(1234.56789, 5): 5504
24: lcd.print(-1234.56789, 5): 5988
25: lcd.print(0x8000, BIN): 8480
26: lcd.print(0x8000, DEC): 2664
27: lcd.print(0x8000, HEX): 2132
28: lcd.backlight_off(): 212
29: lcd.backlight_on(): 208
30: lcd.display_off(): 484
31: lcd.display_on(): 484
32: lcd.display_normal(): 4
33: lcd.display_inverse(): 4
34: lcd.display_clear(): 2096
35: lcd.cursor_blink_on(): 492
36: lcd.cursor_blink_off(): 488
37: lcd.cursor_set(0,0): 488
38: lcd.cursor_home(): 2096
39: lcd.print('\a'): 4
40: lcd.print('\b'): 1472
41: lcd.print('\f'): 2100
42: lcd.print('\n'): 8836
43: lcd.print('\r'): 492
44: lcd.print('\t'): 504
45: lcd.end(): 492
--------------------------------------------------------------------------------
Arduino Uno
Hardware::TWI twi(100000UL);
LCD::GY_IICLCD io(twi);
HD44780 lcd(io);
--------------------------------------------------------------------------------
0: lcd.begin(): 58384
1: lcd.write('0'): 516
2: lcd.print('0'): 520
3: lcd.print("0"): 528
4: lcd.print("10"): 1044
5: lcd.print("100"): 1560
6: lcd.print("1000"): 2080
7: lcd.print("10000"): 2596
8: lcd.print("Hello World"): 5708
9: lcd.print(F("Hello World")): 5700
10: lcd.print(F("\aHello World\a")): 5720
11: lcd.print(F("Hello\tWorld")): 5720
12: lcd.print(F("Hello\nWorld")): 14524
13: lcd.print(F("\nHello\tWorld")): 15036
14: fill_screen(): 25896
15: lcd.print(0): 568
16: lcd.print(1): 572
17: lcd.print(10): 1128
18: lcd.print(100): 1684
19: lcd.print(INT16_MIN): 3308
20: lcd.print(INT16_MAX): 2812
21: lcd.print(INT32_MIN): 6096
22: lcd.print(INT32_MAX): 5580
23: lcd.print(1234.56789, 5): 5764
24: lcd.print(-1234.56789, 5): 6280
25: lcd.print(0x8000, BIN): 8912
26: lcd.print(0x8000, DEC): 2796
27: lcd.print(0x8000, HEX): 2232
28: lcd.backlight_off(): 228
29: lcd.backlight_on(): 228
30: lcd.display_off(): 512
31: lcd.display_on(): 520
32: lcd.display_normal(): 0
33: lcd.display_inverse(): 4
34: lcd.display_clear(): 2120
35: lcd.cursor_blink_on(): 512
36: lcd.cursor_blink_off(): 516
37: lcd.cursor_set(0,0): 520
38: lcd.cursor_home(): 2124
39: lcd.print('\a'): 4
40: lcd.print('\b'): 1560
41: lcd.print('\f'): 2128
42: lcd.print('\n'): 9320
43: lcd.print('\r'): 520
44: lcd.print('\t'): 524
45: lcd.end(): 516
--------------------------------------------------------------------------------
Hardware::TWI twi(400000UL);
LCD::GY_IICLCD io(twi);
HD44780 lcd(io);
--------------------------------------------------------------------------------
0: lcd.begin(): 55896
1: lcd.write('0'): 176
2: lcd.print('0'): 180
3: lcd.print("0"): 184
4: lcd.print("10"): 356
5: lcd.print("100"): 528
6: lcd.print("1000"): 704
7: lcd.print("10000"): 880
8: lcd.print("Hello World"): 1924
9: lcd.print(F("Hello World")): 1928
10: lcd.print(F("\aHello World\a")): 1932
11: lcd.print(F("Hello\tWorld")): 1932
12: lcd.print(F("Hello\nWorld")): 4896
13: lcd.print(F("\nHello\tWorld")): 5080
14: fill_screen(): 8736
15: lcd.print(0): 224
16: lcd.print(1): 228
17: lcd.print(10): 436
18: lcd.print(100): 656
19: lcd.print(INT16_MIN): 1256
20: lcd.print(INT16_MAX): 1076
21: lcd.print(INT32_MIN): 2316
22: lcd.print(INT32_MAX): 2148
23: lcd.print(1234.56789, 5): 2332
24: lcd.print(-1234.56789, 5): 2504
25: lcd.print(0x8000, BIN): 3404
26: lcd.print(0x8000, DEC): 1072
27: lcd.print(0x8000, HEX): 864
28: lcd.backlight_off(): 84
29: lcd.backlight_on(): 88
30: lcd.display_off(): 172
31: lcd.display_on(): 168
32: lcd.display_normal(): 4
33: lcd.display_inverse(): 4
34: lcd.display_clear(): 1776
35: lcd.cursor_blink_on(): 168
36: lcd.cursor_blink_off(): 180
37: lcd.cursor_set(0,0): 176
38: lcd.cursor_home(): 1776
39: lcd.print('\a'): 4
40: lcd.print('\b'): 532
41: lcd.print('\f'): 1788
42: lcd.print('\n'): 3160
43: lcd.print('\r'): 180
44: lcd.print('\t'): 184
45: lcd.end(): 172
--------------------------------------------------------------------------------
Arduino Uno
PCD8544<> lcd;
--------------------------------------------------------------------------------
0: lcd.begin(): 4388
1: lcd.write('0'): 64
2: lcd.print('0'): 68
3: lcd.print("0"): 80
4: lcd.print("10"): 136
5: lcd.print("100"): 208
6: lcd.print("1000"): 272
7: lcd.print("10000"): 332
8: lcd.print("Hello World"): 716
9: lcd.print(F("Hello World")): 720
10: lcd.print(F("\aHello World\a")): 744
11: lcd.print(F("Hello\tWorld")): 684
12: lcd.print(F("Hello\nWorld")): 1424
13: lcd.print(F("\nHello\tWorld")): 1448
14: fill_screen(): 9252
15: lcd.print(0): 120
16: lcd.print(1): 116
17: lcd.print(10): 224
18: lcd.print(100): 328
19: lcd.print(INT16_MIN): 592
20: lcd.print(INT16_MAX): 532
21: lcd.print(INT32_MIN): 1112
22: lcd.print(INT32_MAX): 1048
23: lcd.print(1234.56789, 5): 1236
24: lcd.print(-1234.56789, 5): 1296
25: lcd.print(0x8000, BIN): 2428
26: lcd.print(0x8000, DEC): 528
27: lcd.print(0x8000, HEX): 428
28: lcd.backlight_off(): 4
29: lcd.backlight_on(): 4
30: lcd.display_off(): 12
31: lcd.display_on(): 12
32: lcd.display_normal(): 12
33: lcd.display_inverse(): 12
34: lcd.display_clear(): 4332
35: lcd.cursor_blink_on(): 4
36: lcd.cursor_blink_off(): 4
37: lcd.cursor_set(0,0): 24
38: lcd.cursor_home(): 28
39: lcd.print('\a'): 8
40: lcd.print('\b'): 88
41: lcd.print('\f'): 4340
42: lcd.print('\n'): 764
43: lcd.print('\r'): 32
44: lcd.print('\t'): 36
45: lcd.end(): 4340
--------------------------------------------------------------------------------
*/
