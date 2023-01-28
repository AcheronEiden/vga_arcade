/*
 VGA Arcade - Version 20230123

 CHANGES 2022-12-21 -- 2023-01-27 FOR 1TE663 PROJECT.
 CHANGES BY TOBIAS HOLM (/TH:) AND MOHAMMED NOUR KAMALMAZ (/MK:)
*/

#include <avr/io.h> //TH: To set IO pins using C
#include <Nunchuk.h> //TH: To use Wii-controller
#include <Wire.h> //TH: To use Wii-controller


/********************************************
 * Arduino Breakout                         *
 * for VGAx                                 *
 *                                          *
 * Roberto Melzi - October the 2nd 2016     *
 *                                          *
 * Version 1.0                              *
 *                                          *
 ********************************************/
 
#include <VGAX.h>
#include <math.h>
#define FNT_NANOFONT_HEIGHT 6
#define FNT_NANOFONT_SYMBOLS_COUNT 95
#include <VGAXUtils.h>

VGAX vga;
VGAXUtils vgaU;

//data size=570 bytes
const unsigned char fnt_nanofont_data[FNT_NANOFONT_SYMBOLS_COUNT][1+FNT_NANOFONT_HEIGHT] PROGMEM={
{ 1, 128, 128, 128, 0, 128, 0, }, //glyph '!' code=0
{ 3, 160, 160, 0, 0, 0, 0, }, //glyph '"' code=1
//{ 5, 80, 248, 80, 248, 80, 0, },  //glyph '#' code=2
{ 5, 240, 240, 240, 240, 240, 240, },  //glyph '#' = 'full rectangle' code=2
{ 3, 80, 32, 112, 112, 32, 0, },  //glyph '$' = 'bomb' code=3
{ 5, 32, 40, 120, 240, 32, 32, },  //glyph '%' = 'planeL' code=4
{ 5, 96, 144, 104, 144, 104, 0, },  //glyph '&' code=5
{ 2, 128, 64, 0, 0, 0, 0, },  //glyph ''' code=6
{ 2, 64, 128, 128, 128, 64, 0, }, //glyph '(' code=7
{ 2, 128, 64, 64, 64, 128, 0, },  //glyph ')' code=8
{ 3, 0, 160, 64, 160, 0, 0, },  //glyph '*' code=9
{ 3, 0, 64, 224, 64, 0, 0, }, //glyph '+' code=10
{ 2, 0, 0, 0, 0, 128, 64, },  //glyph ',' code=11
{ 3, 0, 0, 224, 0, 0, 0, }, //glyph '-' code=12
{ 1, 0, 0, 0, 0, 128, 0, }, //glyph '.' code=13
{ 5, 8, 16, 32, 64, 128, 0, },  //glyph '/' code=14
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph '0' code=15
{ 3, 64, 192, 64, 64, 224, 0, },  //glyph '1' code=16
{ 4, 224, 16, 96, 128, 240, 0, }, //glyph '2' code=17
{ 4, 224, 16, 96, 16, 224, 0, },  //glyph '3' code=18
{ 4, 144, 144, 240, 16, 16, 0, }, //glyph '4' code=19
{ 4, 240, 128, 224, 16, 224, 0, },  //glyph '5' code=20
{ 4, 96, 128, 224, 144, 96, 0, }, //glyph '6' code=21
{ 4, 240, 16, 32, 64, 64, 0, }, //glyph '7' code=22
{ 4, 96, 144, 96, 144, 96, 0, },  //glyph '8' code=23
{ 4, 96, 144, 112, 16, 96, 0, },  //glyph '9' code=24
{ 1, 0, 128, 0, 128, 0, 0, }, //glyph ':' code=25
{ 2, 0, 128, 0, 0, 128, 64, },  //glyph ';' code=26
{ 3, 32, 64, 128, 64, 32, 0, }, //glyph '<' code=27
{ 3, 0, 224, 0, 224, 0, 0, }, //glyph '=' code=28
{ 3, 128, 64, 32, 64, 128, 0, },  //glyph '>' code=29
{ 4, 224, 16, 96, 0, 64, 0, },  //glyph '?' code=30
{ 4, 96, 144, 176, 128, 112, 0, },  //glyph '@' code=31
{ 4, 96, 144, 240, 144, 144, 0, },  //glyph 'A' code=32
{ 4, 224, 144, 224, 144, 224, 0, }, //glyph 'B' code=33
{ 4, 112, 128, 128, 128, 112, 0, }, //glyph 'C' code=34
{ 4, 224, 144, 144, 144, 224, 0, }, //glyph 'D' code=35
{ 4, 240, 128, 224, 128, 240, 0, }, //glyph 'E' code=36
{ 4, 240, 128, 224, 128, 128, 0, }, //glyph 'F' code=37
{ 4, 112, 128, 176, 144, 112, 0, }, //glyph 'G' code=38
{ 4, 144, 144, 240, 144, 144, 0, }, //glyph 'H' code=39
{ 3, 224, 64, 64, 64, 224, 0, },  //glyph 'I' code=40
{ 4, 240, 16, 16, 144, 96, 0, },  //glyph 'J' code=41
{ 4, 144, 160, 192, 160, 144, 0, }, //glyph 'K' code=42
{ 4, 128, 128, 128, 128, 240, 0, }, //glyph 'L' code=43
{ 5, 136, 216, 168, 136, 136, 0, }, //glyph 'M' code=44
{ 4, 144, 208, 176, 144, 144, 0, }, //glyph 'N' code=45
{ 4, 96, 144, 144, 144, 96, 0, }, //glyph 'O' code=46
{ 4, 224, 144, 224, 128, 128, 0, }, //glyph 'P' code=47
{ 4, 96, 144, 144, 144, 96, 16, },  //glyph 'Q' code=48
{ 4, 224, 144, 224, 160, 144, 0, }, //glyph 'R' code=49
{ 4, 112, 128, 96, 16, 224, 0, }, //glyph 'S' code=50
{ 3, 224, 64, 64, 64, 64, 0, }, //glyph 'T' code=51
{ 4, 144, 144, 144, 144, 96, 0, },  //glyph 'U' code=52
{ 3, 160, 160, 160, 160, 64, 0, },  //glyph 'V' code=53
{ 5, 136, 168, 168, 168, 80, 0, },  //glyph 'W' code=54
{ 4, 144, 144, 96, 144, 144, 0, },  //glyph 'X' code=55
{ 3, 160, 160, 64, 64, 64, 0, },  //glyph 'Y' code=56
{ 4, 240, 16, 96, 128, 240, 0, }, //glyph 'Z' code=57
{ 2, 192, 128, 128, 128, 192, 0, }, //glyph '[' code=58
{ 5, 128, 64, 32, 16, 8, 0, },  //glyph '\' code=59
{ 2, 192, 64, 64, 64, 192, 0, },  //glyph ']' code=60
{ 5, 32, 80, 136, 0, 0, 0, }, //glyph '^' code=61
{ 4, 0, 0, 0, 0, 240, 0, }, //glyph '_' code=62
{ 2, 128, 64, 0, 0, 0, 0, },  //glyph '`' code=63
{ 3, 0, 224, 32, 224, 224, 0, },  //glyph 'a' code=64
{ 3, 128, 224, 160, 160, 224, 0, }, //glyph 'b' code=65
{ 3, 0, 224, 128, 128, 224, 0, }, //glyph 'c' code=66
{ 3, 32, 224, 160, 160, 224, 0, },  //glyph 'd' code=67
{ 3, 0, 224, 224, 128, 224, 0, }, //glyph 'e' code=68
{ 2, 64, 128, 192, 128, 128, 0, },  //glyph 'f' code=69
{ 3, 0, 224, 160, 224, 32, 224, },  //glyph 'g' code=70
{ 3, 128, 224, 160, 160, 160, 0, }, //glyph 'h' code=71
{ 1, 128, 0, 128, 128, 128, 0, }, //glyph 'i' code=72
{ 2, 0, 192, 64, 64, 64, 128, },  //glyph 'j' code=73
{ 3, 128, 160, 192, 160, 160, 0, }, //glyph 'k' code=74
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph 'l' code=75
{ 5, 0, 248, 168, 168, 168, 0, }, //glyph 'm' code=76
{ 3, 0, 224, 160, 160, 160, 0, }, //glyph 'n' code=77
{ 3, 0, 224, 160, 160, 224, 0, }, //glyph 'o' code=78
{ 3, 0, 224, 160, 160, 224, 128, }, //glyph 'p' code=79
{ 3, 0, 224, 160, 160, 224, 32, },  //glyph 'q' code=80
{ 3, 0, 224, 128, 128, 128, 0, }, //glyph 'r' code=81
{ 2, 0, 192, 128, 64, 192, 0, },  //glyph 's' code=82
{ 3, 64, 224, 64, 64, 64, 0, }, //glyph 't' code=83
{ 3, 0, 160, 160, 160, 224, 0, }, //glyph 'u' code=84
{ 3, 0, 160, 160, 160, 64, 0, },  //glyph 'v' code=85
{ 5, 0, 168, 168, 168, 80, 0, },  //glyph 'w' code=86
{ 3, 0, 160, 64, 160, 160, 0, },  //glyph 'x' code=87
{ 3, 0, 160, 160, 224, 32, 224, },  //glyph 'y' code=88
{ 2, 0, 192, 64, 128, 192, 0, },  //glyph 'z' code=89
{ 3, 96, 64, 192, 64, 96, 0, }, //glyph '{' code=90
{ 1, 128, 128, 128, 128, 128, 0, }, //glyph '|' code=91
{ 3, 192, 64, 96, 64, 192, 0, },  //glyph '}' code=92
{ 3, 96, 192, 0, 0, 0, 0, },  //glyph '~' code=93
{ 4, 48, 64, 224, 64, 240, 0, },  //glyph '£' code=94
};

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
#define SOUND_PIN A4           //TH: Digital A4 (Button on prototype board closest to "T 0")
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

void setup() {

   vga.begin();
   randomSeed(1);               //TH:---vvv---
   DDRB  = 0b00000010;          // B-pins where 0=inputs
   PORTB = 0b00000000;          // B-pins where 1=Pullups
   DDRC  = 0b00100001;          // C-pins where 0=inputs
   PORTC = 0b00110100;          // C-pins where 1=Pullups
   DDRD  = 0b11101000;          // D-pins where 0=inputs
   PORTD = 0b00000000;          // D-pins where 1=Pullups
                                 // set up the ADC
   ADCSRA &= ~PS_128;           // remove bits set by Arduino library
   ADCSRA |= PS_4;              // set our own prescaler to 4. N.B. PS_2 does not work!!!
  
   Wire.begin(); //TH: To use Wii-controller
//   Wire.setClock(100000); //TH: Change TWI speed for nuchuk, which uses TWI (100kHz)
   Wire.setClock(400000); //TH: Change TWI speed for nuchuk, which uses Fast-TWI (400kHz)
   nunchuk_init(); //TH: Init the nunchuk
                               //TH:---^^^--- 
}

// ************* Variable definition ***************************************
boolean buttonStatus = 0;
boolean button2Status = 0; //TH: Added for sound control on/off
int wheelPosition; 
int padPosition; 
int padPositionOld; 
int PadLenght = 6; 
int ballX = 70;
int ballY = 30; 
int ballXold;
int ballYold; 
float ballFX = 70.;
float ballFY = 30.;
float speedX = .03/1.414;
float speedY = .03/1.414;
float speedT = .03; 
float angle; 
float angleDeg = 45.; 
int lenght = 5; // **************** real brick lenght = lenght*2 + 1 
int gameStep = 0; 
int cancelSafe = 0;
int hitScore = 0; 
int width = 100;
int height = 60; 
int lives = 9; 
int distanceX;
int distanceY; 
int distanceXmin;
int distanceYmin; 
int iDel;
int jDel; 
int rowBricks = 7; 
int ballXhit; 
int ballYhit; 
const float pi = 3.1415296;
int nBricks = 0;
int color; 
const float speedIncrement = 1.25992105; 
int beginning = 0; 
int snd = 0; //TH: 0=No sound, 1=Sound on
int sndwait = 1; //TH: 1=Sound toggled, waiting for release of button

void parameterUpdate() {
  angle = angleDeg/180* pi; 
  if (speedX > 0) {speedX = sin(angle)*speedT;} else {speedX = -sin(angle)*speedT;} 
  if (speedY > 0) {speedY = cos(angle)*speedT;} else {speedY = -cos(angle)*speedT;} 
}

void toneL(int frequency, int duration) {
   if(snd == 1) {
      vga.tone(frequency);
      vga.delay(duration);
      vga.noTone(); 
   }
}

void processInputs() {
   padPositionOld = padPosition;

   //TH: Use Wii-controller if present, otherwise use analog potentiometer
//   if (nunchuk_read()) {
   if (1==1) {
      // Work with nunchuk_data
//      nunchuk_print();
      buttonStatus = nunchuk_buttonZ();
      button2Status = nunchuk_buttonC();
      wheelPosition = nunchuk_joystickX();
      padPosition = map(wheelPosition, 1023, 0, 1 + PadLenght, width - 1 - PadLenght);
      padPosition = width/2;
   } else {
      buttonStatus = !(digitalRead(BUTTON_PIN)); //TH: Changed to active low 
      button2Status = !(digitalRead(SOUND_PIN)); //TH: Active low, turn sound on/off 
      wheelPosition = analogRead(WHEEL_PIN);
      padPosition = map(wheelPosition, 1023, 0, 1 + PadLenght, width - 1 - PadLenght);
   }

}

//TH: declare prototypes needed in C99
void gameOver();
void ballCoordinates();
void drawBall(int x, int y, int col);
void cancelPad();
void drawPad();
void gameIni();
void drawBricksGrid(int n);

void drawLives() {
    vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str10, 110, 10, 0); 
    if(lives == 9) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str9, 110, 10, 3);}
    if(lives == 8) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str8, 110, 10, 3);}
    if(lives == 7) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str7, 110, 10, 3);}
    if(lives == 6) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str6, 110, 10, 3);}
    if(lives == 5) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str5, 110, 10, 3);}
    if(lives == 4) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str4, 110, 10, 3);}
    if(lives == 3) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str3, 110, 10, 3);}
    if(lives == 2) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str2, 110, 10, 3);}
    if(lives == 1) {vga.printPROGMEM((byte*)fnt_nanofont_data, FNT_NANOFONT_SYMBOLS_COUNT, FNT_NANOFONT_HEIGHT, 3, 1, str1, 110, 10, 3);}
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

void drawBorder() { // ************************ total screen size: width/height = 120/60
   vgaU.draw_line(0, 0, 0, height - 1, 3);
   vgaU.draw_line(0, 0, width - 1, 0, 3);
   vgaU.draw_line(0, height - 1, width - 1, height - 1, 3);
   vgaU.draw_line(width - 1, 0, width - 1, height, 3);
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
   distanceX = 120;
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
  beginning = 0; //TH: Fixed mistype
  angleDeg = 180/4; 
  parameterUpdate(); 
  drawLives(); 
  drawBorder();
  drawPad();
  drawBricksGrid(gameStep); 
}

// ********************************* Game Start ************************************* 

void loop() {
  if (beginning == 0){ 
     processInputs(); 
     gameIni(); 
     ballStart(); 
     beginning = 1; 
  }
  processInputs(); 
 
  if (padPosition != padPositionOld) {cancelPad(); drawPad();}
  
  ballCoordinates(); 
  
   if (button2Status == 0){ //TH: Check if sound should be toggled on/off
      if (sndwait == 0) {
         if (snd == 0) {
            snd = 1; //TH: Temp turn on sound to indicate the sound is turned off
            toneL(800, 30);
            toneL(600, 30);
            toneL(400, 30);
            snd = 0;
         } else {
            toneL(200, 30);
            toneL(100, 30);
            toneL(150, 50);
         }
         snd = !snd;
         sndwait = 1; //TH: Wait for release of button 2
      }
   }
   if (button2Status == 1){ //TH: Wait for release of button 2
      sndwait = 0; //TH: Button 2 is released
   }

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
              cancelSafe = 0; 
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
