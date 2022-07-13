#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <queue.h>



QueueHandle_t numbers;

void createRandomNumber(void* params){
  for(;;){
    int number =  1 + (rand() % 100);
    xQueueSend(numbers,(void *) &number, 0);
  }

}

void readRandomNumber(void* params){
  int num = 0;
  for(;;){
    
    if(uxQueueMessagesWaiting > 0 ){
      xQueueReceive(numbers,&num, 0);
      Serial.println(num);
    }
    
  }
}



void setup() {
  pinMode(A0,INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  numbers = xQueueCreate( 10, sizeof(int) );
  xTaskCreate(createRandomNumber,"send_data",128,NULL,tskIDLE_PRIORITY+1,NULL);
  xTaskCreate(readRandomNumber,"receive_data",128,NULL,tskIDLE_PRIORITY+1,NULL);
 
}

void loop() {
  // put your main code here, to run repeatedly:
}