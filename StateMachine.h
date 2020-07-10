#ifndef StateMachine_h
#define StateMachine_h
#include "Arduino.h"

class StateMachine
{
  public:
    uint8_t getState();
    uint8_t getAction();
    void updateStateMachine(uint8_t _action, uint8_t _state);
    
  private:
    /** 
     *  0. INIT 
     *  1. Default position
     *  2. Idle position - Waiting and checking
     *  3. First priority sensors triggered
     *  4. Second priority sensors triggered
     *  5. Emergency sensors triggered
     *  6. New Message Recieved
     */
    volatile uint8_t action;

    /** 
     *  1. On-going
     *  2. Complete
     */
    volatile uint8_t state;
};

#endif

