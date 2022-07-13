#include <Arduino.h>

#define LED_PIN_START 2 /* Pins: 2-3-4-5 */
#define LED_COUNT 4 
#define LED_ON 1   /* High OR True */
#define LED_OFF 0 /*  Low OR False*/

int ADC_val = 0; // Initial Value For Analog_Digital_Convertor_Value

void setup() {
  /* 
    1.put our setup code here, to run once.
    2.Set input and output in according Arduino Uno Board That Means Left pins --> ANALOG & Right pins --> Digital.
    3.The pinMode() function is used to configure a specific pin to behave either as an input or an output.
  */
   pinMode(A0, INPUT);
   for (int i = 0; i < LED_COUNT; i++)
   {
     pinMode(LED_PIN_START + i, OUTPUT);
   }
}

void loop() {
  /*
   put our main code Here, to run repeatedly.
   analoRead    --> Reads the value from the specified analog pin.
   digitalWrite --> pin: the Arduino pin number. value: HIGH or LOW. Returns: Nothing. 
  */

  int val = analogRead(A0);
  ADC_val = map(val, 0, 1023, 0, 100);

  for (int counter = 0; counter < LED_COUNT; counter++) {
    if (ADC_val > 90 && counter == 3) {
      digitalWrite(LED_PIN_START + counter, LED_ON);
    }
    else {
      if (ADC_val >= (counter + 1) * 25) {
          digitalWrite(LED_PIN_START + counter, LED_ON);
      } 
      else {
        digitalWrite(LED_PIN_START + counter, LED_OFF);
      }
    }
  }
}