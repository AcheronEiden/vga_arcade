/*
 VGA Arcade [Nunchuk.cpp] - for 1TE663 project - Version 20230228

 Created separate cpp-file to avoid code i h-file.

 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)
*/

#include "Nunchuk.h"

//TH:---vvv---
void NunChukData::mydelay() {
    int n;
    for (n = 1; n < 1000; ++n) { //TH: Count to 100 gives 700us delay?
        PORTC ^= 1; //TH: Toggle portA bit 0 // Just to DO something
    }
}

//TH: Return the Nunchuk data array
uint8_t* NunChukData::getNunChukData() {
    return nunchuk_data;
}

//TH: Init Nunchuk without encryption, and without using I2C macros
void NunChukData::nunchuk_init() {
    Wire.beginTransmission(NUNCHUK_ADDRESS);
    Wire.write(0xF0);
    Wire.write(0x55);
    Wire.endTransmission(true); //TH: normally set to 'true'.
    
    Wire.beginTransmission(NUNCHUK_ADDRESS);
    Wire.write(0xFB);
    Wire.write(0x00);
    Wire.endTransmission(true); //TH: normally set to 'true'.
}

// Read a full chunk of data from Nunchuk. @return A boolean if the data transfer was successful
uint8_t NunChukData::nunchuk_read() {
    // nunchuk_data[0] = nunchuk_data[7]; //TH: Simulate x-pos
    // if (nunchuk_data[7]!=0) {
        // PORTC ^= 0b00000010; //TH: DEBUG Toggle portC bit 1 (signal A1)
        // nunchuk_data[7] = 0; //TH: Set busy flag

        // nunchuk_data[5] = 1; //TH: Simulate a Release of button Z
        // nunchuk_data[0] = 47; //TH: Simulate x-pos
        // uint16_t no1 = 0; // Used as nameless temp-var to save RAM. (small loops, reading wheelPosition)


// request data from device
uint8_t numBytesRead = Wire.requestFrom ( NUNCHUK_ADDRESS, 6 );
if ( numBytesRead != 6 ) return ( false );

// read the data and wait until all bytes have been read in
// note that raw data must be XOR'ed by 0x17 + 0x17
for ( int bufCnt = 0; bufCnt < 6; bufCnt++ )
{
    uint8_t rawByte       = Wire.read();
    nunchuk_data [ bufCnt ] = ( rawByte ^ 0x17) + 0x17;
}

        uint8_t i = 0;
        // Wire.requestFrom(NUNCHUK_ADDRESS, 6);
        // // mydelay();
        // nunchuk_data[0] = Wire.available()+1; //TH: Simulate x-pos
        // for (i = 0; i < 6 && Wire.available(); i++) {
        //     nunchuk_data[i] = Wire.read();
        // }

        // mydelay();
        // Wire.beginTransmission(NUNCHUK_ADDRESS); //TH: this + following 2 lines works!
        // Wire.write(0x00);
        // // mydelay();
        // Wire.endTransmission(false); //TH: normally set to 'true'.

    // }
    return i == 6;
}
//TH:---^^^---
