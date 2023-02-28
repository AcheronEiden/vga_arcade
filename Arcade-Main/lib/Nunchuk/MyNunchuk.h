/*
 MyNunchuk.h - Version 20230214

 Gives access to an external variable containing Nunchuk data.
 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)

To use, include in all your other cpp-files that needs the data structure:
 #include <MyNunchuk.h> //TH:Switched to external struct

Also declare 
 struct MyNunchukd_data myData; //TH:Switched to external struct

 Nu är din datastruktur en extern variabel som är tillgänglig för alla filer som inkluderar header-filen.
 Du kan åberopa variabeln genom att använda namnet myData i de filer som inkluderar header-filen.

*/

#ifndef MYNUNCHUK_H
#define MYNUNCHUK_H

/*
The extern keyword is used to declare a variable or function that is defined in a different translation unit or file.

When you declare a variable or function as extern, you are telling the compiler that the definition of that variable or function is located elsewhere, and that the compiler should not reserve any memory for it or generate any code for it. Instead, it is the linker's responsibility to find the definition of the variable or function and link it with the rest of the program.

In the case of extern uint8_t nunchuk_data[6];, the extern keyword is telling the compiler that the definition of the variable nunchuk_data is located in another translation unit, and that the linker should be responsible for resolving the reference to this variable.
*/
extern uint8_t nunchuk_data[7]; //TH: 0-6 read från Nunchuck. 7=Busy Flag.
    // uint8_t nunchuk_data[6] = {}; //TH:(globally accessible) array of Nunchuk data

// // Define data structure
// struct MyNunchukd_data {
//     uint8_t nunchuk_data[6];
// //    uint8_t nunchuk_cali[16];
// };

// // Define variable as external
// extern struct MyNunchukd_data myData;

#endif

// // data.h

// #ifndef DATA_H
// #define DATA_H

// extern uint8_t nunchuk_data[6];

// #endif
