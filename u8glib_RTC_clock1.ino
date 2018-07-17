/*
based on sketch from http://bentommye.blogspot.com/2013/10/analog-lcd-clock-arduino-lcd.html
completed by Nicu FLORICA (niq_ro) - http://nicuflorica.blogspot.com/
& http://www.arduinotehniq.com
*/

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif
RTC_DS1307 rtc;
//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char daysOfTheWeek[7][12] = {"Duminica", "Luni", "Marti", "Miercuri", "Joi", "Vineri", "Sambata"};


#include "U8glib.h"
// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device
//  U8GLIB_ST7920_128X64_1X u8g(13, 11, 10); // SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
// se orginal code for your LCD!!!!!!
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 13, 2, 3, A5, 12);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
U8GLIB_ST7920_128X64_1X u8g(7, 6, 5 ,8); //Enable, RW, RS, RESET

int X = 0;
int Y = 0;
int I = 0;
int X2 = 0;
int Y2 = 0;
int X3 = 0;
int Y3 = 0;
int Timer = 0;
int Minutt = 0;
int Sekund = 0;
float Vinkel = 0;
int an = 0;
int luna = 0;
int zi = 0;
int zis = 0;

int ScreenWith = 128;
int ScreenWithC = 64;
int ScreenHeight = 64;
int ScreenHeightC = 32;
int yArray[128];
int deics = ScreenWith/4;
int deics2 = 0;

void draw(void) {
  // graphic commands to redraw the complete screen should be placed here 
  u8g.setFont(u8g_font_6x10);
 
    TegnKlokkeBg();
 //  DateTime now = rtc.now();
   TegnViser(Timer-1, 12.0, 15); // Omdreinig, Omdreiningstall / omdreining, radius 
   TegnViser(Minutt-5, 60.0, 24); // Omdreinig, Omdreiningstall / omdreining, radius  
   TegnViser(Sekund, 60-deics, 27); // Omdreinig, Omdreiningstall / omdreining, radius
 u8g.setPrintPos(2*deics + deics2, 35); 

if (zi < 10) u8g.print("0");
u8g.print(zi);
u8g.print("/");
if (luna < 10) u8g.print("0");
u8g.print(luna);
u8g.print("/");
u8g.print(an-2000);
u8g.setPrintPos(2*deics + deics2, 50); 
u8g.print(daysOfTheWeek[zis]);
u8g.setPrintPos(2*deics + deics2, 20); 
u8g.setFont(u8g_font_fur17);
if (Timer < 10) u8g.print(" ");
u8g.print(Timer);
u8g.print(":");
if (Minutt < 10) u8g.print("0");
u8g.print(Minutt);
}

void TegnViser(float Omdreining, float forhold, int Radius) {
    Vinkel = Omdreining * 2.0 * 3.1415 / forhold - 1,5707; // 12 timer blir til 2Pi  
    X2 = ScreenWithC -deics + Radius * cos(Vinkel);
    Y2 = ScreenHeightC + Radius * sin(Vinkel);
    u8g.drawLine(ScreenWithC-deics, ScreenHeightC, X2, Y2);
}

void TegnKlokkeBg() { // draw clock background
    u8g.drawCircle(ScreenWithC-deics, ScreenHeightC, 1);
    u8g.setFont(u8g_font_unifont);
    u8g.setFontPosTop();
    u8g.drawStr(55-deics, 4, "12");
    u8g.drawStr(83-deics, 25, "3");
    u8g.drawStr(60-deics, 45, "6");
    u8g.drawStr(39-deics, 25, "9");
  
    for(int TimeStrek = 0; TimeStrek<12; TimeStrek++) { // draw time lines
      Vinkel = TimeStrek / 12.0 * 2 * 3.1415;
      X2 = ScreenWithC-deics + 30 * cos(Vinkel);
      Y2 = ScreenHeightC + 30 * sin(Vinkel);
      X3 = ScreenWithC-deics + 28 * cos(Vinkel);
      Y3 = ScreenHeightC + 28 * sin(Vinkel);
      u8g.drawLine(X2, Y2, X3, Y3);
    }
}


void setup(void) {
  // flip screen, if required
  // u8g.setRot180();

  // set SPI backup if required
  //u8g.setHardwareBackup(u8g_backup_avr_spi);

  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
//  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop(void) {
 DateTime now = rtc.now();
 Timer = now.hour(), DEC;
 Minutt = now.minute(), DEC;
 Sekund = now.second(), DEC;
    Serial.print(Timer);
    Serial.print(':');
    Serial.print(Minutt);
    Serial.print(':');
    Serial.print(Sekund);
    Serial.print(" - ");
    an = now.year(), DEC;
    Serial.print(an);
    Serial.print('/');
    luna = now.month(), DEC;
    Serial.print(luna);
    Serial.print('/');
    zi = now.day(), DEC;
    Serial.print(zi);
    Serial.print(" (");
    zis = now.dayOfTheWeek();
    Serial.print(daysOfTheWeek[zis]);
    Serial.println(") ");
  
  // picture loop
  u8g.firstPage(); 
  do {
    draw();
  } while( u8g.nextPage() );
  // rebuild the picture after some delay
  delay(1000);
}
