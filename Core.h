#ifndef Core_h
#define Core_h

#include "Wifi.h"
#include "StateMachine.h"
#include "Parameter.h"
#include "Communication.h"

class Core
{
  public:
    Core();

    
    
    bool checkInternet() { return wifi.checkConnection(); }
    String fetchIPAddress() { return wifi.fetchIPAddress(); }
    void stateMachineInitialize();

    void updateStateMachine(uint8_t _action, uint8_t _state){
      uint8_t action = stateMachine.getAction();
      uint8_t state = stateMachine.getState();
        
      if(action == 4 && state == 0){
        Serial.println("State machine set for sending msgs and calls...!! ");
        stateMachine.updateStateMachine(_action, _state);
      }
    }

    bool checkSystemInitializationStatus(){
      uint8_t action = stateMachine.getAction();
      uint8_t state = stateMachine.getState();

      if(action == 4 && state == 0) return true;

      return false;
    }

    void checkAndSetPrioritySensorTriggerStatus();

    void triggerFirstPrioritySensors() {
      stateMachine.updateStateMachine(3,0);
    }

    void updatePIRStatus(char values[]);
    
  private:
    Wifi wifi;
    StateMachine stateMachine;
    Parameter parameter;
    Communication communication;
    bool initializeGSM();
    float pir1=0.0; 
    float pir2=0.0; 
    float pir3=0.0; 
    float pir4=0.0; 
    float pir5=0.0;
};

#endif
  
