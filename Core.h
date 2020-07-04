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

    void triggerFirstPrioritySensors() {
      stateMachine.updateStateMachine(3,0);
    }
    
  private:
    Wifi wifi;
    StateMachine stateMachine;
    Parameter parameter;
    Communication communication;

    bool initializeGSM();
};

#endif
  
