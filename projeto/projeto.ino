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

// Acs712
float currentSum;
float currentMean;
float currentVoltage;
float current;

float sensibility = 185;

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
    uint8_t voltage = map(ldrRead, 0, 1023, 0, 255);
    /*Serial.print("LDR: ");
    Serial.println(ldrRead);
    Serial.print("Voltage: ");
    Serial.println(voltage);*/
    analogWrite(LED, voltage);
  }
  else{
    analogWrite(LED, (int)(0.2 * MAX_PWM));
  }

  for (int i = 0; i < 100; i++)
  {
    currentSum += analogRead(ACS);
    delay(10);
  }

  currentMean = currentSum / 100.0;
  currentVoltage = map(currentMean, 0, 1023, 0, 5000);
  current = (currentVoltage - 2500) / sensibility;
  currentSum = 0;
  
  Serial.print("Corrente: ");
  Serial.println(current);



}

void pir_process(){
  _pirTime = millis();
  _pirEnabled = true;
}