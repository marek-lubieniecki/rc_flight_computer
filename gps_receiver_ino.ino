// GPS headers
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

//OLED headers
#include "U8glib.h"


// GPS pins
#define rxPin 3
#define txPin 4

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
SoftwareSerial GPS_serial = SoftwareSerial(rxPin, txPin);
TinyGPSPlus gps;

unsigned long previousUpdateTime = 0;



void setup() {  
  // assign default color value
  u8g.setColorIndex(1);         // pixel on
  pinMode(8, OUTPUT);
  GPS_serial.begin(9600);
  delay(2000);
}

void updateDisplay()
{
  String lat = String(gps.location.lat());
  String lng = String(gps.location.lng());
  String alt = String(gps.altitude.meters());
  String speed =  String(gps.speed.kmph());
  String course = String(gps.course.deg());
  String sats = String(gps.satellites.value()); 
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  if (gps.location.isValid()){
    u8g.drawStr(0, 22, lat.c_str());
    u8g.drawStr(0, 42, lng.c_str());
  }
  else
  {
    u8g.drawStr(0, 22, "*");
    u8g.drawStr(0, 42, "*");
  }

  if (gps.altitude.isValid()){
    u8g.drawStr(0, 62, alt.c_str());
  }
  else{
    u8g.drawStr(0, 62, "*");  
  }

  if (gps.speed.isValid())
  {
    u8g.drawStr(64, 22, speed.c_str());
  }
  else{
    u8g.drawStr(64, 22, "*");
  }

  if (gps.course.isValid())
  {
    u8g.drawStr(64, 42, course.c_str());
  }
  else{
    u8g.drawStr(64, 42, "*");
  }

  if (gps.satellites.isValid())
  {
    u8g.drawStr(64, 62, sats.c_str());
  }
  else{
    u8g.drawStr(64, 62, "No sats!");
  }
}

void loop() {
  while (GPS_serial.available()>0)
    if (gps.encode(GPS_serial.read()))

        if (millis() -  previousUpdateTime > 500)
        {
          u8g.firstPage();  
          do {
            updateDisplay();
          }while( u8g.nextPage() );
        
        previousUpdateTime = millis();
        }
}

