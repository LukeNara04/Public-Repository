/*PROPELLANT EVALUATION MACHINE CODE* 20Hz/

/*PINS AND OBJECTS*/
int RED = 2;
int BUZZER = 3;

/*CONSTANTS AND VARIABLES*/
const int sensorPin = A0;                   /* Pin where the pressure transducer is connected */
const float sensorMaxPressure = 200.0;      /* Maximum pressure of the transducer (in PSI) */
const float analogMax = 1023.0;             /* Maximum analog value (10-bit ADC) */
const float voltageAtZeroPressure = 0.5;    /* Voltage output from sensor at 0 PSI (adjust if needed) */
const float sensorVoltageRange = 4.5;       /* Voltage output range from sensor (0.5V to 4.5V) */
const float supplyVoltage = 5.0;            /* Supply Voltage for the sensor (5V)*/
float zeroValue = 0;                        /* Calibration value for each measurement (adjusted for temperature and external conditions) */
float zeroVoltage = 0;                      /* Calibration value in voltage */
float zeroPressure = 0;                     /* Calibration value in pressure */

int sensorValue;
float sensorVoltage;

bool start = false;
float pressure = 0;
float time = 0;
unsigned long time_new = 0;
unsigned long time_old = 0;

void setup() {
  Serial.begin(115200); 
  Serial.flush();
  Serial.println("Starting...");
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.println("Starting communication...");
  Serial.flush();

  delay(250+2000);
  zeroValue = analogRead(sensorPin);
  zeroVoltage = (zeroValue / analogMax) * supplyVoltage;
  zeroPressure = ((zeroVoltage - voltageAtZeroPressure) / sensorVoltageRange) * sensorMaxPressure;
  Serial.println("Startup is complete");
  digitalWrite(RED, HIGH);
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

void loop() {
  sensorValue = analogRead(sensorPin);
  sensorVoltage = (sensorValue / analogMax) * supplyVoltage;
  pressure = ((sensorVoltage - voltageAtZeroPressure) / sensorVoltageRange) * sensorMaxPressure - zeroPressure; 
  if (pressure >= 2.0){
    if(start == false){
      time_old = millis();
    }
    time_new = millis();
    start = true;

    time += (time_new - time_old) / 1000.0;
    time_old = time_new;
    
    Serial.print(time,3);
    Serial.print(",");
    Serial.println(pressure,3);
    delay(48.67);
  }
}
