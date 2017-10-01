# Arduino-LCD
The LCD library provides an abstract interface for LCD devices and an
adapter design pattern to allow device drivers to be reused even when
LCD device communication changes. The library includes device drivers
for MAX72XX, HD44780 and PCD8544, and adapter using GPIO, Shift
Registers, and TWI (PCF8574).

Version: 1.2

## Classes

* [LCD Device Driver Support, LCD::Device](./src/LCD.h)

## Device Drivers

* [HD44780](./src/Driver/HD44780.h)
* [MAX72XX](./src/Driver/MAX72XX.h)
* [PCD8544](./src/Driver/PCD8544.h)

## Port Adapters (HD44780)

* [HD44780::Adapter](./src/Driver/HD44780.h)
* [Debug, Serial](./src/Adapter/Debug.h)
* [3-Wire, Shift Register, GPIO](./src/Adapter/SR3W.h)
* [4-Wire, Shift Register, GPIO](./src/Adapter/SR4W.h)
* [7-Wire, 4-bit Parallel Port, GPIO](./src/Adapter/PP7W.h)
* [DFRobot_IIC, PCF8574, TWI](./src/Adapter/DFRobot_IIC.h)
* [GY_IICLCD, PCF8574, TWI](./src/Adapter/GY_IICLCD.h)
* [MJKDZ, PCF8574, TWI](./src/Adapter/MJKDZ.h)

## Example Sketches

* [Demo](./examples/Demo)
* [Benchmark](./examples/Benchmark)

## Benchmarks

Nr|Operation|PP7W|SR4W|SR3W|TWI(SW)|TWI(100)|TWI(400)|PCD8544
--|---------|----|----|----|-------|--------|--------|-------
0|lcd.begin()|54820|49828|54840|58160|58384|55896|4388
1|lcd.write('0')|44|56|60|496|516|176|64
2|lcd.print('0')|44|48|60|496|520|180|68
3|lcd.print("0")|52|56|68|496|528|184|80
4|lcd.print("10")|92|100|124|992|1044|356|136
5|lcd.print("100")|136|148|184|1484|1560|528|208
6|lcd.print("1000")|180|196|240|1976|2080|704|272
7|lcd.print("10000")|228|248|296|2468|2596|880|332
8|lcd.print("Hello World")|496|520|656|5424|5708|1924|716
9|lcd.print(F("Hello World"))|496|516|648|5408|5700|1928|720
10|lcd.print(F("\aHello World\a"))|500|520|656|5420|5720|1932|744
11|lcd.print(F("Hello\tWorld"))|492|520|660|5420|5720|1932|684
12|lcd.print(F("Hello\nWorld"))|1216|1304|1636|13756|14524|4896|1424
13|lcd.print(F("\nHello\tWorld"))|1276|1352|1700|14260|15036|5080|1448
14|fill_screen()|2176|2324|2920|24568|25896|8736|9252
15|lcd.print(0)|96|100|116|540|568|224|120
16|lcd.print(1)|96|96|112|548|572|228|116
17|lcd.print(10)|176|184|208|1072|1128|436|224
18|lcd.print(100)|268|268|300|1608|1684|656|328
19|lcd.print(INT16_MIN)|464|492|560|3156|3308|1256|592
20|lcd.print(INT16_MAX)|432|436|504|2668|2812|1076|532
21|lcd.print(INT32_MIN)|884|912|1044|5808|6096|2316|1112
22|lcd.print(INT32_MAX)|844|868|984|5320|5580|2148|1048
23|lcd.print(1234.56789, 5)|1028|1052|1168|5504|5764|2332|1236
24|lcd.print(-1234.56789, 5)|1068|1100|1232|5988|6280|2504|1296
25|lcd.print(0x8000, BIN)|1320|1360|1548|8480|8912|3404|2428
26|lcd.print(0x8000, DEC)|424|432|492|2664|2796|1072|528
27|lcd.print(0x8000, HEX)|340|348|396|2132|2232|864|428
28|lcd.backlight_off()|4|8|4|212|228|84|4
29|lcd.backlight_on()|8|4|4|208|228|88|4
30|lcd.display_off()|40|44|56|484|512|172|12
31|lcd.display_on()|40|44|56|484|520|168|12
32|lcd.display_normal()|4|4|4|4|4|4|12
33|lcd.display_inverse()|4|4|4|4|4|4|12
34|lcd.display_clear()|1668|1648|1668|2096|2120|1776|4332
35|lcd.cursor_blink_on()|40|44|52|492|512|168|4
36|lcd.cursor_blink_off()|40|44|56|488|516|180|4
37|lcd.cursor_set(0,0)|44|48|56|488|520|176|24
38|lcd.cursor_home()|1660|1648|1668|2096|2124|1776|28
39|lcd.print('\a')|8|4|4|4|4|4|8
40|lcd.print('\b')|132|144|184|1472|1560|532|88
41|lcd.print('\f')|1656|1656|1672|2100|2128|1788|4340
42|lcd.print('\n')|784|836|1044|8836|9320|3160|764
43|lcd.print('\r')|48|52|60|492|520|180|32
44|lcd.print('\t')|52|56|68|504|524|184|36
45|lcd.end()|40||56|492|516|172|4340

## Dependencies

* [Arduino-GPIO](https://github.com/mikaelpatel/Arduino-GPIO)
* [Arduino-TWI](https://github.com/mikaelpatel/Arduino-TWI)
