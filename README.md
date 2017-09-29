# Arduino-LCD
The LCD library provides an abstract interface for LCD devices and an
adapter design pattern to allow device drivers to be reused even when
LCD device communication changes. The library includes device drivers
for MAX72XX and HD44780, and adapter using GPIO, Shift Register, and
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
* [3-Wire Shift Register, GPIO](./src/Adapter/SR3W.h)
* [4-Wire Shift Register, GPIO](./src/Adapter/SR4W.h)
* [7-Wire, 4-bit Parallel, GPIO](./src/Adapter/Port4b.h)
* [DFRobot_IIC, PCF8574, TWI](./src/Adapter/DFRobot_IIC.h)
* [GY_IICLCD, PCF8574, TWI](./src/Adapter/GY_IICLCD.h)
* [MJKDZ, PCF8574, TWI](./src/Adapter/MJKDZ.h)

## Example Sketches

* [Demo](./examples/Demo)
* [Benchmark](./examples/Benchmark)

## Dependencies

* [Arduino-GPIO](https://github.com/mikaelpatel/Arduino-GPIO)
* [Arduino-TWI](https://github.com/mikaelpatel/Arduino-TWI)
