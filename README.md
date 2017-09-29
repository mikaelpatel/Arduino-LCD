# Arduino-LCD
The LCD library provides an abstract interface for LCD devices and an
adapter design pattern to allow device drivers to be reused even when
LCD device communication changes. The library includes device drivers
for MAX72XX and HD44780, and adapter using GPIO, Shift Registers, and
TWI (PCF8574).

Version: 1.1

## Classes

* [LCD Device Driver Support, LCD::Device](./src/LCD.h)

## Device Drivers

* [HD44780](./src/Driver/HD44780.h)
* [MAX72XX](./src/Driver/MAX72XX.h)

## Port Adapters (HD44780)

* [HD44780::Adapter](./src/Driver/HD44780.h)
* [Debug, Serial](./src/Adapter/Debug.h)
* [3-Wire, Shift Register, GPIO](./src/Adapter/SR3W.h)
* [4-Wire, Shift Register, GPIO](./src/Adapter/SR4W.h)
* [7-Wire, 4-bit Parallel, GPIO](./src/Adapter/PP7W.h)
* [DFRobot_IIC, PCF8574, TWI](./src/Adapter/DFRobot_IIC.h)
* [GY_IICLCD, PCF8574, TWI](./src/Adapter/GY_IICLCD.h)
* [MJKDZ, PCF8574, TWI](./src/Adapter/MJKDZ.h)

## Example Sketches

* [Demo](./examples/Demo)
* [Benchmark](./examples/Benchmark)

## Benchmarks

Nr | Operation | PP7W | SR4W | SR3W | SoftTWI | TWI@100 | TWI@400
---|-----------|--------|------|------|---------|---------|--------
0 | lcd.begin() | 54800 | 54892 | 49864 | 58640 | 58892 | 56056
1 | lcd.backlight_off() | 4 | 4 | 8 | 212 | 228 | 88
2 | lcd.backlight_on() | 8 | 8 | 4 | 208 | 224 | 88
3 | lcd.display_off() | 40 | 56 | 44 | 492 | 512 | 172
4 | lcd.display_on() | 44 | 52 | 44 | 488 | 512 | 172
5 | lcd.display_clear() | 1652 | 1664 | 1656 | 2096 | 2120 | 1776
6 | lcd.cursor_home() | 1652 | 1660 | 1656 | 2096 | 2120 | 1784
7 | lcd.cursor_underline_on() | 36 | 56 | 44 | 492 | 512 | 172
8 | lcd.cursor_underline_off() | 40 | 56 | 44 | 484 | 512 | 172
9 | lcd.cursor_blink_off() | 40 | 60 | 44 | 480 | 516 | 172
10 | lcd.set_cursor(0,0) | 44 | 56 | 48 | 496 | 516 | 172
11 | lcd.line_clear() | 688 | 928 | 736 | 7860 | 8292 | 2796
12 | lcd.write('L') | 44 | 60 | 48 | 492 | 520 | 176
13 | lcd.print('\a') | 400028 | 400028 | 400032 | 400444 | 400468 | 400184
14 | lcd.print('\b') | 48 | 60 | 52 | 492 | 524 | 180
15 | lcd.print('\f') | 1656 | 1668 | 1656 | 2100 | 2128 | 1788
16 | lcd.print('\n') | 784 | 1056 | 844 | 8848 | 9324 | 3160
17 | lcd.print('\r') | 48 | 68 | 52 | 500 | 524 | 180
18 | lcd.print('\t') | 52 | 68 | 56 | 500 | 528 | 184
19 | lcd.print("0") | 48 | 68 | 52 | 496 | 528 | 180
20 | lcd.print("10") | 96 | 132 | 100 | 992 | 1048 | 356
21 | lcd.print("100") | 136 | 184 | 152 | 1480 | 1560 | 532
22 | lcd.print("1000") | 180 | 248 | 196 | 1976 | 2080 | 708
23 | lcd.print("10000") | 228 | 300 | 244 | 2464 | 2600 | 876
24 | lcd.print("Hello World") | 488 | 656 | 528 | 5424 | 5708 | 1936
25 | lcd.print(F("Hello World")) | 484 | 648 | 528 | 5416 | 5700 | 1928
26 | lcd.print(F("Hello\tWorld")) | 496 | 656 | 528 | 5428 | 5708 | 1936
27 | lcd.print(F("Hello\nWorld")) | 1224 | 1644 | 1316 | 13776 | 14500 | 4908
28 | lcd.print(F("\nHello\tWorld")) | 1272 | 1708 | 1368 | 14272 | 15048 | 5072
29 | lcd.print(0) | 96 | 112 | 100 | 544 | 576 | 228
30 | lcd.print(1) | 96 | 108 | 100 | 540 | 572 | 228
31 | lcd.print(10) | 180 | 208 | 184 | 1076 | 1128 | 444
32 | lcd.print(100) | 260 | 304 | 276 | 1600 | 1680 | 656
33 | lcd.print(INT16_MIN) | 472 | 556 | 492 | 3156 | 3312 | 1256
34 | lcd.print(INT16_MAX) | 424 | 496 | 440 | 2668 | 2800 | 1076
35 | lcd.print(INT32_MIN) | 884 | 1048 | 920 | 5808 | 6104 | 2324
36 | lcd.print(INT32_MAX) | 840 | 992 | 872 | 5320 | 5576 | 2144
37 | lcd.print(1234.56789, 5) | 1028 | 1176 | 1056 | 5504 | 5780 | 2340
38 | lcd.print(-1234.56789, 5) | 1072 | 1232 | 1108 | 6000 | 6288 | 2516
39 | lcd.print(0x8000, BIN) | 1312 | 1560 | 1364 | 8484 | 8896 | 3404
40 | lcd.print(0x8000, DEC) | 420 | 500 | 436 | 2668 | 2792 | 1080
41 | lcd.print(0x8000, HEX) | 344 | 396 | 356 | 2132 | 2244 | 860
42 | lcd.end() | 44 | 56 | 44 | 496 | 512 | 172

## Dependencies

* [Arduino-GPIO](https://github.com/mikaelpatel/Arduino-GPIO)
* [Arduino-TWI](https://github.com/mikaelpatel/Arduino-TWI)
