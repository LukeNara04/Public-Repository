/*THRUST STAND MACHINE CODE* 10Hz/

/*LIBRARIES*/
#include <Wire.h>
#include <SPI.h>
#include <HX711_ADC.h>

/*EEPROM*/
#if defined(ESP8266) || defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

/*PINS AND OBJECTS*/
const int HX711_dout = 4; 
const int HX711_sck = 5; 

HX711_ADC LoadCell(HX711_dout, HX711_sck);
    
const int calVal_eepromAdress = 0;
unsigned long stabilizingtime = 2000;

int GREEN = 6;
int BUZZER = 8;

boolean _tare = true;
static boolean newDataReady = 0;

/*CONSTANTS AND VARIABLES*/
bool start = false;
float thrust = 0;
float time = 0;                  // Initialize time to 0
unsigned long time_new = 0;      // Variable to store current time
unsigned long time_old = 0;      // Variable to store previous time
float calibrationValue;
const int serialPrintInterval = 1; 
  
/*SETUP*/
void setup() {
  Serial.begin(115200);
  Serial.flush();
  Serial.println("Starting...");
  pinMode(GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.println("Starting communication...");
  Serial.flush();

  delay(250);

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
    delay(250);
    digitalWrite(BUZZER, LOW);
    delay(250);
    digitalWrite(BUZZER, HIGH);
    delay(250);
    digitalWrite(BUZZER, LOW);
    delay(250);
    digitalWrite(BUZZER, HIGH);
    delay(250);
    digitalWrite(BUZZER, LOW);
  }
}

/*LOOP*/
void loop() {
  if (LoadCell.update()) {
    newDataReady = true;
  }

  if (newDataReady){                      
    thrust = LoadCell.getData() / 1000 * 9.81;     // Calculate thrust
    if (thrust >= 0.2){
      if(start == false){
        time_old = millis();
      }
      time_new = millis();                           // Get the current time
      start = true;
    
      time += (time_new - time_old) / 1000.0;        // Convert ms to seconds for `time`
      time_old = time_new;                           // Update the previous time

      Serial.print(time, 3);                         // Print time with 3 decimal places
      Serial.print(",");
      Serial.println(thrust, 3);                     // Print thrust with 3 decimal places

      newDataReady = false;                          // Reset the flag
    }
  } 
}

