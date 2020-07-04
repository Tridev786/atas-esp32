#include "Arduino.h"
#include "StateMachine.h"

uint8_t StateMachine::getState(){ 
  return state; 
}

uint8_t StateMachine::getAction(){ 
  return action; 
}

void StateMachine::updateStateMachine(uint8_t _action, uint8_t _state){
  action = _action;
  state = _state;
//  Serial.print("Updated action value is: ");
//  Serial.println(action);
//
//  Serial.print("Updated state value is: ");
//  Serial.println(state);
}


