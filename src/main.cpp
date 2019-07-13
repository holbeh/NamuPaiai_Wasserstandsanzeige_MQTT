#include <Arduino.h>


// GxEPD_Example : test example for e-Paper displays from Waveshare and from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display,
// available on http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// The e-paper displays are available from:
//
// https://www.aliexpress.com/store/product/Wholesale-1-54inch-E-Ink-display-module-with-embedded-controller-200x200-Communicate-via-SPI-interface-Supports/216233_32824535312.html
//
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_8363&product_id=35120
// or https://www.aliexpress.com/store/product/E001-1-54-inch-partial-refresh-Small-size-dot-matrix-e-paper-display/600281_32815089163.html
//

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Wemos D1 mini
// BUSY -> D2, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V


#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
#include GxEPD_BitmapExamples
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#if defined(ESP8266)

GxIO_Class io(SPI, /*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2); // arbitrary selection of D3(=0), D4(=2), selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=D4*/ 2, /*BUSY=D2*/ 4); // default selection of D4(=2), D2(=4)

#elif defined(ESP32)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\Documents\Arduino\hardware\espressif\esp32\variants\lolin32\pins_arduino.h

GxIO_Class io(SPI, /*CS=5*/ SS, /*DC=*/ 17, /*RST=*/ 16); // arbitrary selection of 17, 16
GxEPD_Class display(io, /*RST=*/ 16, /*BUSY=*/ 4); // arbitrary selection of (16), 4

#elif defined(ARDUINO_ARCH_SAMD)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.6.19\variants\mkr1000\variant.h
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\samd\1.6.19\variants\mkrzero\variant.h

GxIO_Class io(SPI, /*CS=*/ 4, /*DC=*/ 7, /*RST=*/ 6);
GxEPD_Class display(io, /*RST=*/ 6, /*BUSY=*/ 5);

#elif defined(ARDUINO_GENERIC_STM32F103C) && defined(MCU_STM32F103C8)


GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 3, /*RST=*/ 2);
GxEPD_Class display(io, /*RST=*/ 2, /*BUSY=*/ 1);

#elif defined(ARDUINO_GENERIC_STM32F103V) && defined(MCU_STM32F103VB)


GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ PE15, /*RST=*/ PE14); // DC, RST as wired by DESPI-M01
GxEPD_Class display(io, /*RST=*/ PE14, /*BUSY=*/ PE13); // RST, BUSY as wired by DESPI-M01

#elif defined(ARDUINO_AVR_MEGA2560)

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.21\variants\mega\pins_arduino.h

// select one, depending on your CS connection
GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
//GxIO_Class io(SPI, /*CS=*/ 10, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9, CS on 10 (for CS same as on UNO, for SPI on ICSP use)

GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

#else

// for SPI pin definitions see e.g.:
// C:\Users\xxx\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.21\variants\standard\pins_arduino.h

GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

#endif

int VCC = 3.3;
int Messeingang=A0;
int R1=1000.0;
long Messwert;
float SpannungR2;
float Widerstand;

int Balkenlaenge = 0;
int Wassermenge = 50;
int Fuellmengenprozent = 99;

int links_Taste = D0;
int mitte_Taste = D1;
int rechts_Taste = D10;

void Anzeige_Standard();
void Balkenberechnung();
void Anzeige_Menue();
void Messen();

void setup()
{
  pinMode(links_Taste, INPUT_PULLUP);
  pinMode(mitte_Taste, INPUT_PULLUP);
  pinMode(rechts_Taste, INPUT_PULLUP);
  digitalWrite(links_Taste, HIGH);
  digitalWrite(rechts_Taste, HIGH);
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init(9600); // enable diagnostic output on Serial
  display.setRotation(1);
  Serial.println("setup done");
  wdt_disable();
}

void loop()
{
  Balkenberechnung();
  if (digitalRead(mitte_Taste) == LOW){
    Anzeige_Menue();
  }
  else {
   Messen();
   Anzeige_Standard();
  }
}

void Anzeige_Standard(){
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(35,7);
  display.setTextColor(GxEPD_BLACK);
  display.println("Trinkwasser");

  display.setTextSize(3);
  display.setCursor(5, 30);
  display.print("Wasser:");
  display.print(Wassermenge);
  display.print("l");

  display.setTextSize(6);
  display.setCursor(10, 80);
  display.print(Fuellmengenprozent);
  display.print("%");

  //Anzeigebalken unten
  display.drawRoundRect(0, 174, 200, 25, 5, GxEPD_BLACK);
  display.fillRoundRect(0, 174, Balkenlaenge, 25, 5, GxEPD_BLACK);
  display.update();
  delay(5000);
}

void Balkenberechnung(){
  Balkenlaenge = map(Wassermenge, 0, 80, 0, 200);
}

void Anzeige_Menue(){
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(80,7);
  display.setTextColor(GxEPD_BLACK);
  display.println("Menue");
  display.setCursor(5,30);
  display.setTextSize(2);
  display.println("Wenn Tank leer,");
  display.println("linke Taste ...");
  display.println(" ");
  display.println("Wenn Tank voll,");
  display.println("rechte Taste ...");
  display.println(" ");
  display.update();
  
  while ((digitalRead(links_Taste) == HIGH) && (digitalRead(rechts_Taste) == HIGH)){
    //wdt_reset();
    if (digitalRead(rechts_Taste) == LOW){
      Fuellmengenprozent = 0;
      delay(100);
    }
    // if (digitalRead(rechts_Taste) == LOW){
    //   Fuellmengenprozent = 100;
    //   delay(100);
    //   break;
    // }
  }
}

void Messen(){
  //5 Messungen machen und Mittelwert bilden
  Messwert=0;
  for(int i=0;i<5;i++){
    Messwert+=analogRead(Messeingang);
  }
  Messwert=trunc(Messwert/5);
  
  //Spannung berechnen
  SpannungR2=(VCC/1023.0)*Messwert;
  Serial.print("Spannung ueber R2 betraegt ");
  Serial.print(SpannungR2,2);
  Serial.println(" Volt!");
  //Berechnung: (R2 = R1 * (U2/U1))
  Widerstand=R1*(SpannungR2/(VCC-SpannungR2));
  Serial.print("Der Widerstand hat ");
  Serial.print(Widerstand,2);
  Serial.println(" Ohm.");
  Serial.println();
  delay(1000);
}




