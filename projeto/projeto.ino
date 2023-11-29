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

// Variables

// Flags PIR
bool _pirEnabled = false;

// Time
unsigned long _now;
unsigned long _pirTime = 0;

float _current;
float _voltageResistor;

uint8_t _voltage = 0;

void setup() {
  // Pin configuration
  pinMode(PIR, INPUT);
  pinMode(LDR, INPUT);
  pinMode(LED, OUTPUT);

  // Serial configuration
  Serial.begin(9600);
  Serial1.begin(9600);

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
    _voltage = map(ldrRead, 0, 1023, 0, 255);
    analogWrite(LED, _voltage);
    _current = calculateCurrent(_voltage);
  }
  else{
    _voltage = (uint8_t)(0.2 * MAX_PWM);
    analogWrite(LED, _voltage);
    _current = calculateCurrent(_voltage);
  }

  Serial.print("Current: ");
  Serial.println(_current);
}

uint16_t calculateCurrent(uint8_t voltage){
  float voltageResistor = -0.0155 + 0.0117 * voltage;
  uint16_t current = voltageResistor * 1000000 / 330;
  return current;
}

void pir_process(){
  _pirTime = millis();
  _pirEnabled = true;
}