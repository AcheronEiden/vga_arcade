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
//#include <MyNunchuk.h> //TH:Switched to external struct
//#include <math.h> //TH: fixes error: 'atan2' was not declared in this scope
//#include <VGAX.h> //TH: To use the VGA delay-routine
//VGAX vga2; //TH: To use the VGA delay-routine

// Calibration accelerometer values, depends on your Nunchuk
#define NUNCHUK_ACCEL_X_ZERO 512
#define NUNCHUK_ACCEL_Y_ZERO 512
#define NUNCHUK_ACCEL_Z_ZERO 512

// Calibration joystick values
#define NUNCHUK_JOYSTICK_X_ZERO 127
#define NUNCHUK_JOYSTICK_Y_ZERO 128

// Whether to disable encryption. Enabling encryption means that every packet must be decrypted, which wastes cpu cycles. Cheap Nunchuk clones have problems with the encrypted init sequence, so be sure you know what you're doing
#define NUNCHUK_DISABLE_ENCRYPTION

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

//TH:---vvv--- A public class to be able to pass data between different cpp-files
uint8_t nunchuk_data[6] = {}; //TH:(globally accessible) array of Nunchuk data

class NunChukData {
public:
    uint8_t* getNunChukData() { //TH: Return the Nunchuk data array
        return nunchuk_data;
    }

    void nunchuk_init() {
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

        // I2C_START(NUNCHUK_ADDRESS);
        // I2C_WRITE(0x40); //TH: White Nunchuk 0x40
        // I2C_WRITE(0x00); //TH: White Nunchuk 0x00
        // I2C_STOP();
        // vga2.delay(1);
        // I2C_START(NUNCHUK_ADDRESS);
        // I2C_WRITE(0x00); //TH: White Nunchuk
        // I2C_STOP();
        // vga2.delay(1);
    }

    // Read a full chunk of data from Nunchuk. @return A boolean if the data transfer was successful
    uint8_t nunchuk_read() {
        uint8_t i;
        Wire.requestFrom(NUNCHUK_ADDRESS, 6);
        for (i = 0; i < 6 && Wire.available(); i++) {
            nunchuk_data[i] = I2C_READ();
        }
        I2C_START(NUNCHUK_ADDRESS);
        I2C_WRITE(0x00);
        I2C_STOP();
        return i == 6;
    }

    void mydelay() {
        int n;
        for (n = 1; n < 4000; ++n) { //TH: Count to 4000 ?
        }
    }

}; //TH:---^^^---

// uint8_t nunchuk_data[6]; //TH:Switched to external struct
// uint8_t nunchuk_cali[16]; //TH: Verkar inte behövas
//struct MyNunchukd_data myData; //TH:Switched to external struct
//TH: Define variables as external
//extern struct nunchuk_data nunchuk_extdata;
//extern nunchuk_data nunchuk_extdata;

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) // Only Arduino UNO
/**
 * Use normal analog ports as power supply, which is useful if you want to have all pins in a row
 * Like for the famous WiiChuck adapter
 * @see https://todbot.com/blog/2008/02/18/wiichuck-wii-nunchuck-adapter-available/
 */
//TH:Removed routine "void nunchuk_init_power"
// static void nunchuk_init_power() {
//     // Add power supply for port C2 (GND) and C3 (PWR)
//     PORTC &= ~_BV(PORTC2);
//     PORTC |= _BV(PORTC3);
//     DDRC |= _BV(PORTC2) | _BV(PORTC3);
// //    delay(100); //TH: Removed
// }
#endif

// static void mydelay() {
//     int n;
//     for (n = 1; n < 4000; ++n) { //TH: Count to 4000 ?
//     }
// }

/**
 * Initializes the Nunchuk communication by sending a sequence of bytes
 */
// static void nunchuk_init() {

// #ifdef NUNCHUK_DISABLE_ENCRYPTION
//     I2C_START(NUNCHUK_ADDRESS);
//     I2C_WRITE(0xF0); //TH: Black Nunchuk
//     I2C_WRITE(0x55); //TH: Black Nunchuk
//     I2C_STOP();
//     mydelay();
    
//     I2C_START(NUNCHUK_ADDRESS);
//     I2C_WRITE(0xFB); //TH: Black Nunchuk
//     I2C_WRITE(0x00); //TH: Black Nunchuk
//     I2C_STOP();
//     mydelay();
//     //vga2.delay(10);

//     // I2C_START(NUNCHUK_ADDRESS);
//     // I2C_WRITE(0x40); //TH: White Nunchuk 0x40
//     // I2C_WRITE(0x00); //TH: White Nunchuk 0x00
//     // I2C_STOP();
//     // vga2.delay(1);
//     // I2C_START(NUNCHUK_ADDRESS);
//     // I2C_WRITE(0x00); //TH: White Nunchuk
//     // I2C_STOP();
//     // vga2.delay(1);
// #else
//     I2C_START(NUNCHUK_ADDRESS);
//     I2C_WRITE(0x40);
//     I2C_WRITE(0x00);
//     I2C_STOP();
//     // delay(1);
// #endif

// #ifdef NUNCHUK_DEBUG
//     Serial.print("Ident: "); // 0xA4200000 for Nunchuck, 0xA4200101 for Classic, 0xA4200402 for Balance

//     I2C_START(NUNCHUK_ADDRESS);
//     I2C_WRITE(0xFA);
//     I2C_STOP();

//     Wire.requestFrom(NUNCHUK_ADDRESS, 6);
//     for (uint8_t i = 0; i < 6; i++) {
//         if (Wire.available()) {
//             Serial.print(I2C_READ(), HEX);
//             Serial.print(" ");
//         }
//     }
//     I2C_STOP();
//     Serial.println("");

// //    delay(100); // Wait for serial transfer, before loop()ing
//     mydelay();
// //    vga2.delay(100); //TH: Replaced delay routine
// #endif

// }


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
//  * Retrieves the raw X-value of the joystick
//  */
// static uint8_t nunchuk_joystickX_raw() {
//     return nunchuk_data[0];
// }

// /**
//  * Retrieves the raw Y-value of the joystick
//  */
// static uint8_t nunchuk_joystickY_raw() {
//     return nunchuk_data[1];
// }

// /**
//  * Retrieves the calibrated X-value of the joystick
//  */
// static int16_t nunchuk_joystickX() {
//     return (int16_t) nunchuk_joystickX_raw() - (int16_t) NUNCHUK_JOYSTICK_X_ZERO;
// }

// /**
//  * Retrieves the calibrated Y-value of the joystick
//  */
// static int16_t nunchuk_joystickY() {
//     return (int16_t) nunchuk_joystickY_raw() - (int16_t) NUNCHUK_JOYSTICK_Y_ZERO;
// }

// /**
//  * Calculates the angle of the joystick
//  */
// static float nunchuk_joystick_angle() {
// //    return atan2((float) nunchuk_joystickY(), (float) nunchuk_joystickX());
//     return 0;
// }

// /**
//  * Retrieves the raw X-value of the accelerometer
//  */
// static uint16_t nunchuk_accelX_raw() {
//     return ((uint16_t) myData.nunchuk_data[2] << 2) | ((myData.nunchuk_data[5] >> 2) & 3); //TH:Switched to external struct
// }

// /**
//  * Retrieves the raw Y-value of the accelerometer
//  */
// static uint16_t nunchuk_accelY_raw() {
//     return ((uint16_t) myData.nunchuk_data[3] << 2) | ((myData.nunchuk_data[5] >> 4) & 3); //TH:Switched to external struct
// }

// /**
//  * Retrieves the raw Z-value of the accelerometer
//  */
// static uint16_t nunchuk_accelZ_raw() {
//     return ((uint16_t) myData.nunchuk_data[4] << 2) | ((myData.nunchuk_data[5] >> 6) & 3); //TH:Switched to external struct
// }

// /**
//  * Retrieves the calibrated X-value of the accelerometer
//  */
// static int16_t nunchuk_accelX() {
//     return (int16_t) nunchuk_accelX_raw() - (int16_t) NUNCHUK_ACCEL_X_ZERO;
// }

// /**
//  * Retrieves the calibrated Y-value of the accelerometer
//  */
// static int16_t nunchuk_accelY() {
//     return (int16_t) nunchuk_accelY_raw() - (int16_t) NUNCHUK_ACCEL_Y_ZERO;
// }

// /**
//  * Retrieves the calibrated Z-value of the accelerometer
//  */
// static int16_t nunchuk_accelZ() {
//     return (int16_t) nunchuk_accelZ_raw() - (int16_t) NUNCHUK_ACCEL_Z_ZERO;
// }

// /**
//  * Calculates the pitch angle THETA around y-axis of the Nunchuk in radians
//  */
// static float nunchuk_pitch() { // tilt-y
// //    return atan2((float) nunchuk_accelY(), (float) nunchuk_accelZ());
//     return 0;
// }

// /**
//  * Calculates the roll angle PHI around x-axis of the Nunchuk in radians
//  */
// static float nunchuk_roll() { // tilt-x
// //    return atan2((float) nunchuk_accelX(), (float) nunchuk_accelZ());
//     return 0;
// }

// /**
//  * A handy function to print either verbose information of the Nunchuk or a CSV stream for Processing
//  */
// static void nunchuk_print() {

// #ifdef NUNCHUK_DEBUG
//     char buf[100];

//     sprintf(buf, "Joy X=%4d Y=%4d   Acc X=%4d Y=%4d Z=%4d   Btn Z=%1d C=%1d\n",
//         nunchuk_joystickX(), nunchuk_joystickY(),
//         nunchuk_accelX(), nunchuk_accelY(), nunchuk_accelZ(),
//         nunchuk_buttonZ(), nunchuk_buttonC());
//     Serial.print(buf);
// #else
// //TH: Removed below because 'Serial' was not declared in this scope
//     // Serial.print(nunchuk_joystickX(), DEC);
//     // Serial.print(",");
//     // Serial.print(nunchuk_joystickY(), DEC);
//     // Serial.print(",");
//     // Serial.print(nunchuk_accelX(), DEC);
//     // Serial.print(",");
//     // Serial.print(nunchuk_accelY(), DEC);
//     // Serial.print(",");
//     // Serial.print(nunchuk_accelZ(), DEC);
//     // Serial.print(",");
//     // Serial.print(nunchuk_buttonZ(), DEC);
//     // Serial.print(",");
//     // Serial.print(nunchuk_buttonC(), DEC);
//     // Serial.print("\n");
// #endif
// }

#endif
