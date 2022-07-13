  #include <Arduino.h>


#define Oscilloscope_PIN 4 /* Oscilloscope */

int toggle = 0;
int Analog_value;

void setup() {

  pinMode(Oscilloscope_PIN, OUTPUT);

  /* Clear Interrupt */
  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 0;
  
  TCCR1A |= (1 << COM1A0) | (1 << COM1B0);
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  TIMSK1 |= (1 << OCIE1A);

  /* Set Interrupt */
  sei();
}

ISR(TIMER1_COMPA_vect){
  if(toggle) {
    digitalWrite(Oscilloscope_PIN, LOW);
    OCR1A = 1023 - Analog_value;
  } else {
    digitalWrite(Oscilloscope_PIN, HIGH);
    OCR1A = Analog_value;
  }
  toggle = ~toggle;
}


void loop() {
  Analog_value = map(analogRead(A0), 0, 1023, 1023 * 0.1, 1023 * 0.9);
}