/*THRUST STAND MACHINE CODE 50Hz*/

/*LIBRARIES*/
#include <Wire.h>
#include <SPI.h>
#include <HX711_ADC.h>

/*EEPROM*/
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

/*PINS AND OBJECTS*/
const int HX711_dout = 8; 
const int HX711_sck = 3; 

HX711_ADC LoadCell(HX711_dout, HX711_sck);
    
const int calVal_eepromAdress = 0;
unsigned long t = 0;
unsigned long dt_us = 0.0;
unsigned long now_us = 0.0;
unsigned long stabilizingtime = 2000;

int GREEN = 6;
int BUZZER = 8;

boolean _tare = true;
static boolean newDataReady = 0;

/*CONSTANTS AND VARIABLES*/
float ts = 0.0;
float ts_now = 0.0;
float ts_last = 0.0;
float dt = 0.0;
float thrust = 0;
float calibrationValue;
const int serialPrintInterval = 10; 
unsigned long LOOP_PERIOD_US = 20000;   // 20 ms  → 50 Hz

/* ---- globals ---- */
float ts_last_us = 0.0;   // time of previous loop() start


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
  LoadCell.setSamplesInUse(1);
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
  ts_last_us = micros(); 


}

/*LOOP*/
void loop() {
/* ---------- wait until 20 ms have elapsed ---------- */
  now_us = micros();
  dt_us = now_us - ts_last_us;        // elapsed time (µs)

  if (dt_us < LOOP_PERIOD_US) {                      // still too early?
    delayMicroseconds(LOOP_PERIOD_US - dt_us);       // busy‑wait remainder
    now_us = micros();                               // refresh after wait
    dt_us  = now_us - ts_last_us;
  }
  ts_last_us = now_us;                               // mark start of this cycle

  /* ---------- do the 50 Hz job ---------- */
  float dt   = dt_us * 1e-6;                         // s
  if (LoadCell.update()) newDataReady = true;

  float thrust = LoadCell.getData() / 1000.00 * 9.80665;

  Serial.print(ts);
  Serial.print(',');
  Serial.println(thrust);

  ts += dt;                                       
}


