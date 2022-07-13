#include <Arduino.h>
#include <SPI.h>

int Analog_value_Read_Potentiometer = 0;
int Analog_read_previous = 0;

void setup() {
    pinMode(A0, INPUT);
    SPI.begin();
    digitalWrite(SS, HIGH);
    SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop() {
    Analog_value_Read_Potentiometer = analogRead(A0) >> 2;
    if (Analog_read_previous != Analog_value_Read_Potentiometer){
      Analog_read_previous = Analog_value_Read_Potentiometer;
      digitalWrite(SS, LOW);
      SPI.transfer((char) Analog_value_Read_Potentiometer);
      digitalWrite(SS, HIGH);
    }
    
    
}