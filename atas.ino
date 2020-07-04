#include "Core.h"
#include "Arduino.h"
#include <Wire.h>

#define PIR_1 35
#define PIR_2 32
#define PIR_3 33
#define PIR_4 25
#define PIR_5 26

#define TX2 17
#define RX2 16

Core core;
const long cloudDataInterval = 500;
//TaskHandle_t Task1;
int firstTime = 0;

void setup() 
{
  Serial.begin(57600);
  Serial2.begin(19200, SERIAL_8N1, RX2, TX2);

//  xTaskCreatePinnedToCore(
//    Task1code,   /* Task function. */
//    "Task1",     /* name of task. */
//    10000,       /* Stack size of task */
//    NULL,        /* parameter of the task */
//    1,           /* priority of the task */
//    &Task1,      /* Task handle to keep track of created task */
//    0);          /* pin task to core 0 */ 
  
  //core.initializeComponents();
  
  delay(500); 
}

void Task1code( void * pvParameters ){

  unsigned long previousMillis = 0;

  for(;;){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= cloudDataInterval) {
      previousMillis = currentMillis;
      if(core.checkInternet())
      {
        Serial.println("Internet is connected");
      }

      // Recieve calls

      // Recieve messages

      //Serial.println(core.fetchIPAddress());
    }
    delay(100);
  }
}


void loop() {
  core.stateMachineInitialize();
  
  if(! core.checkSystemInitializationStatus()){
    return;
  }
    
    Serial.println("Waiting for any trigger!!");

    int commaCount = 0;
    if (Serial2.available() > 0) {
       char bfr[501];
       memset(bfr,0, 501);
       Serial2.readBytesUntil( '\n',bfr,500);
  
       for(int i =0; i<=sizeof(bfr); i++){
          if(bfr[i] == ','){
            commaCount = commaCount + 1;
          }
        }
      
        if(commaCount == 4){
          Serial.println("<< Data Recieved ");
          Serial.println(bfr);
       }
    }
    
//    int ADC_VALUE = analogRead(PIR_1);
//    int voltage_value = (ADC_VALUE * 3.3 ) / (4095);
//    if(voltage_value > 2.5){
//      Serial.println("Trigger on PIR 1");
//      core.updateStateMachine(2,0);
//    }
//  
//    ADC_VALUE = analogRead(PIR_2);
//    voltage_value = (ADC_VALUE * 3.3 ) / (4095);
//    if(voltage_value > 2.5){
//      Serial.println("Trigger on PIR 2");
//      core.updateStateMachine(2,0);
//    }
//  
//    ADC_VALUE = analogRead(PIR_3);
//    voltage_value = (ADC_VALUE * 3.3 ) / (4095);
//    if(voltage_value > 2.5){
//      Serial.println("Trigger on PIR 3");
//      core.updateStateMachine(2,0);
//    }
//  
//    ADC_VALUE = analogRead(PIR_4);
//    voltage_value = (ADC_VALUE * 3.3 ) / (4095);
//    if(voltage_value > 2.5){
//      Serial.println("Trigger on PIR 4");
//      core.updateStateMachine(2,0);
//    }
//  
//    ADC_VALUE = analogRead(PIR_5);
//    voltage_value = (ADC_VALUE * 3.3 ) / (4095);
//    if(voltage_value > 2.5){
//      Serial.println("Trigger on PIR 5");
//      core.updateStateMachine(2,0);
//    }
  
  delay(2000);
}

