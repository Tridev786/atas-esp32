#include "Parameter.h"

String Parameter::fetchNumber(int no, int type){
  int tmp[10];
  memcpy(tmp, numbersActive, sizeof(numbersActive));
  if(type == 2){
    memcpy(tmp, thirdPriorityNumbers, sizeof(thirdPriorityNumbers));
  }

  if(no == 0){
    if(tmp[no] == 1){
      return number0;
    }else{
      return "";
    }
  }else if(no == 1){
    if(tmp[no] == 1){
      return number1;
    }else{
      return "";
    }
  }else if(no == 2){
    if(tmp[no] == 1){
      return number2;
    }else{
      return "";
    }
  }else if(no == 3){
    if(tmp[no] == 1){
      return number3;
    }else{
      return "";
    }
  }else if(no == 4){
    if(tmp[no] == 1){
      return number4;
    }else{
      return "";
    }
  }else if(no == 5){
    if(tmp[no] == 1){
      return number5;
    }else{
      return "";
    }
  }else if(no == 6){
    if(tmp[no] == 1){
      return number6;
    }else{
      return "";
    }
  }else if(no == 7){
    if(tmp[no] == 1){
      return number7;
    }else{
      return "";
    }
  }else if(no == 8){
    if(tmp[no] == 1){
      return number8;
    }else{
      return "";
    }
  }else if(no == 9){
    if(tmp[no] == 1){
      return number9;
    }else{
      return "";
    }
  }
}

String Parameter::fetchMsgForNumber(int no, int type){
  String msgType = "";
  if(type == 4){
    msgType = alert_types[type];
  }else{
    msgType = "Warning " + alert_types[type];
  }

  if(no == 3 && type == 2){
    return template_part1 +  msgType;
  }

  if((no == 0 || no == 1 || no == 4 || no == 5 || no == 6 || no == 9) && type == 2){
    return template_part1 +  msgType + template_part3;
  }
  
  if(no == 0 || no == 1 || no == 4 || no == 5 || no == 6){
    return template_part1 +  msgType + template_part2;
  }else {
    return template_part1 +  msgType;
  }
}

bool Parameter::checkIfSecondMsgRequired(int no){

      if(no == 0) return false;

      for(int i=0; i<=secPriorityCount-1; i++){
        int number = secondPriorityNumbers[i];

        Serial.print(F("number in series is: "));
        Serial.print(no);
        Serial.print(F(" || second priority number in series is: "));
        Serial.println(number);
        if(number == no){
          return true;
        }
      }
      return false;
    }

