/*THRUST STAND MACHINE CODE*/

/*LIBRARIES*/
#include <Wire.h>
#include <SPI.h>
#include <HX711_ADC.h>

/*EEPROM*/
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

/*PINS AND OBJECTS*/
const int HX711_dout = 4; 
const int HX711_sck = 5; 

HX711_ADC LoadCell(HX711_dout, HX711_sck);
    
const int calVal_eepromAdress = 0;
unsigned long t = 0;
unsigned long stabilizingtime = 2000;

int GREEN = 6;
int BUZZER = 8;

boolean _tare = true;
static boolean newDataReady = 0;

/*CONSTANTS AND VARIABLES*/
float thrust = 0;
float calibrationValue;
const int serialPrintInterval = 10; 

/*TIME*/
float time;
  

/*SETUP*/
void setup() {
  Serial.begin(115200);
  Serial.flush();
  Serial.println("Starting...");
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.println("Starting communication...");
  Serial.flush();

  delay(500);

  LoadCell.begin();
  EEPROM.get(calVal_eepromAdress, calibrationValue);
  LoadCell.start(stabilizingtime, _tare);

  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("HX711 failed to initialize ...");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue);
    Serial.println("Startup is complete");
    digitalWrite(GREEN, HIGH);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
    delay(500);
    digitalWrite(BUZZER, HIGH);
    delay(500);
    digitalWrite(BUZZER, LOW);
  }


}

/*LOOP*/
void loop() {
  Serial.flush();
  if (LoadCell.update()) newDataReady = true;

  if (newDataReady) {
    
      /*LOADCELL READING*/
      thrust = LoadCell.getData()/1000*9.81;

      float data[2] = {time,thrust};

      Serial.print(data[0]);
      Serial.print(",");
      Serial.println(data[1]);
      
      newDataReady = 0;
      time = time + 101;
  } 
}

