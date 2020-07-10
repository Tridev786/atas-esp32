#ifndef Communication_h
#define Communication_h
#include "Parameter.h"

class Communication
{
  public:
    Communication();
    bool setupGSMModule();
    uint8_t getIMEINumber();
    uint8_t getNetworkStatus();
    bool beginSequence();
    void setSensorTriggeredStatus(int sensor){
      sensorTriggered = sensor;
    }
    
  private:

    Parameter parameter;

    uint8_t currentNumber = 0;
    uint8_t callAnsweredState = 1; // 0 for success // 1 for failed
    uint8_t currentAction; // 0 for msg // 1 for call
    uint8_t sequenceStatus; // 0 for idle // 1 failed attempt 1 // 2 failed attempt 2 // 3 Ringing // 4 Picked up // 5. Call ended 
    uint8_t previousState = 0; 

    bool sendSms(String fetchedNumber, String msgToSend);
    bool makeCall(String fetchedNumber);
    char parseInformation(char str[]);
    bool checkIfCallPickedUp();
    bool checkIfReadyForCall();
    void flushSerial();
    uint8_t callStatus();
    bool contactUser(String fetchedNumber);
    int sensorTriggered = 0;
};

#endif

