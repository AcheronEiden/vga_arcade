/*
 VGA Arcade [Nunchuk.cpp] - for 1TE663 project - Version 20230227

 Created separate cpp-file to avoid code i h-file.

 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)
*/

#include "Nunchuk.h"

//TH:---vvv---
void NunChukData::mydelay() {
    int n;
    for (n = 1; n < 100; ++n) { //TH: Count to 100 gives 700us delay
        PORTC ^= 1; //TH: Toggle portA bit 0 // Just to DO something
    }
}

uint8_t* NunChukData::getNunChukData() { //TH: Return the Nunchuk data array
    return nunchuk_data;
}

void NunChukData::nunchuk_init() {
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xF0); //TH: Black Nunchuk
    I2C_WRITE(0x55); //TH: Black Nunchuk
    I2C_STOP();
    // mydelay();
    
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xFB); //TH: Black Nunchuk
    I2C_WRITE(0x00); //TH: Black Nunchuk
    I2C_STOP();
    // mydelay();

    // I2C_START(NUNCHUK_ADDRESS); // Is this wrong? Encryption?
    // I2C_WRITE(0x40); //TH: White Nunchuk 0x40
    // I2C_WRITE(0x00); //TH: White Nunchuk 0x00
    // I2C_STOP();

    // I2C_START(NUNCHUK_ADDRESS);
    // I2C_WRITE(0x00); //TH: White Nunchuk
    // I2C_STOP();
}

// Read a full chunk of data from Nunchuk. @return A boolean if the data transfer was successful
uint8_t NunChukData::nunchuk_read() {
    uint8_t i = 0;
    if (nunchuk_data[8]==0) {
        PORTC ^= 2; //TH: DEBUG Toggle portC bit 1 (signal A1)
        nunchuk_data[8] = 1; //TH: Set busy flag

        nunchuk_data[5] = 1; //TH: Simulate a Release of button Z
        // uint16_t no1 = 0; // Used as nameless temp-var to save RAM. (small loops, reading wheelPosition)

        // Wire.requestFrom(NUNCHUK_ADDRESS, 6);
        // for (i = 0; i < 6 && Wire.available(); i++) {
        //     nunchuk_data[i] = I2C_READ();
        // }
        // mydelay();
        I2C_START(NUNCHUK_ADDRESS);
        // I2C_WRITE(0x00);
        // I2C_STOP();
        // mydelay();
        // nunchuk_data[7] = 0; //TH: Clear busy flag

    }
    return i == 6;
}
//TH:---^^^---
