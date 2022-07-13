#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <queue.h>

LiquidCrystal_I2C lcd(0x3f,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4;  //Four Rows
const byte COLS = 3; //Three Columns

char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};

byte rowPins[ROWS] = {13, 12, 11, 10};
byte colPins[COLS] = {9, 8, 7};
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

int motorPower=0;

QueueHandle_t motorPowerQueue;

int getKeyPadValue() {
   if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)
        {
            if ( kpd.key[i].stateChanged && kpd.key[i].kstate == PRESSED )
            {
              return kpd.key[i].kchar;
            }
        }
    }
    return -1;
}

void Read_Keypad_Show_On_LCD(void* params){
  for(;;){
    int value;
    if((value = getKeyPadValue()) != -1) {
      if(value >= '0' && value <= '9') {
        motorPower *= 10;
        motorPower += (value - '0');
        lcd.write(value);
      }
      else if (value == '*'){
        if(motorPower > 100 ){
          motorPower %= 100;
          xQueueSend(motorPowerQueue,(void *) &motorPower, 0);
        }
        else {
          xQueueSend(motorPowerQueue,(void *) &motorPower, 0);
        }
        String keypad = "key pad value: ";
        keypad = keypad + motorPower;
        Serial.println(keypad);
      }
      else if (value == '#') {
        motorPower = 0;
        lcd.clear();
      }
    }
    
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }

}


void Speed_Motor(void* params){
  int power = 0;
  Serial.println("speed Motor");
  for(;;){
    if(uxQueueMessagesWaiting > 0 ){
      xQueueReceive(motorPowerQueue,&power, 0);
      Serial.println(power);
      int power2 =map(power, 0, 100, 0, 255);
      analogWrite(3,power);
      
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void setup() {
  lcd.init();    // initialize the lcd 
  lcd.backlight();

  Serial.begin(9600);

  pinMode(3,OUTPUT);
  motorPowerQueue = xQueueCreate( 10, sizeof(int) );
  xTaskCreate(Read_Keypad_Show_On_LCD,"Keypad_LCD",128,NULL,tskIDLE_PRIORITY+1,NULL);
  xTaskCreate(Speed_Motor,"MOTOR",128,NULL,tskIDLE_PRIORITY+1,NULL);

}

void loop() {

}