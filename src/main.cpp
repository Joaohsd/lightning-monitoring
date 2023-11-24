#include <Arduino.h>

// Defines
#define PIR 2
#define LED 3
#define LDR A0

#define MAX_PWM 255
#define MIN_PWM 0
#define MAX_PIR 1023
#define MIN_PIR 0

// Variables
bool _hasPerson = false;
bool _pirEnabled = false;
uint32_t _pirStart = 0;

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
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(PIR) == HIGH){
    uint16_t ldrRead = analogRead(LDR);
    uint8_t voltage = map(ldrRead, 0, 1023, 0, 255);
    voltage = 255 - voltage;
    analogWrite(LED, voltage);
  }
  else{
    analogWrite(LED, (int)(0.2 * MAX_PWM));
  }
}