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
const long cloudDataInterval = 2000;
TaskHandle_t Task1;
int firstTime = 0;

void setup() 
{
  Serial.begin(57600);
  Wire.begin(); // start 2-wire communication
  Wire.setClock(400000L);
  Serial2.begin(19200, SERIAL_8N1, RX2, TX2);

  xTaskCreatePinnedToCore(
    Task1code,   /* Task function. */
    "Task1",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &Task1,      /* Task handle to keep track of created task */
    0);          /* pin task to core 0 */ 
  
  delay(500); 
}

void Task1code( void * pvParameters ){

  unsigned long previousMillis = 0;

  for(;;){
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= cloudDataInterval) {
      previousMillis = currentMillis;

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
      
        if(commaCount == 9){
          //Serial.println(bfr);
          core.updatePIRStatus(bfr);
        }
      }

      // Recieve calls
      // Recieve messages
    }
    delay(1000);
  }
}

void loop() {

  if(core.checkSystemInitializationStatus()){
    core.checkAndSetPrioritySensorTriggerStatus();
  }
  core.updateDoorClosedStatus();
  core.stateMachineInitialize();
  delay(1000);
}
