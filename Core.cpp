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
  pir6 = values[10];
  pir7 = values[12];
  pir8 = values[14];
  pir9 = values[16];
  pir10 = values[18];
  pir11 = values[20];
  pir12 = values[22];
  pir13 = values[24];
  pir14 = values[26];
  pir15 = values[28];
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
  }else if(pir6=='1'){
    Serial.println(F("First camera triggered"));
    sendSnapShot(1);
  }else if(pir7=='1'){
    Serial.println(F("Second camera triggered"));
    sendSnapShot(2);
  }else if(pir8=='1'){
    Serial.println(F("Third camera triggered"));
    sendSnapShot(3);
  }else if(pir9=='1'){
    Serial.println(F("Forth camera triggered"));
    sendSnapShot(4);
  }else if(pir10=='1'){
    Serial.println(F("Fifth camera triggered"));
    sendSnapShot(5);
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
  }else if(pir15=='0'){
    Serial.println(F("Main door broken triggered"));
    lastTiggeredSensor = 0;
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
  
  // 1 means door closed and 0 means door opened
  if(doorClosed==1 && pir14=='0'){
    doorClosed = 0; // Door has been opened
  }

  if(doorClosed==0 && pir14=='1'){
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
      parameter.setupDisplay();
      parameter.setupScreen("Connecting to GSM.");
      bool systemStatus = communication.setupGSMModule();
      if(systemStatus){
        parameter.setupScreen("Connected to GSM");
      }
      delay(3000);
      stateMachine.updateStateMachine(0,1);
      
    } else if(state == 1){
      parameter.setupScreen("Setting up wifi");
      if(! wifi.checkConnection()){
        wifi.setUsernamePassword(parameter.ssid, parameter.password);
      }
      parameter.setupScreen("Wifi connected");

      uint8_t imei = communication.getIMEINumber();
      if (imei <= 0){
        parameter.setupScreen("Reonnecting to GSM");        
        communication.setupGSMModule();
      } else {
        parameter.setupScreen("Setup completed");
        delay(5000);
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
      //Serial.println("in here ");
      parameter.standByMsgs();
      //Serial.println("State idle");
      delay(2000);
    }
    break;
  }
}


