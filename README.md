Forked from Tobias Holm & Mohammed Nour Kamalmaz
# vga_arcade
1TE663 project

One Arduino controls the VGA output, controls the slave Arduino to play music/sfx, reads Wii-controller and handles the game logic.

## VGA
The VGA-library implement a 120x60px framebuffer. The code produces 2-bit color output,
and in this project we have used the colors black, white, red and cyan.
The framebuffer is stored inside SRAM and require at least 1800 bytes.
This mean that on ATMega328 the programs cannot use more than 200 bytes of SRAM.
| | R, pin D6 (PD6) | G, pin D7 (PD7) | B, pin D7 (PD7)|
| --- | --- | --- | --- |
|00: Black|	0|	0|	0|
|01: Cyan|	0|	1|	1|
|10: Red|	1|	0|	0|
|11: White|	1|	1|	1|
See color combinations at [smaffer github page](https://github.com/smaffer/vgax).

The VGA-library can produce a simple square wave tone output to a buzzer, connected on pin A0 (PC0).

The VGA-signal is produced using two interrupts:

TIMER1_OVF_vect: VSYNC interrupt, 60Hz, output on pin D9 (PB1).

TIMER2_OVF_vect: HSYNC interrupt, 15kHz, output on pin D3 (PD3).

Video generation is implemented using PORTD, so we cannot use any of the PORTD pins to other tasks.

## Timers
This library uses all the 3 timers of ATMega328 MCU.

TIMER1 and TIMER2 are configured to generate VSYNC and HSYNC pulses.

TIMER0 is used to fix the interrupt jitter. By default the TIMER0 is used by Arduino to implement these functions:

	unsigned millis();
	unsigned long micros();
	void delay(ms); 
	void delayMicroseconds(us);

Instead of using these functions, there is an alternative version provided by the VGA-library:

	vga.delay(10); // Delay 10 ms

VGAXUA provides an alternative version of rand() that can be used to reduce the SRAM memory usage:

	unsigned rand();


## Wii Nunchuk
A Wii-Nunchuk controller is connected to control the game.

SDA is connected to pin A4 (PC4), and SDA is connected to pin A5 (PC5).

GND and +3.3V is also connected to the Wii-controller.


## Milestones / Progress
The project milestones:
- [x] Get a stable VGA image on a TFT-screen using GPIO/timers/PWM/interrupts.
- [x] Use an analog potentiometer to control the game paddle (abandoned).
- [x] Use an analog potentiometer for volume using external amp IC.
- [x] Moving bouncing ball in a playable game.
- [x] Use a Wii-nunchuk (I2C) to control the game paddle.
- [x] Create advanced music and sfx using a secondary Arduino.
- [x] Create a 3D-printed box for the hardware.
- [x] Create the project report (=> 10 pages).

The project progress are marked below:
- [x] Code adjustment for C++ syntax.
- [x] Prototype hardware constructed and assembled.
- [x] Both ordinary buttons are now active low with internal pull-ups.
- [x] Simple buzzer sound effects tests.
- [x] Wii-nunchuk routine that can coexist with VGA timing.
- [x] SID Music and sound effects using an slave Atmel 328P.
- [x] Music controlled using I2C.
- [x] Full screen animations for main and load screens.


# External resources
This project uses the following public libraries and also used information from external sites listed below. The libraries are often modified to suit this project and we have made comments in the code when edits are made. In some parts of this project only smaller fractions of the public libraries are used.

## VGA references
VGAX library for Arduino UNO and MEGA [smaffer github page](https://github.com/smaffer/vgax)

VGAXUA library for Arduino UNO and MEGA [https://github.com/smaffer/vgaxua](https://github.com/smaffer/vgaxua)

Electrical connections for VGA-cable [https://forum.arduino.cc/t/vga-library-for-arduino-uno-and-atmega328/308936](https://forum.arduino.cc/t/vga-library-for-arduino-uno-and-atmega328/308936)

VGA Breakout With Arduino Uno [https://www.instructables.com/VGA-Breakout-With-Arduino-Uno/](https://www.instructables.com/VGA-Breakout-With-Arduino-Uno/)

## Nunchuk references
Wii-Nunchuk communication [https://github.com/infusion/Fritzing](https://github.com/infusion/Fritzing)

Using Nunchuk with I2C [https://www.xarg.org/2016/12/using-a-wii-nunchuk-with-arduino/](https://www.xarg.org/2016/12/using-a-wii-nunchuk-with-arduino/)

Technical info about the Wii Nunchuk Interface [https://bootlin.com/labs/doc/nunchuk.pdf](https://bootlin.com/labs/doc/nunchuk.pdf)

nuncha cpp-lib [https://github.com/mgalardini/nuncha/tree/master/nuncha](https://github.com/mgalardini/nuncha/tree/master/nuncha)

## I2C references
Arduino I2C master library [https://github.com/rambo/I2C](https://github.com/rambo/I2C)

Basics of the I2C Communication Protocol [https://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/](https://www.circuitbasics.com/basics-of-the-i2c-communication-protocol/)

## SID references
MOS6581 SID Emulator Arduino Library [https://github.com/cybernesto/sid-arduino-lib](https://github.com/cybernesto/sid-arduino-lib)

MOS6581 SID Emulator Extra examples MIDI [https://github.com/daitangio/sid-arduino-lib](https://github.com/daitangio/sid-arduino-lib)

SIDDump github [https://github.com/munshkr/siddump](https://github.com/munshkr/siddump)

SIDDump V1.08: [https://csdb.dk/release/?id=192079#google_vignette](https://csdb.dk/release/?id=192079#google_vignette)

SIDDumper from SIDCog project (tool missing) [https://forums.parallax.com/](https://forums.parallax.com/discussion/118285/sidcog-the-sound-of-the-commodore-64-now-in-the-obex/p1)

Tools for analyzing SID music [https://github.com/anarkiwi/desidulate](https://github.com/anarkiwi/desidulate)

ChiptuneSAK github [https://github.com/c64cryptoboy/ChiptuneSAK](https://github.com/c64cryptoboy/ChiptuneSAK)

High Voltage SID Collection, 56665 songs [https://hvsc.c64.org/](https://hvsc.c64.org/)

e-magazine "Commodore Hacking" [http://www.ffd2.com/fridge/chacking/](http://www.ffd2.com/fridge/chacking/)

midicsv [https://pypi.org/project/py-midicsv/](https://pypi.org/project/py-midicsv/)

Commodore SID Music, ChiptuneSAK [https://chiptunesak.readthedocs.io/en/latest/sid.html](https://chiptunesak.readthedocs.io/en/latest/sid.html)

SID file format description [https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/SID_file_format.txt](https://www.hvsc.c64.org/download/C64Music/DOCUMENTS/SID_file_format.txt)

MOS Technology 6581 wikipage [https://en.wikipedia.org/wiki/MOS_Technology_6581](https://en.wikipedia.org/wiki/MOS_Technology_6581)

Converting SID traces [http://www.ucapps.de/howto_sid_wavetables_1.html](http://www.ucapps.de/howto_sid_wavetables_1.html)

## Uwe references
Uwe:s blogg [https://www.sciencetronics.com/greenphotons/](https://www.sciencetronics.com/greenphotons/?p=337)
