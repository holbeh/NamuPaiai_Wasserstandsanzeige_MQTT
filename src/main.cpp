#include <Arduino.h>


#include "main.h"
#include "config.h"

void setup(){
  mytime::setup();
  pinMode(links_Taste, INPUT_PULLUP);
  pinMode(mitte_Taste, INPUT_PULLUP);
  //pinMode(rechts_Taste, INPUT_PULLUP);
  digitalWrite(links_Taste, HIGH);
  //digitalWrite(rechts_Taste, HIGH);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("setup"));
  display.init(9600); // enable diagnostic output on Serial
  display.setRotation(1);
  Serial.println(F("setup done"));
  pixels = new Adafruit_NeoPixel(numPixels, pin, pixelFormat);
  pixels->begin();
  pixels->clear();
  pixels->show();
  //wdt_disable();
  loadConfig();
  Messwert_HIGH = cfg.Messwert_HIGH;
  Messwert_LOW = cfg.Messwert_LOW;
  Serial.println(F(""));
  Serial.println(F("Hier die Daten aus dem EEPRom"));
  Serial.println(F(""));
  Serial.println(Messwert_LOW);
  Serial.println(Messwert_HIGH);
  Serial.println(F("ich hoffe, dass passt..."));
  Startanzeige();
  setup_wifi();
}

void loop(){
  Messen();
  all_mapping();
  pixels->clear();
  pixels->show();
  if (digitalRead(links_Taste) == LOW){
    delay(200);
    LEDs = !LEDs;
  }
  if (digitalRead(mitte_Taste) == LOW){
    delay(200);
    Anzeige_Menue();
  }
  else {
   if (LEDs == true){
   Anzeige_LED();
   }
   if (ePaperzaehler >= 20){
    ePaperzaehler = 0;
    Anzeige_Standard();
   }
  }
ePaperzaehler++;
Serial.print(F("ePaperzaehler: "));
Serial.println(ePaperzaehler);
delay(100);
}

void Anzeige_Standard(){
  
  boolean written = publish(Fuellmengenprozent);
  if (written) {
    Serial.println(F("written to mqtt"));
  } else {
    Serial.println(F("error writing to mqtt"));
  }

  char *time = mytime::isoTime();
  Serial.print(F("time: "));
  Serial.println(time);

  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(35,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(F("Trinkwasser"));

  display.setTextSize(3);
  display.setCursor(5, 30);
  display.print(F("Wasser:"));
  display.print(Wassermenge);
  display.print(F("l"));

  display.setTextSize(10);
  display.setCursor(10, 80);
  display.print(Fuellmengenprozent);
  display.print(F("%"));

  //Anzeigebalken unten
  display.drawRoundRect(0, 174, 200, 25, 5, GxEPD_BLACK);
  display.fillRoundRect(0, 174, Balkenlaenge, 25, 5, GxEPD_BLACK);
  display.update();
}



void Anzeige_Menue(){
  eraseConfig();
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(80,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(MenuString);
  display.setCursor(5,30);
  display.setTextSize(2);
  display.println(F("Wenn Tank leer,"));
  display.println(F("Auswahl-Taste"));
  display.println(F("druecken ..."));
  display.println(F(" "));
  display.update();
  
 // while ((digitalRead(links_Taste) == HIGH) && (digitalRead(rechts_Taste) == HIGH)){
   while (true){
    delay(200);

    if (digitalRead(links_Taste) == LOW){
      Messen();
      cfg.Messwert_LOW = Messwert;
      Messwert_LOW = Messwert;
      delay(100);
      Serial.print(F("Messwert_LOW: "));
      Serial.println(Messwert_LOW);
      Serial.println(F(""));
      break;
    }
  }
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(80,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(MenuString);
  display.setCursor(5,30);
  display.setTextSize(3);
  display.println(F("Danke!"));
  display.println(F("Jetzt Tank fuellen!"));
  display.println(F(" "));
  display.update();
  delay(2000);


  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(80,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(MenuString);
  display.setCursor(0,30);
  display.setTextSize(2);
  display.println("Wenn Tank voll,");
  display.println("Auswahl-Taste ");
  display.println("druecken ...");
  display.println(" ");
  display.update();

   while (true){
    delay(200);
    if (digitalRead(links_Taste) == LOW){
      Messen();
      cfg.Messwert_HIGH = Messwert;
      Messwert_HIGH = Messwert;
      delay(100);
      Serial.print(F("Messwert_High: "));
      Serial.println(Messwert_HIGH);
      Serial.println(F(""));
      break;
    }
  }
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(80,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(MenuString);
  display.setCursor(0,30);
  display.setTextSize(2);
  display.println(F("Danke!"));
  display.setTextSize(1);
  display.println(F("Daten werden jetzt"));
  display.println(F("gespeichert"));
  display.println(F("..."));
  display.update();
  delay(2000);
  saveConfig();
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(80,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(MenuString);
  display.setCursor(0,30);
  display.setTextSize(2);
  display.println(F("folgende Daten  stehen im EEPROM"));
  display.println(F(""));
  display.print(F("unterer Wert: "));
  display.println(cfg.Messwert_LOW);
  display.print(F("oberer Wert: "));
  display.println(cfg.Messwert_HIGH);
  display.update();
  ePaperzaehler = 20;
  delay(5000);
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
  Serial.print(F("Spannung ueber R2 betraegt "));
  Serial.print(SpannungR2,2);
  Serial.println(F(" Volt!"));
  //Berechnung: (R2 = R1 * (U2/U1))
  Widerstand=R1*(SpannungR2/(VCC-SpannungR2));
  Serial.print(F("Der Widerstand hat "));
  Serial.print(Widerstand,2);
  Serial.println(F(" Ohm."));
  Serial.println();
  Serial.println(Messwert);
  delay(100);
}

void Anzeige_LED(){
  for(int i=0; i<LED_Menge; i++) { // For each pixel...
    // pixels->Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels->setPixelColor(i, pixels->Color(Farbwert_rot, Farbwert_gruen, 0));
  }
pixels->show();   // Send the updated pixel colors to the hardware. 
}

void all_mapping(){
  Farbwert_gruen= map(Messwert, cfg.Messwert_LOW, cfg.Messwert_HIGH, 0, 100);
  Farbwert_rot=map(Messwert, cfg.Messwert_LOW, cfg.Messwert_HIGH, 100, 0);
  LED_Menge=map(Messwert, cfg.Messwert_LOW, cfg.Messwert_HIGH, 1,8);
  Fuellmengenprozent = map(Messwert, cfg.Messwert_LOW, cfg.Messwert_HIGH, 0, 99);
  Balkenlaenge = map(Messwert, cfg.Messwert_LOW, cfg.Messwert_HIGH, 0, 200);
  Wassermenge = map(Messwert, cfg.Messwert_LOW, cfg.Messwert_HIGH, Wassermin, Wassermax);
  if (Farbwert_gruen > 100){
    Farbwert_gruen = 100;
  }
  if (Farbwert_rot > 100){
    Farbwert_rot = 100;
  }
  if (Farbwert_gruen < 0){
    Farbwert_gruen = 0;
  }
  if (Farbwert_rot < 0){
    Farbwert_rot = 0;
  }
  if (LED_Menge > 8){
    LED_Menge = 8;
  }
  if (LED_Menge <0){
    LED_Menge = 0;
  }
  if (Fuellmengenprozent > 99){
    Fuellmengenprozent = 99;
  }
  if (Fuellmengenprozent < 15){
    LEDs = !LEDs;
  }
  if (Fuellmengenprozent < 0){
    Fuellmengenprozent = 0;
  }
  if (Balkenlaenge > 200){
    Balkenlaenge = 200;
  }
  if (Balkenlaenge < 0){
    Balkenlaenge = 0;
  }
}

void eraseConfig() {
  // Reset EEPROM bytes to '0' for the length of the data structure
  EEPROM.begin(4095);
  for (unsigned int i = cfgStart ; i < sizeof(cfg) ; i++) {
    EEPROM.write(i, 0);
  }
  delay(200);
  EEPROM.commit();
  EEPROM.end();
}

void saveConfig() {
  // Save configuration from RAM into EEPROM
  EEPROM.begin(4095);
  EEPROM.put( cfgStart, cfg );
  delay(200);
  EEPROM.commit();                      // Only needed for ESP8266 to get data written
  EEPROM.end();                         // Free RAM copy of structure
}

void loadConfig() {
  // Loads configuration from EEPROM into RAM
  EEPROM.begin(4095);
  EEPROM.get( cfgStart, cfg );
  EEPROM.end();
}

void Startanzeige(){
  display.fillScreen(GxEPD_WHITE);
  display.drawRoundRect(0, 0, 200, 25, 5, GxEPD_BLACK);
  display.setTextSize(2);
  display.setCursor(40,7);
  display.setTextColor(GxEPD_BLACK);
  display.println(F("Willkommen"));
  display.setCursor(0,30);
  display.setTextSize(2);
  display.println(F("folgende Daten  wurden aus dem  EEPROM gelesen:"));
  display.println(F(""));
  display.print(F("unterer Wert: "));
  display.println(cfg.Messwert_LOW);
  display.print(F("oberer Wert: "));
  display.println(cfg.Messwert_HIGH);
  display.update();
  delay(8000);
}