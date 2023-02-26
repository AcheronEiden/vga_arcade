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

// Define data structure
struct MyNunchukd_data {
    uint8_t nunchuk_data[6];
//    uint8_t nunchuk_cali[16];
};

// Define variable as external
extern struct MyNunchukd_data myData;

#endif