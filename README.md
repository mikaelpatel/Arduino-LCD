# Arduino-LCD
The LCD library provides an abstract interface for LCD devices and an
adapter design pattern to allow device drivers to be reused even when
LCD device communication changes. The library includes device drivers
for MAX72XX and HD44780, and adapter using GPIO and TWI (PCF8574).

Version: 1.0

## Classes

* [LCD Device Driver Support, LCD::Device](./src/LCD.h)

## Device Drivers

* [HD44780](./src/Driver/HD44780.h)
* [MAX72XX](./src/Driver/MAX72XX.h)

## Port Adapters (HD44780)

* [Debug, Serial](./src/Adapter/Debug.h)
* [DFRobot_IIC, PCF8574, TWI](./src/Adapter/DFRobot_IIC.h)
* [GY_IICLCD, PCF8574, TWI](./src/Adapter/GY_IICLCD.h)
* [MAX72XX, PCF8574, TWI](./src/Adapter/MAX72XX.h)
* [Port4b, GPIO](./src/Adapter/Port4b.h)

## Example Sketches

* [Demo](./examples/Demo)
