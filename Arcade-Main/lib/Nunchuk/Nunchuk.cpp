/*
 VGA Arcade [Nunchuk.cpp] - for 1TE663 project - Version 20230227

 Created separate cpp-file to avoid code i h-file.

 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)
*/

#include "Nunchuk.h"

//TH:---vvv---
void NunChukData::nunchuk_init() {
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xF0); //TH: Black Nunchuk
    I2C_WRITE(0x55); //TH: Black Nunchuk
    I2C_STOP();
    mydelay();
    
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xFB); //TH: Black Nunchuk
    I2C_WRITE(0x00); //TH: Black Nunchuk
    I2C_STOP();
    mydelay();

    // I2C_START(NUNCHUK_ADDRESS); // Is this wrong? Encryption?
    // I2C_WRITE(0x40); //TH: White Nunchuk 0x40
    // I2C_WRITE(0x00); //TH: White Nunchuk 0x00
    // I2C_STOP();
    // mydelay();

    // I2C_START(NUNCHUK_ADDRESS);
    // I2C_WRITE(0x00); //TH: White Nunchuk
    // I2C_STOP();
    // mydelay();
}

// Read a full chunk of data from Nunchuk. @return A boolean if the data transfer was successful
uint8_t NunChukData::nunchuk_read() {
    nunchuk_data[5] = 1;
    PORTC ^= 1; //TH: Toggle portA bit 0
    uint16_t no1 = 0; // Used as nameless temp-var to save RAM. (small loops, reading wheelPosition)

    uint8_t i;
    // Wire.requestFrom(NUNCHUK_ADDRESS, 6);
    // for (i = 0; i < 6 && Wire.available(); i++) {
    //     nunchuk_data[i] = I2C_READ();
    // }
    mydelay();
    // I2C_START(NUNCHUK_ADDRESS);
    // I2C_WRITE(0x00);
    // I2C_STOP();
    mydelay();

    return i == 6;
}

void mydelay() {
    int n;
    for (n = 1; n < 4000; ++n) { //TH: Count to 4000 ?
    }
}
//TH:---^^^---
