#include <Arduino.h>

// Defines
#define PIR 2
#define LED 3
#define LDR A0
#define ACS A1

#define MAX_PWM 255
#define MIN_PWM 0
#define MAX_PIR 1023
#define MIN_PIR 0

#define MIN_PERSON_TIME 7000

#define START 0x1A

// Variables

// Flags PIR
bool _pirEnabled = false;

// Time
unsigned long _now;
unsigned long _pirTime = 0;

uint16_t _current;
float _voltageResistor;

uint8_t _pwm = 0;
uint8_t _percentage = 0;

// Buffer
uint8_t buffer[4];
size_t length = 4;

void setup() {
  // Pin configuration
  pinMode(PIR, INPUT);
  pinMode(LDR, INPUT);
  pinMode(LED, OUTPUT);

  // Serial configuration
  Serial.begin(9600);
  Serial1.begin(115200);

  // Initialize process
  digitalWrite(LED, LOW);

  // ISR
  attachInterrupt(digitalPinToInterrupt(PIR), pir_process, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  _now = millis();

  if((_now - _pirTime) > MIN_PERSON_TIME){
    _pirEnabled = false;
  }

  if(_pirEnabled){
    uint16_t ldrRead = analogRead(LDR);
    _pwm = map(ldrRead, 0, 1023, 0, 255);
    analogWrite(LED, _pwm);
    _current = calculateCurrent(_pwm);
  }
  else{
    _pwm = (uint8_t)(0.2 * MAX_PWM);
    analogWrite(LED, _pwm);
    _current = calculateCurrent(_pwm);
  }

  _percentage = (float)_pwm * 100 / MAX_PWM;

  // Print on serial
  Serial.print("Current: ");
  Serial.println(_current);
  Serial.print("Percentage: ");
  Serial.println(_percentage);

  // Send to nodemcu
  buffer[0] = START;                   // Start byte
  buffer[1] = (_current >> 8) & 0xFF; // Most significant bits are sent first
  buffer[2] = _current & 0xFF;        // Least significant bits are sent after
  buffer[3] = _percentage & 0xFF;
  Serial1.write(buffer, length);

  delay(200);
}

uint16_t calculateCurrent(uint8_t pwm){
  float voltageResistor = -0.0155 + 0.0117 * pwm;
  uint16_t current = voltageResistor * 1000000 / 330;
  return current;
}

void pir_process(){
  _pirTime = millis();
  _pirEnabled = true;
}