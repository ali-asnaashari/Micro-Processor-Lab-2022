#include <Arduino.h>

#define MOTOR_PORT 9

int Analog_value_Read_Potentiometer = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(A0, INPUT);
  pinMode(MOTOR_PORT, OUTPUT);
}


void loop() {
  // put your main code here, to run repeatedly:
  Analog_value_Read_Potentiometer = analogRead(A0) >> 2;
  analogWrite(MOTOR_PORT, Analog_value_Read_Potentiometer);
}