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

void Core::updatePIRStatus(char values[]){
  char buf1[4];
  buf1[0]=values[0];
  buf1[1]=values[1];
  buf1[2]=values[2];
  buf1[3]=values[3];
  pir1 = atof(buf1);

  char buf2[4];
  buf2[0]=values[5];
  buf2[1]=values[6];
  buf2[2]=values[7];
  buf2[3]=values[8];
  pir2 = atof(buf2);

  char buf3[4];
  buf3[0]=values[10];
  buf3[1]=values[11];
  buf3[2]=values[12];
  buf3[3]=values[13];
  pir3 = atof(buf3);

  char buf4[4];
  buf4[0]=values[15];
  buf4[1]=values[16];
  buf4[2]=values[17];
  buf4[3]=values[18];
  pir4 = atof(buf4);

  char buf5[4];
  buf5[0]=values[20];
  buf5[1]=values[21];
  buf5[2]=values[22];
  buf5[3]=values[23];
  pir5 = atof(buf5);
}

void Core::checkAndSetPrioritySensorTriggerStatus(){
  if(pir1>=4.9){
    Serial.println("Priority 1 sensor triggered");
    triggerFirstPrioritySensors();
    pir1 = 0.0;
  }else if(pir2>=4.9){
    Serial.println("Priority 2 sensor triggered");
    triggerFirstPrioritySensors();
    pir2 = 0.0;
  }else if(pir3>=4.9){
    Serial.println("Priority 3 sensor triggered");
    triggerFirstPrioritySensors();
    pir3 = 0.0;
  }else if(pir4>=4.9){
    Serial.println("Priority 4 sensor triggered");
    triggerFirstPrioritySensors();
    pir4 = 0.0;
  }else if(pir5>=4.9){
    Serial.println("Priority 5 sensor triggered");
    triggerFirstPrioritySensors();
    pir5 = 0.0;
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
      //Serial.println("State idle");
      //delay(2000);
    }
    break;
  }
}

