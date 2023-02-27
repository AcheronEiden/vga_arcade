#include <avr/io.h> //TH: To set IO pins using C
#include <Arduino.h> //MK
#include <SID.h>
#include <Wire.h>
#include <inttypes.h>
#include <String.h>
#include <avr/pgmspace.h>  //used to store data in the flash rom
//#include "Arkanoid10.h" //RAW SID register data file in flash
#include "toneB.h" //RAW SID register data file in flash
SID mySid;


void setup()  { 
  mySid.begin();
      } 

void loop() {
  for(uint16_t sidPointer=0;sidPointer<=sidLengthArkanoid10;sidPointer++){
    for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, (pgm_read_byte(&sidDataArkanoid10[(sidPointer+sidRegister)])));
    };
    delay(19);
    sidPointer=sidPointer+24;
  };
  // set all registers to 0 to stop playing music/ stop noise from continuing
  delay(19);
  for(uint8_t sidRegister=0;sidRegister<=24;sidRegister++){
      mySid.set_register(sidRegister, 0);
  };
  delay(1000);
}