/*
 VGA Arcade [main.cpp] - for 1TE663 project - Version 20230306

 Based on "Arduino Breakout for VGAx" by Roberto Melzi.
 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)

 Used RAM before Nunchuck: 1773 Bytes. Used Flash before Nunchuck:  9362
 Used RAM in final code:   1970 Bytes. Used Flash in final code:   12898
*/

#include <avr/io.h> //TH: To set IO pins using C
#include <math.h>
#include <Arduino.h>
#include <Nunchuk.h> //TH: To use Wii-controller
#define SID_ADDRESS 0x0F //TH: Address to I2C-slave Arduino device
//#define BUZZER     //TH: If you want buzzer sound effects

#include <VGAX.h>
#define FNT_NANOFONT_HEIGHT 6
#define FNT_NANOFONT_SYMBOLS_COUNT 95
#include <VGAFont4x6.h>
#include <VGAXUtils.h>
#include <Main01.h> //TH: Game screen graphics
#include <Main02.h>
#include <Main03.h>
#include <Main04.h>
#include <Main05.h>
#include <Main06.h>
#include <Main07.h>
#include <Main08.h>
#include <Spin01.h> //TH: Loading graphics
#include <Spin02.h>
#include <Spin03.h>

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

static const char strEND[] PROGMEM="Game Over!!!";
static const char strPAK[] PROGMEM="PRESS Z BUTTON";
const uint8_t colors01[] = {0,2,3,1,1,1,1,1,1,1,1,1,1,1,3,2}; //TH: Color flash effect on text

                               //TH:---vvv---
                               // Define various ADC prescaler
// const unsigned char PS_2 = (1 << ADPS0);
// const unsigned char PS_4 = (1 << ADPS1);
// const unsigned char PS_8 = (1 << ADPS1 | 1 << ADPS0);
// const unsigned char PS_16 = (1 << ADPS2);
// const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
// const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
// const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
                               //TH:---^^^---

// ************* Variable definition ***************************************

// struct MyFlags { //TH: define struct to hold boolean that only occupy ONE BIT each to save RAM
//   uint8_t cancelSafe : 1;
//   uint8_t beginning : 1;
//   uint8_t buttonStatusZ : 1;
//   uint8_t buttonStatusC : 1;
//   uint8_t color : 2;
//   uint8_t state : 2;
// };
// MyFlags flags; // Declare an object of type MyFlags called flags

uint8_t padPosition;
uint8_t padPositionOld;
#define SetPadLen 6;
uint8_t PadLength = SetPadLen;
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
//uint8_t beginning = 0;
uint8_t color = 0;
uint8_t cancelSafe = 0;
uint8_t state = 5; //TH: Loading screen
uint8_t buttonStatusZ = 0;
uint8_t buttonStatusC = 0;
// uint8_t n; //TH: Used as nameless temp-var to save RAM. (small loops, reading wheelPosition)
unsigned long timeX = 0; //TH: Keep track of time passed to pause main screen
unsigned long timeY = 0; //TH: Keep track of time passed to color cycle text
unsigned long timeP = 0; //TH: Keep track of time passed to slow down inputs
uint8_t playSID = 0; //TH: Sound to play
uint8_t idx = 0; //TH: Cycle colors in main loop title screen
// uint8_t col = 0; //TH: Color used in main loop title screen
const uint8_t *colPointer = colors01; //TH: Setup pointer to color data
uint8_t frame = 0; //TH: Animation frame
uint8_t cntStar = 0; //TH: Animation delay

//--- SOUND ------------------------------------------
void toneL(int frequency, int duration) {
#ifdef BUZZER
   vga.tone(frequency);
   vga.delay(duration);
   vga.noTone();
#endif
}

//TH: Setup
void setup() {
   //TH: Setup and init Nunchuk
   Wire.begin(); //TH: To use Wii-controller
   Wire.setClock(400000); //TH: Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
   // Wire.setWireTimeout(800, false); //TH: 'false' gives data on bus from Nunchuk, 'true' does not.
   nunchuk_init(); //TH: Init the nunchuk

   //TH: Setup debug ports using raw C++ punch
   DDRB  |= 0b00000001; //TH: DEBUG Set    portB bit 0 (Extra debug, "röd över") 1=OUTPUT
   PORTB |= 0b00000001; //TH: DEBUG Set    portB bit 0 (Extra debug, "röd över")

   vga.begin();


//                                  // set up the ADC
//    ADCSRA &= ~PS_128;           // remove bits set by Arduino library
//    ADCSRA |= PS_4;              // set our own prescaler to 4. N.B. PS_2 does not work!!!

   //TH: Setup and init Nunchuk (SLASK DATA)
   // NunChukData TheNunchuk; // create an instance called 'TheNunchuk' of the class 'NunChukData'.
   // uint8_t* nunchuk_data_ptr = TheNunchuk.getNunChukData(); //TH: get a pointer to the array (NOT NEEDED WHEN USING "MyNunchuk.h")
   // uint8_t first_byte_method1 = *data_ptr; // get the first byte of the array
   // uint8_t second_byte_method1 = *(data_ptr + 1); // get the second byte of the array
   // uint8_t first_byte_method2 = data_ptr[0]; // get the first byte of the array
   // Clear Nunchuck array
   // for (n = 0; n < 7; n++) {nunchuk_data[n] = 0;}

   //TH: Setup and init Nunchuk (TRY 3)
   // Wire.begin(); //TH: To use Wii-controller
   // Wire.setClock(400000); //TH: Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
   // Wire.setWireTimeout(800, false); //TH: 'false' gives data on bus from Nunchuk, 'true' does not.
                               //TH:---^^^--- 
   timeX = vtimer+60; //TH: Player has to wait at least 1 sec
}

//TH: Calc direction of ball movement
void parameterUpdate() {
  angle = angleDeg/180* pi; 
  if (speedX > 0) {speedX = sin(angle)*speedT;} else {speedX = -sin(angle)*speedT;} 
  if (speedY > 0) {speedY = cos(angle)*speedT;} else {speedY = -cos(angle)*speedT;} 
}

//TH: Print 3-digit number on screen
void printNum(uint8_t n, uint8_t x, uint8_t y, uint8_t c, uint8_t bg) {
   static unsigned char s[]="###";
   //TH: Clear old digits
   vgaU.draw_rect(x, y, 13, 4, bg, bg);

   //TH: Print tre digit value
   s[0]=(int(n/100))+48;
   s[1]=int((n-(int(n/100))*100)/10)+48;
   s[2]=(n%10)+48;

   //String(n).toCharArray(s,3);
// vga.printSRAM(const_cast<byte*>(fnt_nanofont_data), FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, s, x, y, c);
   const char cc=c;
   vga.printSRAM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, s, x, y, cc);
}

//TH: Wait for screen blanking, Read Nunchuk, set new paddle position, send SID play command on queue
void processInputs() {
   padPositionOld = padPosition;

   if (!(PORTC & (1 << 1))) {asm volatile("nop");} //TH: Wait to exit safe zone
   // if ( (PORTC & (1 << 1))) {asm volatile("nop");} //TH: Wait to exit safe zone

   // if (timeP == vtimer) {} //TH: Slow down if this function has been called within one VBLANK
   // timeP = vtimer;
   // if ((rasterline<361)) {}

   // if ((rasterline>60)) {
  if ((blank<1) & (timeP != vtimer)) {
      timeP = vtimer;
      blank=1;
      if ( nunchuk_read() ) {
         buttonStatusZ = (nunchuk_buttonZ());
         buttonStatusC = (nunchuk_buttonC());
         uint8_t n = nunchuk_joystickX()+127;
         //TH: int mappedValue = map(value, fromLow, fromHigh, toLow, toHigh);
         padPosition = map(n, 0, 255, 1+PadLength, width-1-PadLength);
      }
      if (buttonStatusC) { //TH: Cheat mode
         padPosition = map(ballX, 0, width, 1+PadLength, width-1-PadLength);
      }
      if (playSID) {
         Wire.beginTransmission(SID_ADDRESS);
         Wire.write(playSID);             //TH: play sound
         Wire.endTransmission(true);      //TH: normally set to 'true'.
         playSID = 0;                     //TH: Prepare to receive next sound command
      }
      // printNum(padPosition, 98, 0, 3, 0); //TH: Debug to show pad position OSD
      // timeY = vga.millis()+1;
      // while (vga.millis() < timeY) {asm volatile("nop");} //TH: Wait for blanking
      // if ( (PORTC & (1 << 1))) {asm volatile("nop");} //TH: Wait to exit safe zone
      // timeY = vtimer;
      // while (vtimer == timeY) {asm volatile("nop");}
   }

};

//TH: declare prototypes needed in C99
void gameOver();
void ballCoordinates();
void drawBall(int x, int y, int col);
void cancelPad();
void drawPad();
void gameIni();
void drawBricksGrid(int n);

//TH: This routine draws score, and also runs GAME OVER routine if lives=0
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
    if(lives == 1) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str1, posX, 10, 1);}
    if(lives == 0) {gameOver();}
}

//TH: This routine resets ball pos, and waits for button Z while allowing pad to move using Nunchuk
void ballStart(){
   ballFX = padPosition; 
   ballFY = 55; 
   ballCoordinates(); //TH: This routine moves the ball coordinates according to speed vectors
   ballY = 55; 
   drawBall(ballX, ballY, 2); 
   while (buttonStatusZ == 0){
      processInputs();
      if (padPosition != padPositionOld){
        cancelPad(); 
        drawPad();
        drawBall(ballX, ballY, 0); 
        ballFX = padPosition; 
        ballFY = 55; 
        ballCoordinates(); //TH: This routine moves the ball coordinates according to speed vectors
        ballY = 55; 
        drawBall(ballX, ballY, 2); 
      }
   }
}

void gameOver() {

   toneL(660, 200); 
   toneL(330, 200);
   toneL(165, 200); 
   toneL(82, 200);

   playSID = 'C'; //TH: play sound GAME OVER
   processInputs(); //TH: Start sound

   vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, strEND, 25, 40, 1);
   // vga.delay(100);
   ballStart(); //TH: This routine resets ball pos, and waits for button Z while allowing pad to move using Nunchuk
   // vga.clear(0);
   gameIni(); //TH: This function clear screen, reset lifes/speed/angle
   state = 0;
   timeX = vtimer+60; //TH: Player has to wait at least 1 sec
}

void nextLevel() {
  gameStep++; 
  speedT = speedIncrement*speedT; 
  drawBricksGrid(gameStep); 
}

void drawMenu() {
   vga.copy((byte*)img_Main01_data);
   playSID = 'A'; //TH: play sound MAIN THEME 'A'
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
   vgaU.draw_line(padPosition - PadLength, height - 2, padPosition + PadLength, height - 2, 1);
   vgaU.draw_line(padPosition - PadLength, height - 3, padPosition + PadLength, height - 3, 1);
}

void cancelPad() {
   vgaU.draw_line(padPositionOld - PadLength, height - 2, padPositionOld + PadLength, height - 2, 0);
   vgaU.draw_line(padPositionOld - PadLength, height - 3, padPositionOld + PadLength, height - 3, 0);
}

//TH: This routine blows up the pad when ball missed
void blowUpPad() {
   long x1,x2;
   for (uint8_t n=0; n<120; n++) { //TH: Original PadLength = 6 = SetPadLen
      x1 = padPosition-((3+n)/2);
      x2 = padPosition+((3+n)/2);
      if (x1<1) {x1=1;}
      if (x2>(width-1)) {x2=width-1;}
      if (n<50) {
         vgaU.draw_line(x1, height-3, x2, height-3, n%4);
      } else {
         vgaU.draw_line(x1, height-3, x2, height-3, 0);
      }
      vgaU.draw_line(x1, height-2, x2, height-2, n%4);
      vga.delay(10);
   }
   // PadLength = SetPadLen;
   vgaU.draw_line(1, height - 2, width-1, height - 2, 0);
   vgaU.draw_line(1, height - 3, width-1, height - 3, 0);
   vgaU.draw_line(padPosition - PadLength, height - 2, padPosition + PadLength, height - 2, 1);
   vgaU.draw_line(padPosition - PadLength, height - 3, padPosition + PadLength, height - 3, 1);
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
         color = (i + j)%3 + 1; 
         brick(2 + 1 + lenght + i*2*(lenght + 1), 1 + 2 + j*2*2, color); 
         if (color |= 0) {nBricks++;}
      }
    }
  }
  if (n%3 == 1)
  {   
    for (int i = 0; i <= int(width/(2*lenght + 3)); i++) {
      for (int j = 0; j < rowBricks; j++) {
         color = (i + j)%4 + 0; 
         brick(2 + 1 + lenght + i*2*(lenght + 1), 1 + 2 + j*2*2, color); 
         if (color |= 0) {nBricks++;}
      }
    }
  }
  if (n%3 == 2)
  {   
    for (int i = 0; i <= int(width/(2*lenght + 3)); i++) {
      for (int j = 0; j < rowBricks; j++) {
         color = (i + j)%3 + 1; 
         brick(2 + 1 + lenght + i*2*(lenght + 1), 1 + 2 + j*2*2, color); 
         if (color |= 0) {nBricks++;}
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
   if (nBricks == 0){
      nextLevel();
      //playSID = 'F'; //TH: play sound NEXT LEVEL
      //processInputs(); //TH: Start sound
      Wire.beginTransmission(SID_ADDRESS);
      Wire.write('F');              //TH: play sound
      Wire.endTransmission(true);   //TH: normally set to 'true'.
      vga.delay(2000);
      speedT = .02; //TH: Ball speed .03 orginal
   }
}

void drawBall(int x, int y, int col) {
   vga.putpixel(x, y, col); 
   vga.putpixel(x + 1, y, col); 
   vga.putpixel(x, y + 1, col); 
   vga.putpixel(x + 1, y + 1, col);
}

//TH: This routine moves the ball coordinates according to speed vectors
void ballCoordinates() {
  ballXold = ballX; 
  ballYold = ballY; 
  
  ballFX += speedX;
  ballFY += speedY;
  
  ballX = int(ballFX); 
  ballY = int(ballFY); 
}

//TH: This function clear screen, reset lifes/speed/angle
void gameIni() {
   vga.clear(0); //TH: Moved screen clear here
   lives = 5; //TH: 9 lives
   speedT = .01; //TH: Ball speed .03 orginal
   gameStep = 0; 
   angleDeg = 180/4; 
   parameterUpdate();
   drawLives(); //TH: This routine draws score, and also runs GAME OVER routine if lives=0
   drawBorder();
   drawPad();
   drawBricksGrid(gameStep);
}

// *********************************** Loading ************************************** 

void loop() {
   processInputs(); //TH: Wait for screen blanking, Read Nunchuk

   if(state == 0) {
      drawMenu();
      state = 1;
      // processInputs(); //TH: Wait for screen blanking, Read Nunchuk
   }

   if ((state == 5)) {
      timeY = vtimer+3;
      while (vtimer < timeY) {asm volatile("nop");} //TH: Wait 3 frames

      blank=1;
      while (blank==1) {asm volatile("nop");} //TH: Wait for blanking

      cntStar++;
      if (cntStar%2==0) {
         frame++;
         frame=frame%3;
         switch(frame) {
            case 1:
               vga.copy((byte*)img_Spin02_data);
               break;
            case 2:
               vga.copy((byte*)img_Spin03_data);
               break;
            default:
               vga.copy((byte*)img_Spin01_data);
               break;
         }

      }

      blank=1;
      while (blank==1) {
         processInputs();
      }

      if ((buttonStatusZ == 1) & (vtimer > timeX)) {
         buttonStatusZ = 0;
         state = 1;
         playSID = 'A'; //TH: play sound MAIN THEME 'A'
         timeX = vtimer+60; //TH: Player has to wait at least 1 sec
      }

      if ((vtimer > (timeX+300))) {
         state = 1;
         playSID = 'A'; //TH: play sound MAIN THEME 'A'
         timeX = vtimer+60; //TH: Player has to wait at least 1 sec
      }
   }

// ********************************* Game Start ************************************* 

   if ((state == 1)) {
      timeY = vtimer+3;
      while (vtimer < timeY) {asm volatile("nop");} //TH: Wait 3 frames

      blank=1;
      while (blank==1) {asm volatile("nop");} //TH: Wait for blanking

      idx++;
      idx=idx%sizeof(colors01);

      cntStar++;
      if (cntStar%3==0) {
         frame++;
         frame=frame%8;
         switch(frame) {
            case 1:
               vga.copy((byte*)img_Main02_data);
               break;
            case 2:
               vga.copy((byte*)img_Main03_data);
               break;
            case 3:
               vga.copy((byte*)img_Main04_data);
               break;
            case 4:
               vga.copy((byte*)img_Main05_data);
               break;
            case 5:
               vga.copy((byte*)img_Main06_data);
               break;
            case 6:
               vga.copy((byte*)img_Main07_data);
               break;
            case 7:
               vga.copy((byte*)img_Main08_data);
               break;
            case 8:
               vga.copy((byte*)img_Main07_data);
               break;
            case 9:
               vga.copy((byte*)img_Main06_data);
               break;
            case 10:
               vga.copy((byte*)img_Main05_data);
               break;
            case 11:
               vga.copy((byte*)img_Main04_data);
               break;
            case 12:
               vga.copy((byte*)img_Main03_data);
               break;
            case 13:
               vga.copy((byte*)img_Main02_data);
               break;
            default:
               vga.copy((byte*)img_Main01_data);
               break;
         }

      }

      vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, strPAK, 22, 19, colPointer[idx]);

      blank=1;
      while (blank==1) {
         processInputs();
      }

      if ((buttonStatusZ == 1) & (vtimer > timeX)) {
         buttonStatusZ = 0;
         state = 3;
         // processInputs(); //TH: Wait for screen blanking, Read Nunchuk
         gameIni(); //TH: This function clear screen, reset lifes/speed/angle
         ballStart(); //TH: This routine resets ball pos, and waits for button Z while allowing pad to move using Nunchuk
      }

      if ((buttonStatusC == 1)) {
         playSID = 0; //TH: Load screen has no sound
         Wire.beginTransmission(SID_ADDRESS);
         Wire.write(playSID);             //TH: play sound
         Wire.endTransmission(true);      //TH: normally set to 'true'.

         processInputs(); //TH: Wait for screen blanking, Read Nunchuk
         buttonStatusC = 0;
         timeX = vtimer;
         if (vtimer == timeX) {}
         vga.delay(9);
         state = 5;
         timeX = vtimer+60; //TH: Player has to wait at least 1 sec
      }
   }

// ********************************* Main Game ************************************* 

   if (state == 3) {
      if (padPosition != padPositionOld) {cancelPad(); drawPad();}
   
      ballCoordinates(); //TH: This routine moves the ball coordinates according to speed vectors

      //TH: Fixed paranthesis around OR operand
      if ( (ballX != ballXold) | (ballY != ballYold) ) { 
         if (cancelSafe == 0) {
            drawBall(ballXold, ballYold, 0); 
            cancelSafe = 1; 
         }
         if (vga.getpixel(ballX, ballY) != 0) {hitScore += 1;}
         if (vga.getpixel(ballX + 1, ballY) != 0) {hitScore += 2;}
         if (vga.getpixel(ballX, ballY + 1) != 0) {hitScore += 4;}
         if (vga.getpixel(ballX + 1, ballY + 1) != 0) {hitScore += 8;}
         if (hitScore == 0) // *************************************** ball did not hit anything and can go ahead ************
         {
            drawBall(ballX, ballY, 2); 
            cancelSafe = 0; 
         }
         else // ******************************* ball has hit something  ***********************
         {
            if ( (hitScore == 3) | (hitScore == 12) ) //TH: Fixed paranthesis around OR operand 
            {
               speedY = -speedY;
               if (hitScore == 3) { //MK: fixed bug that played sound when hitting ground
                  playSID = 'D'; //TH: play sound HIT WALL
               }
            }
            else
            {
               if ( (hitScore == 5) | (hitScore == 10) ) //TH: Fixed paranthesis around OR operand
               {
                  speedX = -speedX;
                     playSID = 'D'; //TH: play sound HIT WALL
               }
               else 
               {
                  speedX = -speedX;
                  speedY = -speedY;
                     playSID = 'D'; //TH: play sound HIT WALL
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

                     playSID = 'B'; //TH: play sound HIT PAD
               } 
               else // *************** ball hits the bottom *************************
               {
                  
                  lives = lives - 1; 
                  if (lives != 0){ //MK: Miss one life, but not game over
                        toneL(110,100);

                        Wire.beginTransmission(SID_ADDRESS);
                        Wire.write('G');              //TH: play sound Arkanoid18 (Lost one life)
                        Wire.endTransmission(true);   //TH: normally set to 'true'.
                        blowUpPad();
                        // vga.delay(2000);
                  }
                  drawLives(); //TH: This routine draws score, and also runs GAME OVER routine if lives=0
                  ballStart(); //TH: This routine resets ball pos, and waits for button Z while allowing pad to move using Nunchuk
                  cancelSafe = 0; 
                  speedY = - abs(speedY); 
               }
            }
            else if (ballX > 1 && ballX + 1 < width - 1 && ballY > 1) // *********** ball hits a brick *******************
            {

               toneL(440,30);

               playSID = 'E'; //TH: play sound HIT BRICK

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
}
