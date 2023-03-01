/*
 VGA Arcade [main.cpp] - for 1TE663 project - Version 20230224

 Based on "Arduino Breakout for VGAx" by Roberto Melzi.
 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)
*/

#include <avr/io.h> //TH: To set IO pins using C
#include <math.h>
#include <Arduino.h>
// #include <MyNunchuk.h> //TH: External data structure to share Nunchuk data
//#include <Nunchuk.h> //TH: To use Wii-controller, uses 35B RAM
//#include <Wire.h> //TH: To use Wii-controller, uses 182B RAM
//#include <I2C.h> //TH: To use Wii-controller, uses 2059-1884B= 175B RAM
//TH: Setup Nunchuk data structure
// uint8_t nunchuk_data[7] = {};
//NunChukData TheNunchuk; // create an instance called 'TheNunchuk' of the class 'NunChukData'.
//uint8_t* nunchuk_data_ptr = TheNunchuk.getNunChukData(); //TH: get a pointer to the array (NOT NEEDED WHEN USING "MyNunchuk.h")

// Try 2
// #include <Nunchuck.h> //TH: To use Wii-controller, uses 225B RAM
// NunchuckInterface TheNunchuk2; // create an instance called 'TheNunchuk2' of the class 'NunchuckInterface'.

// Try 3
#include <Nunchuk.h> //TH: To use Wii-controller, uses 225B RAM

#include <VGAX.h>
#define FNT_NANOFONT_HEIGHT 6
#define FNT_NANOFONT_SYMBOLS_COUNT 95
#include <VGAFont4x6.h>
#include <VGAXUtils.h>

VGAX vga;
VGAXUtils vgaU;
//TH: Removed font to a separate file to make this source code easier to read.
static const char str0[] PROGMEM="0"; 
static const char str1[] PROGMEM="1"; 
static const char str2[] PROGMEM="2"; 
static const char str3[] PROGMEM="3"; 
static const char str4[] PROGMEM="4"; 
static const char str5[] PROGMEM="5"; 
static const char str6[] PROGMEM="6"; 
static const char str7[] PROGMEM="7"; 
static const char str8[] PROGMEM="8"; 
static const char str9[] PROGMEM="9"; 
static const char str10[] PROGMEM="#"; 

static const char str14[] PROGMEM="Level";  
static const char str15[] PROGMEM="Score"; 
static const char str16[] PROGMEM="VGA Arcade"; //TH:
static const char str17[] PROGMEM="1TE663";     //TH:
static const char str18[] PROGMEM="Game Over!!!"; 

#define BUTTON_PIN A2          //TH: Digital A2 (Button on prototype board closest to "T 10")
//#define SOUND_PIN A4           //TH: Digital A4 (Button on prototype board closest to "T 0")
#define WHEEL_PIN 3            //TH: Analog

                               //TH:---vvv---
                               // Define various ADC prescaler
const unsigned char PS_2 = (1 << ADPS0);
const unsigned char PS_4 = (1 << ADPS1);
const unsigned char PS_8 = (1 << ADPS1 | 1 << ADPS0);
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
                               //TH:---^^^--- 
// ************* Variable definition ***************************************
boolean buttonStatus = 0;
// boolean button2Status = 0; //TH: Added for sound control on/off
uint8_t n; //TH: Used as nameless temp-var to save RAM. (small loops, reading wheelPosition)
uint16_t m = 0; //TH: Used to skip calls to nunchuk read to save process time
char s[]="000";

struct MyFlags { //TH: define struct to hold boolean that only occupy ONE BIT each to save RAM
  uint8_t cancelSafe : 1;
  uint8_t beginning : 1;
  uint8_t color : 2;
  uint8_t flag4 : 1;
};
MyFlags flags; // Declare an object of type MyFlags called flags

//int wheelPosition; 
uint8_t padPosition; 
uint8_t padPositionOld; 
// const int PadLenght = 6; 
#define PadLenght 6 
uint8_t ballX = 70;
uint8_t ballY = 30; 
uint8_t ballXold;
uint8_t ballYold; 
float ballFX = 70.;
float ballFY = 30.;
float speedX = .03/1.414;
float speedY = .03/1.414;
float speedT = .03; 
float angle; 
float angleDeg = 45.; 
uint8_t lenght = 5; // **************** real brick lenght = lenght*2 + 1 
uint8_t gameStep = 0; 
uint8_t hitScore = 0; 
uint8_t width = 100;
uint8_t height = 60; 
uint8_t lives = 9; 
uint8_t distanceX;
uint8_t distanceY; 
uint8_t distanceXmin;
uint8_t distanceYmin; 
uint8_t iDel;
uint8_t jDel; 
uint8_t rowBricks = 7; 
uint8_t ballXhit; 
uint8_t ballYhit; 
const float pi = 3.1415296;
uint8_t nBricks = 0;
// uint8_t color; 
const float speedIncrement = 1.25992105; 
uint8_t beginning = 0; 

//--- SOUND ------------------------------------------
void toneL(int frequency, int duration) {
   if (1==1) { //TH: Mute if wanted
      vga.tone(frequency);
      vga.delay(duration);
      vga.noTone();
   }
}

void setup() {
   //TH: Setup and init Nunchuk (TRY 3)
   Wire.begin(); //TH: To use Wii-controller
   Wire.setClock(400000); //TH: Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
   // Wire.setWireTimeout(800, false); //TH: 'false' gives data on bus from Nunchuk, 'true' does not.
   nunchuk_init(); //TH: Init the nunchuk

   //TH: Assign values to bit fields
   flags.cancelSafe = 0;
   flags.beginning = 0;

   //TH: Setup ports using raw C++ punch
   DDRB  |= 0b00000001; //TH: DEBUG Set    portB bit 0 (Extra debug, "röd över") 1=OUTPUT
   PORTB |= 0b00000001; //TH: DEBUG Set    portB bit 0 (Extra debug, "röd över")

//    DDRB  = 0b00000011;          // B-pins DIR   0=inputs   PB1/D9=VSYNC, D8=VSYNC MONITOR
//    PORTB = 0b00000000;          // B-pins where 1=Pullups  

//    DDRC  = 0b00110011;          // C-pins DIR   0=inputs   PC0=Buzzer, PC1=DEBUG, PC4=SDA, PC5=SCL
//    PORTC = 0b00000000;          // C-pins where 1=Pullups
// // PINC; // C-pins reads value

//    DDRD  = 0b11101000;          // D-pins DIR   0=inputs   D3=HSYNC, D6-D7=R+G
//    PORTD = 0b00000000;          // D-pins where 1=Pullups

//                                  // set up the ADC
//    ADCSRA &= ~PS_128;           // remove bits set by Arduino library
//    ADCSRA |= PS_4;              // set our own prescaler to 4. N.B. PS_2 does not work!!!

   vga.begin();
   randomSeed(1);               //TH:---vvv--- Using gcc-variables

    // Initialize I2C library manually
   //  I2c.begin();
   //  I2c.timeOut(500);
   //  I2c.pullup(false);
// vga.tone(500);
// vga.delay(500);
// vga.tone(1600);
// vga.delay(500);

// toneL(200, 300);
// toneL(100, 300);
// toneL(150, 500);

//Serial.begin(115200);

   //TH: Setup and init Nunchuk (TRY 2)
   // TheNunchuk2.begin();

   //TH: Setup and init Nunchuk (SLASK DATA)
   // NunChukData nunchuk; // create an instance called 'nunchuk' of the class 'NunChukData'.
   // uint8_t* nunchuk_data_ptr = nunchuk.getNunChukData(); // get a pointer to the array
   // uint8_t first_byte_method1 = *data_ptr; // get the first byte of the array
   // uint8_t second_byte_method1 = *(data_ptr + 1); // get the second byte of the array
   // uint8_t first_byte_method2 = data_ptr[0]; // get the first byte of the array

   //TH: Init Nunchuk (ALMOST WORKED)
   // Wire.begin(); //TH: To use Wii-controller
   // // Wire.setClock(100000); //TH: Change TWI speed for nuchuk, which uses TWI (100kHz) 7 bytes= ~700us read time
   // Wire.setClock(400000); //TH: Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
   // // Wire.setTimeout(1000); //TH: from 'void Stream' ??
   // Wire.setWireTimeout(800, false); //TH: 'false' gives data on bus from Nunchuk, 'true' does not.
   // TheNunchuk.nunchuk_init(); //TH: Init the nunchuk

   // Clear Nunchuck array
   // for (n = 0; n < 7; n++) {nunchuk_data[n] = 0;}

   //TH: Test if I2C are reacting
   // for (n = 1; n < 200000; ++n) {
// PORTC ^= 2; //TH: Toggle portC bit 1 (signal A1)
//       PORTC &= 0b11101111; // AND to zero bit 4 (SDA)
// //      PORTC = (0b00010000 << 1);
// //      PORTB = 1 ; // Red   00000001
      // vga.delay(1);
   // }

   //TH: Setup and init Nunchuk (TRY 3)
   Wire.begin(); //TH: To use Wii-controller
   Wire.setClock(400000); //TH: Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
   // Wire.setWireTimeout(800, false); //TH: 'false' gives data on bus from Nunchuk, 'true' does not.
                               //TH:---^^^--- 
}

void parameterUpdate() {
  angle = angleDeg/180* pi; 
  if (speedX > 0) {speedX = sin(angle)*speedT;} else {speedX = -sin(angle)*speedT;} 
  if (speedY > 0) {speedY = cos(angle)*speedT;} else {speedY = -cos(angle)*speedT;} 
}

void processInputs() {
//   NunChukData TheNunchuk; // create an instance called 'TheNunchuk' of the class 'NunChukData'.

   padPositionOld = padPosition;

   //TH: Use Wii-controller if present, otherwise use analog potentiometer
   n=0; //TH: If nunchuk read fails, set idle position
   m++;

   if (m>100) {

   //TH: Wait to enter safe zone
   // if ((PORTC & (1 << 1))) {
   //    asm("nop");
   //    PORTC &= 0b11011111; //TH: DEBUG Reset  portC bit 4 (signal SCL)
   // }
   // PORTC &= 0b11011111; //TH: DEBUG Reset  portC bit 5 (signal SCL)
   // PORTC |= 0b00100000; //TH: DEBUG Set    portC bit 5 (signal SCL)
   PORTC ^= 0b00100000; //TH: DEBUG Toggle portC bit 5 (signal SCL)
   PORTC &= 0b11101111; //TH: DEBUG Reset  portC bit 4 (signal SDA)
   PORTC |= 0b00010000; //TH: DEBUG Set    portC bit 4 (signal SDA)

   DDRB  |= 0b00000001; //TH: DEBUG Set    portB bit 0 (Extra debug, "röd över") 1=OUTPUT
   PORTB ^= 0b00000001; //TH: DEBUG Toggle portB bit 0 (Extra debug, "röd över")

   // if (PORTC & (1 << 1)) { //TH: performs a bitwise AND operation between the PORTC register and a mask consisting of a 1 bit shifted left 1 positions, effectively isolating the bit 1 of the PORTC register. The resulting value will be non-zero if the bit is set and zero if the bit is clear.

   // NunChukData nunchuk; // create an instance called 'nunchuk' of the class 'NunChukData'.
   // uint8_t* nunchuk_data_ptr = nunchuk.getNunChukData(); // get a pointer to the array
   // uint8_t first_byte_method1 = *data_ptr; // get the first byte of the array
   // uint8_t second_byte_method1 = *(data_ptr + 1); // get the second byte of the array
   // uint8_t first_byte_method2 = data_ptr[0]; // get the first byte of the array

   // TRY 3
   // if ( nunchuk_read() ) {
   //    buttonStatus = (nunchuk_buttonZ());
   //    n = nunchuk_joystickX();
   // }
   //TH: int mappedValue = map(value, fromLow, fromHigh, toLow, toHigh);
   padPosition = map(n, 0, 255, 1+PadLenght, width-1-PadLenght);

   // TRY 2
   // TheNunchuk2.update();
   // n=TheNunchuk2.getXStick()+2;
   // buttonStatus = 1;

      //TH: get the Nunchuk data, ALMOST WORKED
   //   uint8_t* nunchuk_data_ptr = TheNunchuk.getNunChukData();

      //TH: access the data using pointer arithmetic
//      buttonStatus = (nunchuk_data_ptr[5]) & 1; // button z
      // uint8_t joy_x = *(data + 0);
      // uint8_t joy_y = *(data + 1);
      // uint8_t accel_x = *(data + 2);
      // uint8_t accel_y = *(data + 3);
      // uint8_t accel_z = *(data + 4);
      // uint8_t button_z = (*(data + 5) >> 0) & 1;
      // uint8_t button_c = (*(data + 5) >> 1) & 1;

//      if (nunchuk_read()) {
      //   buttonStatus = (nunchuk_buttonZ()); //TH: Button Z to start ball
         // buttonStatus = 1;

//         button2Status = nunchuk_buttonC(); //TH:
      // buttonStatus = ((nunchuk_data_ptr[5]) & 1); //TH: ALMOST WORKED
      // n = nunchuk_data_ptr[0]; //TH: Joystick X-Axis [7:0] //TH: ALMOST WORKED
//         n = nunchuk_joystickX(); // Read wheelPosition
         //n = nunchuk_accelX();
         //Serial.println(n);
         // m=(n/100);

         //TH: Clear old digits
         //printSRAM(byte *fnt, byte glyphscount, byte fntheight, byte hspace, byte vspace, const char *str, char dx, char dy, byte color);
         vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str10, 2,       52,      0);
         vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str10, 7,       52,      0);
         vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str10, 12,      52,      0);

         //TH: Print tre digit value
         // n=padPosition;
         s[0]=(int(n/100))+48;
         s[1]=int((n-(int(n/100))*100)/10)+48;
         s[2]=(n%10)+48;
         // s[1]=((n-m*100)/10)+48;
         // s[2]=(n%10)+48;
         //String(n).toCharArray(s,3);
         vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, s, 2, 52, 3);

   // if (!(PORTC & (1 << 1))) {} //TH: Wait to exit safe zone
   // if ((PORTC & (1 << 1))) {} //TH: Wait to exit safe zone

//      }
      m=0;
   }

   // } else {
   //    buttonStatus = !(digitalRead(BUTTON_PIN)); //TH: Changed to active low 
   //    button2Status = !(digitalRead(SOUND_PIN)); //TH: Active low, turn sound on/off 
   //    wheelPosition = analogRead(WHEEL_PIN);
   //    padPosition = map(wheelPosition, 1023, 0, 1 + PadLenght, width - 1 - PadLenght);

}

void drawDebug() {
   
};

//TH: declare prototypes needed in C99
void gameOver();
void ballCoordinates();
void drawBall(int x, int y, int col);
void cancelPad();
void drawPad();
void gameIni();
void drawBricksGrid(int n);

void drawLives() {
    const int posX = 108; //TH: new const + changed posX from 110 to 108
    vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str10, posX, 10, 0); 
    if(lives == 9) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str9, posX, 10, 3);}
    if(lives == 8) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str8, posX, 10, 3);}
    if(lives == 7) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str7, posX, 10, 3);}
    if(lives == 6) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str6, posX, 10, 3);}
    if(lives == 5) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str5, posX, 10, 3);}
    if(lives == 4) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str4, posX, 10, 3);}
    if(lives == 3) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str3, posX, 10, 3);}
    if(lives == 2) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str2, posX, 10, 3);}
    if(lives == 1) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str1, posX, 10, 3);}
    if(lives == 0) {gameOver();}
}

void ballStart(){
   ballFX = padPosition; 
   ballFY = 55; 
   ballCoordinates(); 
   ballY = 55; 
   drawBall(ballX, ballY, 2); 
   while (buttonStatus == 0){
      processInputs();
      if (padPosition != padPositionOld){
        cancelPad(); 
        drawPad();
        drawBall(ballX, ballY, 0); 
        ballFX = padPosition; 
        ballFY = 55; 
        ballCoordinates(); 
        ballY = 55; 
        drawBall(ballX, ballY, 2); 
      }
   }
}

void gameOver() {
//   if(snd == 1) {
      toneL(660, 200); 
      toneL(330, 200);
      toneL(165, 200); 
      toneL(82, 200);
//   }
   vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str18, 25, 40, 1);
   ballStart(); 
   vga.clear(0);
   gameIni(); 
}

void gameOverNonStop() {
   lives = 9; 
   drawLives(); 
}

void nextLevel() {
  gameStep++; 
  speedT = speedIncrement*speedT; 
  drawBricksGrid(gameStep); 
}

void drawMenu() {
}

void drawGameScreen() {
}

void drawBorder() { // TH:******************** total screen size: width/height = 112/60. draw_line(x0,y0,x1,y1,c)
   vgaU.draw_line(0, 0, 0, height - 1, 3);                  // Left. c=3=White, c=2=Cyan, c=1=Red
   vgaU.draw_line(0, 0, width - 1, 0, 3);                   // Top
   vgaU.draw_line(0, height - 1, width - 1, height - 1, 3); // Bottom
   vgaU.draw_line(width - 1, 0, width - 1, height, 3);      // Right
}

void drawPad() {
   vgaU.draw_line(padPosition - PadLenght, height - 2, padPosition + PadLenght, height - 2, 1);
   vgaU.draw_line(padPosition - PadLenght, height - 3, padPosition + PadLenght, height - 3, 1);
}

void cancelPad() {
   vgaU.draw_line(padPositionOld - PadLenght, height - 2, padPositionOld + PadLenght, height - 2, 0);
   vgaU.draw_line(padPositionOld - PadLenght, height - 3, padPositionOld + PadLenght, height - 3, 0);
}

void brick(int x, int y, int col) {
   vgaU.draw_line(x - lenght, y + 1, x + lenght, y + 1 , col);
   vgaU.draw_line(x - lenght, y - 1, x + lenght, y - 1 , col);
   vga.putpixel(x - lenght, y, col); 
   vga.putpixel(x + lenght - 1, y, col); 
}

void drawBricksGrid(int n) { 
  nBricks = 0; 
  if (n%3 == 0)
  {   
    for (int i = 1; i <= int(width/(2*lenght + 3) - 1); i++) {
      for (int j = 1; j < rowBricks - 2; j++) {
         flags.color = (i + j)%3 + 1; 
         brick(2 + 1 + lenght + i*2*(lenght + 1), 1 + 2 + j*2*2, flags.color); 
         if (flags.color |= 0) {nBricks++;}
      }
    }
  }
  if (n%3 == 1)
  {   
    for (int i = 0; i <= int(width/(2*lenght + 3)); i++) {
      for (int j = 0; j < rowBricks; j++) {
         flags.color = (i + j)%4 + 0; 
         brick(2 + 1 + lenght + i*2*(lenght + 1), 1 + 2 + j*2*2, flags.color); 
         if (flags.color |= 0) {nBricks++;}
      }
    }
  }
  if (n%3 == 2)
  {   
    for (int i = 0; i <= int(width/(2*lenght + 3)); i++) {
      for (int j = 0; j < rowBricks; j++) {
         flags.color = (i + j)%3 + 1; 
         brick(2 + 1 + lenght + i*2*(lenght + 1), 1 + 2 + j*2*2, flags.color); 
         if (flags.color |= 0) {nBricks++;}
      }
    }
  }
}


void searchHitBrick(int bX, int bY) { 
   distanceX = 112; //TH:Changed 120 to 112
   distanceY = 60;
   for (int i = 0; i <= int(width/(2*lenght + 3)); i++) {
      for (int j = 0; j < rowBricks; j++) {
         if (abs(2 + 1 + lenght + i*2*(lenght + 1) - bX) < distanceX){
            distanceX = abs(2 + 1 + lenght + i*2*(lenght + 1) - bX); 
            iDel = i;
         }
         if (abs(1 + 2 + j*2*2 - bY) < distanceY){
            distanceY = abs(1 + 2 + j*2*2 - bY); 
            jDel = j;
         }
      }
   }
   brick(2 + 1 + lenght + iDel*2*(lenght + 1), 1 + 2 + jDel*2*2, 0); 
   nBricks--; 
   if (nBricks == 0){nextLevel();}
}

void drawBall(int x, int y, int col) {
   vga.putpixel(x, y, col); 
   vga.putpixel(x + 1, y, col); 
   vga.putpixel(x, y + 1, col); 
   vga.putpixel(x + 1, y + 1, col);
}

void ballCoordinates() {
  ballXold = ballX; 
  ballYold = ballY; 
  
  ballFX += speedX;
  ballFY += speedY;
  
  ballX = int(ballFX); 
  ballY = int(ballFY); 
}

//TH: Moved function
void gameIni() {
  lives = 9;
  speedT = .03; 
  gameStep = 0; 
  angleDeg = 180/4; 
  parameterUpdate(); 
  drawLives(); 
  drawBorder();
  drawPad();
  drawBricksGrid(gameStep); 
}

// ********************************* Game Start ************************************* 

void loop() {
   if (flags.beginning == 0){ 
      processInputs(); 
      gameIni(); 
      ballStart(); 
      flags.beginning = 1; 
   }

   // PORTC ^= 0b00000010; //TH: DEBUG Toggle portC bit 1 (signal A1)

   processInputs(); 
 
   if (padPosition != padPositionOld) {cancelPad(); drawPad();}
  
   ballCoordinates(); 

  //TH: Fixed paranthesis around OR operand
  if ( (ballX != ballXold) | (ballY != ballYold) ) { 
     if (flags.cancelSafe == 0) {
        drawBall(ballXold, ballYold, 0); 
        flags.cancelSafe = 1; 
     }
     if (vga.getpixel(ballX, ballY) != 0) {hitScore += 1;}
     if (vga.getpixel(ballX + 1, ballY) != 0) {hitScore += 2;}
     if (vga.getpixel(ballX, ballY + 1) != 0) {hitScore += 4;}
     if (vga.getpixel(ballX + 1, ballY + 1) != 0) {hitScore += 8;}
     if (hitScore == 0) // *************************************** ball did not hit anything and can go ahead ************
     {
        drawBall(ballX, ballY, 2); 
        flags.cancelSafe = 0; 
     }
     else // ******************************* ball has hit something  ***********************
     {
        if ( (hitScore == 3) | (hitScore == 12) ) //TH: Fixed paranthesis around OR operand 
        {
           speedY = -speedY;
        }
        else
        {
           if ( (hitScore == 5) | (hitScore == 10) ) //TH: Fixed paranthesis around OR operand
           {
              speedX = -speedX;
           }
           else 
           {
              speedX = -speedX;
              speedY = -speedY;
           }
        }
        if (ballY > 55) 
        {                      //TH: Fixed paranthesis around OR operand
            if ( (vga.getpixel(ballX, ballY + 1) == 1) | (vga.getpixel(ballX + 1, ballY + 1) == 1) ) // ******* ball hits the paddle *************
           {
              angleDeg = angleDeg - 2*speedX/abs(speedX)*(padPosition - ballX); 
              if (angleDeg < 20) {angleDeg = 20;}
              if (angleDeg > 70) {angleDeg = 70;}
              parameterUpdate(); 
           } 
           else // *************** ball hits the bottom *************************
           {
              
              lives = lives - 1; 
              if (lives |= 0){
//                  if(snd == 1) {
                     toneL(110,100);
//                  }
              }
              drawLives(); 
              ballStart(); 
              flags.cancelSafe = 0; 
              speedY = - abs(speedY); 
           }
        }
        else if (ballX > 1 && ballX + 1 < width - 1 && ballY > 1) // *********** ball hits a brick *******************
        {
//            if(snd == 1) {
              toneL(440,30);
//            }
           if ( (hitScore == 1) | (hitScore == 3) ) {ballXhit = ballX; ballYhit = ballY;}          //TH: Fixed paranthesis around OR operand
           if ( (hitScore == 2) | (hitScore == 10) ) {ballXhit = ballX + 1; ballYhit = ballY;}     //TH: Fixed paranthesis around OR operand
           if ( (hitScore == 8) | (hitScore == 12) ) {ballXhit = ballX + 1; ballYhit = ballY + 1;} //TH: Fixed paranthesis around OR operand
           if ( (hitScore == 4) | (hitScore == 5) ) {ballXhit = ballX; ballYhit = ballY + 1;}      //TH: Fixed paranthesis around OR operand
           searchHitBrick(ballXhit, ballYhit); 
        }
        hitScore = 0; 
      }
   }
}
