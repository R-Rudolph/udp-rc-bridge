# udp-rc-bridge
Arduino project that bridges between UDP messages and 433MHz signals. The arduino is connected an ethernet shield (ENC28J60) and 433MHz transmitter and receiver modules. It listenes on a single UDP port (currently 17000) for a signal to send (in decimal string format) and sends these via the 433MHz transmitter. When it receives a 433MHz signal, it broadcasts it to the network (via UDP port 17001).

## Parts

* Arduino Mega 2560
* [ENC28J60](https://www.amazon.de/-/en/gp/product/B07D8SV85Q)
* [433MHz sender and receiver](https://www.amazon.de/-/en/gp/product/B01N5GV39I)

## Libraries

* [EtherCard](https://github.com/njh/EtherCard)
* [RCSwitch](https://github.com/sui77/rc-switch)

## Wiring

The wiring given is for the Arduino Mega 2560.

#### ENC28J60

Wiring is mostly done as descripted in the EtherCard library, but on my ethernet shield some of the pin names were different and I used 5V instead of 3.3V.

| ENC28J60      | Arduino       |
| ------------- |:-------------:|
| VCC           | 5V            |
| GND           | GND           |
| SCK           | Pin 52        |
| SO            | Pin 50        |
| ST            | Pin 51        |
| CS            | Pin 53        |

All other pins were left unconnected.

#### 433MHz transmitter

| Transmitter   | Arduino       |
| ------------- |:-------------:|
| VCC           | 5V            |
| GND           | GND           |
| Data          | Pin 10        |

#### 433MHz receiver

The receiver has two (unmarked) data pins. Both seem to work.

| Transmitter   | Arduino                        |
| ------------- |:------------------------------:|
| VCC           | 5V                             |
| GND           | GND                            |
| Data          | Pin 2 (interrupt pin 0)        |
