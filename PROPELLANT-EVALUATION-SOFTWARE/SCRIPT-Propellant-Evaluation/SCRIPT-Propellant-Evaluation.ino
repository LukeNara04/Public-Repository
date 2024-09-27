/*PROPELLANT EVALUATION MACHINE CODE*/

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
float pressure;

/*TIME*/
float time;

void setup() {
  Serial.begin(115200); 
  Serial.flush();
  Serial.println("Starting...");
  pinMode(RED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.println("Starting communication...");
  Serial.flush();

  delay(500);
  zeroValue = analogRead(sensorPin);
  zeroVoltage = (zeroValue / analogMax) * supplyVoltage;
  zeroPressure = ((zeroVoltage - voltageAtZeroPressure) / sensorVoltageRange) * sensorMaxPressure;
  Serial.println("Startup is complete");
  digitalWrite(RED, HIGH);
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

void loop() {
  Serial.flush();

  /*PRESSURE TRANSDUCER READING*/
  sensorValue = analogRead(sensorPin);
  sensorVoltage = (sensorValue / analogMax) * supplyVoltage;
  pressure = ((sensorVoltage - voltageAtZeroPressure) / sensorVoltageRange) * sensorMaxPressure - zeroPressure; 
  
  float data[2] = {time,pressure};

  Serial.print(data[0]);
  Serial.print(",");
  Serial.println(data[1]);

  delay(10);
  time = time + 12.73;
}
