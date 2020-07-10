#include "Core.h"
#include "Wifi.h"
#include "StateMachine.h"
#include "Parameter.h"
#include "Communication.h"

Core::Core(){
  wifi = Wifi();
  stateMachine = StateMachine();
  stateMachine.updateStateMachine(0, 0);
}

bool Core::initializeGSM(){
  bool result = true;
  
  if(! communication.setupGSMModule()){
    Serial.println(F("GSM setup has failed!! "));
    result = false;
  }

  if(result) {
    stateMachine.updateStateMachine(0,2);
  }

  return result;
}

void Core::updatePIRStatus(char values[]){

  pir1 = values[0];
  pir2 = values[2];
  pir3 = values[4];
  pir4 = values[6];
  pir5 = values[8];
  pir11 = values[10];
  pir12 = values[12];
  pir13 = values[14];
  pir14 = values[16];
}

void Core::checkAndSetPrioritySensorTriggerStatus(){
  if(pir1=='1'){
    Serial.println(F("Priority 1 sensor triggered"));
    lastTiggeredSensor = 1;
    triggerFirstPrioritySensors();
  }else if(pir2=='1'){
    Serial.println(F("Priority 2 sensor triggered"));
    lastTiggeredSensor = 1;
    triggerFirstPrioritySensors();
  }else if(pir3=='1'){
    Serial.println(F("Priority 3 sensor triggered"));
    lastTiggeredSensor = 1;
    triggerFirstPrioritySensors();
  }else if(pir4=='1'){
    Serial.println(F("Priority 4 sensor triggered"));
    lastTiggeredSensor = 1;
    triggerFirstPrioritySensors();
  }else if(pir5=='1'){
    Serial.println(F("Priority 5 sensor triggered"));
    lastTiggeredSensor = 1;
    triggerFirstPrioritySensors();
  }else if(pir11=='1'){
    Serial.println(F("Fire sensor triggered"));
    lastTiggeredSensor = 2;
    triggerFirstPrioritySensors();
  }else if(pir12=='1'){
    Serial.println(F("Alert sensor triggered"));
    lastTiggeredSensor = 3;
    triggerFirstPrioritySensors();
  }else if(pir13=='1'){
    Serial.println(F("Emergency help sensor triggered"));
    lastTiggeredSensor = 4;
    triggerFirstPrioritySensors();
  }
  // 1 means door closed and 0 means door opened
  if(doorClosed==1 && pir14=='0'){
    Serial.println(F("Door has been opened"));
    doorClosed = 0; // Door has been opened
    activateAllSensors = 0;
  }

  if(doorClosed==0 && pir14=='1'){
    Serial.println(F("Door has been closed"));
    doorClosed = 1;
    lastDoorClosedTime = millis();
  }
}

void Core::stateMachineInitialize(){
  uint8_t action = stateMachine.getAction();
  uint8_t state = stateMachine.getState();

 switch(action){
  case 0: // INIT 
    if(state == 0){
      communication.setupGSMModule();
      stateMachine.updateStateMachine(0,1);
    } else if(state == 1){
      
      if(! wifi.checkConnection()){
        wifi.setUsernamePassword(parameter.ssid, parameter.password);
      }

      uint8_t imei = communication.getIMEINumber();
      if (imei <= 0){
        communication.setupGSMModule();
      } else {
        stateMachine.updateStateMachine(4,0);
      }
    }
     break;
   case 1: // Default position - Check if sim is connected to the network    
    if(state == 0){ 
      stateMachine.updateStateMachine(1,1);
    } else if(state == 1){
      uint8_t statusOfNetwork = communication.getNetworkStatus();
      if(statusOfNetwork == 1 || statusOfNetwork == 5){
        Serial.println(F("Network connected"));
        stateMachine.updateStateMachine(2,0);
      }else{
        Serial.println(F("Not connected: "));
      }
    }
    break;
   case 2:
    if(state == 0){ 
      delay(2000);
      stateMachine.updateStateMachine(3,0);
    }
    break;
   case 3: // First priority sensors triggered
    if(state == 0){ 
      stateMachine.updateStateMachine(3,1);
    } else if(state == 1){
      // Begin sequence
      communication.setSensorTriggeredStatus(lastTiggeredSensor);
      bool result = communication.beginSequence();
      if(result){
        stateMachine.updateStateMachine(4,0);
      }
    }
    break;
   case 4: // dummy!!
    if(state == 0){ 
      //Serial.println("State idle");
      //delay(2000);
    }
    break;
  }
}


