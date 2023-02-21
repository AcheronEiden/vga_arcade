/*
 VGA Arcade SID - Version 20230221

 CHANGES 2022-12-21 -- 2023-02-21 FOR 1TE663 PROJECT.
 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)
*/

#include <avr/io.h> //TH: To set IO pins using C
#include <Arduino.h> //MK
#include <SID.h>
#include <Wire.h>
#include <inttypes.h>
#include <String.h>
#include <avr/pgmspace.h>  //used to store data in the flash rom
#include "toneA.h" //RAW SID register data file in flash, Gameplay music (play at start)
#include "toneB.h" //RAW SID register data file in flash, Ball hit pad
#include "toneC.h" //RAW SID register data file in flash, Ball hit block
#include "toneD.h" //RAW SID register data file in flash, Game Over

/************************************************************************

        Arduino controls an
	Atmega8 MOS6581 SID Emulator
        by softare serial line
        
	( SID = Sound Interface Device )

         random sound pitch example
        


****************************************************************************

	date	authors					version		comment
	======	======================	=======		==============================
	2007 	(ch) Christoph Haberer	V1.0		First implementation
	2012 	Mario Patino    	V1.1		Adapted for Arduino SID Library

	Versions:

	V1.0
	- 2 channel random pitch

	V1.1
	- Adapted to work with the Arduino SID Library

***************************************************************************/
/***************************************************************************
 *   
 *   (c) 2007 Christoph Haberer, christoph(at)roboterclub-freiburg.de
 *
 ***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation version 2 of the License,                *
 *   If you extend the program please maintain the list of authors.        *
 *   If you want to use this software for commercial purposes and you      *
 *   don't want to make it open source, please contact the authors for     *
 *   licensing.                                                            *
 ***************************************************************************/
/*
<!> Programmer = USBasp for arduino Nano ATmega328
 Brian Tucker 1-5-2015
 sid-arduino-lib:
 https://code.google.com/p/sid-arduino-lib/
 based SID player example program sets
 the 25 SID/6581 registers at 50Hz (delay(19)=~50Hz) 
 The register data is created using the excellent app
 "SIDDumper" from the SIDCog project:
 SIDCog:
 http://forums.parallax.com/showthread.php/118285-SIDcog-The-sound-of-the-Commodore-64-!-%28Now-in-the-OBEX%29
 SIDDumper:
 http://gadgetgangster.com/scripts/displayasset.php?id=361
 For best results use an IRQ at 50 or 60Hz and set the registers programatically.
 For info on how to make music with the SID chip and related registers
 I recomend the e-magazine "Commodore Hacking":
 http://www.ffd2.com/fridge/chacking/
 Specifically the Rob Hubbard play subroutine as that is the code
 all serious C64 SID musicians must understand.
 convert the dump to number with : http://www.expertsetup.com/~brian/c64/JConverter.jar / http://softcollection.sytes.net/javaprog
 
*/
// https://github.com/daitangio/sid-arduino-lib/blob/master/examples/SID_I2C_receiver/SID_I2C_receiver.ino
// Connect PIN 9 from your arduino Uno to audio output (+ ground to the other pin of the audio output)!



/*
//MK
// code for master arduino if we use Wire.h
Wire.beginTransmission(23);   // arbitrary address 23                                                
Wire.write('A');  // or 'B', 'C', 'D' (which sound effect we want to play)                                          
Wire.endTransmission();
-----------------------------------------------------------------------------------------------------------------
If the arduino is overwhelmed, we can make a separate function called for instance receiveEvent, and add Wire.onReceive(receiveEvent) to the setup. This will direct the program to
the specified function in case a message is recieved. The main loop can then include a delay for example. 

*/

SID mySid;


void setup()  {
  Wire.begin(23); // join i2c bus with address #23
  mySid.begin();
  
} 

void functionA() {
for(uint16_t sidPointer=0;sidPointer<=sidLength1;sidPointer++){
    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, (pgm_read_byte(&sidData1[(sidPointer+sidRegister)])));
    };
    delay(19);
    sidPointer=sidPointer+24;
  };
  // set all registers to 0 to stop playing music/ stop noise from continuing
  delay(19);
  for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, 0);
  };
}

void functionB() {
for(uint16_t sidPointer=0;sidPointer<=sidLength2;sidPointer++){
    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, (pgm_read_byte(&sidData2[(sidPointer+sidRegister)])));
    };
    delay(19);
    sidPointer=sidPointer+24;
  };
  // set all registers to 0 to stop playing music/ stop noise from continuing
  delay(19);
  for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, 0);
  };
}

void functionC() {
for(uint16_t sidPointer=0;sidPointer<=sidLength3;sidPointer++){
    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, (pgm_read_byte(&sidData3[(sidPointer+sidRegister)])));
    };
    delay(19);
    sidPointer=sidPointer+24;
  };
  // set all registers to 0 to stop playing music/ stop noise from continuing
  delay(19);
  for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, 0);
  };
}

void functionD() {
for(uint16_t sidPointer=0;sidPointer<=sidLength4;sidPointer++){
    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, (pgm_read_byte(&sidData4[(sidPointer+sidRegister)])));
    };
    delay(19);
    sidPointer=sidPointer+24;
  };
  // set all registers to 0 to stop playing music/ stop noise from continuing
  delay(19);
  for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, 0);
  };
}

void loop() {
  while (Wire.available() > 0) {
  char command = Wire.read();
    if (command == 'A') 
    {
      functionA();
    }
    else if (command == 'B')
    {
      functionB();
    }
    else if (command == 'C')
    {
      functionC();
    }
    else if (command == 'C')
    {
      functionC();
    }
  }
}