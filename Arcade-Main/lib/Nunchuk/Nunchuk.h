/**
 * @license Nunchuk Arduino library v0.0.1 16/12/2016
 * http://www.xarg.org/2016/12/arduino-nunchuk-library/
 *
 * Copyright (c) 2016, Robert Eisele (robert@xarg.org)
 * Dual licensed under the MIT or GPL Version 2 licenses.
 **/

#ifndef NUNCHUK_H
#define NUNCHUK_H

#include <Wire.h> //TH: To use Wii-controller, uses 182B RAM
#include <MyNunchuk.h> //TH: External data structure to share Nunchuk data
//#include <math.h> //TH: fixes error: 'atan2' was not declared in this scope
//#include <VGAX.h> //TH: To use the VGA delay-routine
//VGAX vga2; //TH: To use the VGA delay-routine

// Calibration accelerometer values, depends on your Nunchuk
// #define NUNCHUK_ACCEL_X_ZERO 512
// #define NUNCHUK_ACCEL_Y_ZERO 512
// #define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
// #define NUNCHUK_JOYSTICK_X_ZERO 127
// #define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
// #define NUNCHUK_DISABLE_ENCRYPTION

// Print debug information instead of a CSV stream to the serial port
// #define NUNCHUK_DEBUG

// The Nunchuk I2C address
#define NUNCHUK_ADDRESS 0x52 //TH: Correct address is 0x52

#if ARDUINO >= 100
#define I2C_READ() Wire.read()
#define I2C_WRITE(x) Wire.write(x)
#else
#define I2C_READ() Wire.receive()
#define I2C_WRITE(x) Wire.send(x)
#endif

#define I2C_START(x) Wire.beginTransmission(x)
#define I2C_STOP() Wire.endTransmission(true)

//TH:---vvv--- A public class to be able to pass data between different cpp-files
//uint8_t nunchuk_data[6] = {}; //TH:(globally accessible) array of Nunchuk data

class NunChukData {
public:
    uint8_t* getNunChukData();      // Return the Nunchuk data array

    static void mydelay();          // Just a simple delay of some kind, about 600us.
    
    static void nunchuk_init();     // Start wire and send init commands to Nunchuk

    uint8_t nunchuk_read();         // Read a full chunk of data from Nunchuk.
                                    // @return A boolean if the data transfer was successful

    //Using the static keyword in a function declaration or definition can help reduce namespace pollution, by limiting the visibility of the function to only the source file where it is needed.
}; //TH:---^^^---



//TH: A public class to be able to pass data between different cpp-files
// class NunChukData {
// public:
// //    static uint8_t nunchuk_data[6]; //TH:Static (globally accessible) variable
//     uint8_t nunchuk_data[6]; //TH:(globally accessible) variable
//     uint8_t getNunChukData() {
//         return nunchuk_data;
//     }
//     int myVar;
//     int getMyVar() {
//         return myVar;
//     }
// };

// //TH:---vvv--- A public class to be able to pass data between different cpp-files
// //uint8_t nunchuk_data[6] = {}; //TH:(globally accessible) array of Nunchuk data

// class NunChukData {
// public:
//     uint8_t* getNunChukData() { //TH: Return the Nunchuk data array
//         return nunchuk_data;
//     }

//     void nunchuk_init() {
//         I2C_START(NUNCHUK_ADDRESS);
//         I2C_WRITE(0xF0); //TH: Black Nunchuk
//         I2C_WRITE(0x55); //TH: Black Nunchuk
//         I2C_STOP();
//         mydelay();
        
//         I2C_START(NUNCHUK_ADDRESS);
//         I2C_WRITE(0xFB); //TH: Black Nunchuk
//         I2C_WRITE(0x00); //TH: Black Nunchuk
//         I2C_STOP();
//         mydelay();

//         // I2C_START(NUNCHUK_ADDRESS); // Is this wrong? Encryption?
//         // I2C_WRITE(0x40); //TH: White Nunchuk 0x40
//         // I2C_WRITE(0x00); //TH: White Nunchuk 0x00
//         // I2C_STOP();
//         // mydelay();

//         // I2C_START(NUNCHUK_ADDRESS);
//         // I2C_WRITE(0x00); //TH: White Nunchuk
//         // I2C_STOP();
//         // mydelay();
//     }

//     // Read a full chunk of data from Nunchuk. @return A boolean if the data transfer was successful
//     uint8_t nunchuk_read() {
//         nunchuk_data[5] = 1;
//         PORTC ^= 1; //TH: Toggle portA bit 0
// uint16_t no1 = 0; // Used as nameless temp-var to save RAM. (small loops, reading wheelPosition)
//         uint8_t i;
//         // Wire.requestFrom(NUNCHUK_ADDRESS, 6);
//         // for (i = 0; i < 6 && Wire.available(); i++) {
//         //     nunchuk_data[i] = I2C_READ();
//         // }
//         mydelay();
//         // I2C_START(NUNCHUK_ADDRESS);
//         // I2C_WRITE(0x00);
//         // I2C_STOP();
//         mydelay();

//         return i == 6;
//     }

//     void mydelay() {
//         int n;
//         for (n = 1; n < 4000; ++n) { //TH: Count to 4000 ?
//         }
//     }

// }; //TH:---^^^---

// uint8_t nunchuk_data[6]; //TH:Switched to external struct
// uint8_t nunchuk_cali[16]; //TH: Verkar inte behövas
//struct MyNunchukd_data myData; //TH:Switched to external struct
//TH: Define variables as external
//extern struct nunchuk_data nunchuk_extdata;
//extern nunchuk_data nunchuk_extdata;


// /**
//  * Central function to read a full chunk of data from Nunchuk
//  *
//  * @return A boolean if the data transfer was successful
//  */
// static uint8_t nunchuk_read() {

//     uint8_t i;
//     Wire.requestFrom(NUNCHUK_ADDRESS, 6);
//     for (i = 0; i < 6 && Wire.available(); i++) {
//         nunchuk_data[i] = I2C_READ();
//     }
//     I2C_START(NUNCHUK_ADDRESS);
//     I2C_WRITE(0x00);
//     I2C_STOP();
//     return i == 6;
// }


// /**
//  * Checks the current state of button Z
//  */
// static uint8_t nunchuk_buttonZ() {
//     return (~nunchuk_data[5] >> 0) & 1;
// }

// /**
//  * Checks the current state of button C
//  */
// static uint8_t nunchuk_buttonC() {
//     return (~nunchuk_data[5] >> 1) & 1;
// //    return (~myData.nunchuk_data[5] >> 1) & 1; //TH:Switched to external struct
// }


// /**
//  * Retrieves the calibrated X-value of the joystick
//  */
// static int16_t nunchuk_joystickX() {
//     return (int16_t) nunchuk_joystickX_raw() - (int16_t) NUNCHUK_JOYSTICK_X_ZERO;
// }


// /**
//  * Retrieves the raw X-value of the accelerometer
//  */
// static uint16_t nunchuk_accelX_raw() {
//     return ((uint16_t) myData.nunchuk_data[2] << 2) | ((myData.nunchuk_data[5] >> 2) & 3); //TH:Switched to external struct
// }


#endif
