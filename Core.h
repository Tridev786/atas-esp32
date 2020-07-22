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
        //Serial.println("State machine set for sending msgs and calls...!! ");
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
      if(activateAllSensors == 0 && lastTiggeredSensor == 1){
        Serial.println(F("Door is opened, PIR's not active!!"));
        return;
      }
      stateMachine.updateStateMachine(3,0);
    }

    void updateDoorClosedStatus(){

      if(doorClosed == 0){
        Serial.println(F("Door is opened, hence skipping!!"));
        return;
      }
      
      unsigned long currentTime = millis();
      if (currentTime - lastDoorClosedTime >= parameter.doorOpenedSensitivity) {
        activateAllSensors = 1;
      }
    }

    void updatePIRStatus(char values[]);
    
  private:
    Wifi wifi;
    StateMachine stateMachine;
    Parameter parameter;
    Communication communication;
    bool initializeGSM();
    byte pir1=0; 
    byte pir2=0; 
    byte pir3=0; 
    byte pir4=0; 
    byte pir5=0;
    byte pir11=0; 
    byte pir12=0; 
    byte pir13=0;
    byte pir14=0;
    byte pir15=0;

    byte doorClosed=1;
    byte activateAllSensors = 1;
    unsigned long lastDoorClosedTime;
    int lastTiggeredSensor = 1;
};

#endif
  

