#include "Arduino.h"
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
    Serial.println("GSM setup has failed!! ");
    result = false;
  }

  if(result) {
    stateMachine.updateStateMachine(0,2);
  }

  return result;
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
        Serial.print("statusOfNetwork: ");
        Serial.println(statusOfNetwork);
        Serial.println("Network connected");
        stateMachine.updateStateMachine(2,0);
      }else{
        Serial.print("Waiting for network connection...Status: ");
        Serial.println(statusOfNetwork);
      }
    }
    break;
   case 2:
    if(state == 0){ 
      Serial.println("All set...waiting...");
      delay(2000);
      stateMachine.updateStateMachine(3,0);
    }
    break;
   case 3: // 3. First priority sensors triggered
    if(state == 0){ 
      stateMachine.updateStateMachine(3,1);
    } else if(state == 1){
      // Begin sequence
      bool result = communication.beginSequence();
      if(result){
        stateMachine.updateStateMachine(4,0);
      }
    }
    break;
   case 4: // dummy!!
    if(state == 0){ 
      Serial.println("State idle");
      //delay(2000);
    }
    break;
  }
}

