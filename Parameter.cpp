#include "Parameter.h"

void Parameter::standByMsgs(){
  long timeCalc = millis();
  long calculated = timeCalc - displayResetTimer;
  Serial.print(calculated);
  Serial.print(" : ");
  Serial.println(standByMsgInterval);
  if( calculated > standByMsgInterval){
    displayDefault(standByMsgCounter);
    displayResetTimer = timeCalc;
  }
}

void Parameter::displayDefault(int displayValue){
  switch(displayValue){
    case 0:
        displayController.clearAll();
        displayController.setDisplayPosition(0,3,"P.Dharshinisri");
        displayController.setDisplayPosition(1,10,"&");
        displayController.setDisplayPosition(2,2,"P.Tridev Vishakan");
        displayController.setDisplayPosition(3,4,"Version : 9.1");
        standByMsgCounter = 1;
      break;
    case 1:
        displayController.clearAll();
        displayController.setDisplayPosition(0,6,"Results");
        displayController.setDisplayPosition(1,7,"From");
        displayController.setDisplayPosition(2,6,"Hardwork");
        displayController.setDisplayPosition(3,0,"");
        standByMsgCounter = 2;
      break;
    case 2:
        displayController.clearAll();
        displayController.setDisplayPosition(0,1,"Safety Starts From");
        displayController.setDisplayPosition(1,6,"A.T.A.S");
        displayController.setDisplayPosition(2,0,"Always Protect Your");
        displayController.setDisplayPosition(3,0,"Property From Theft");
        standByMsgCounter = 3;
      break;
    case 3:
        displayController.clearAll();
        displayController.setDisplayPosition(0,0,"Anti Theft ALERT Sys");
        displayController.setDisplayPosition(1,2,"Private Limited ");
        displayController.setDisplayPosition(2,1,"Technical Support");
        displayController.setDisplayPosition(3,0,"Mob: +91 9894786595");
        standByMsgCounter = 4;
      break;
    case 4:
        displayController.clearAll();
        displayController.setDisplayPosition(0,1,"A.T.A.S - Device");
        displayController.setDisplayPosition(1,0,"Tested & Approved By");
        displayController.setDisplayPosition(2,7,"POLICE");
        displayController.setDisplayPosition(3,5,"Department");
        standByMsgCounter = 5;
      break;
    case 5:
        displayController.clearAll();
        displayController.setDisplayPosition(0,0,"A.T.A.S Aspiring To");
        displayController.setDisplayPosition(1,3,"Be Worlds No 1");
        displayController.setDisplayPosition(2,3,"In Preventing");
        displayController.setDisplayPosition(3,2,"Theft And Murder");
        standByMsgCounter = 0;
      break;
    case 6:
        displayController.clearAll();
        displayController.setDisplayPosition(0,0,"A.T.A.S Theft Sensor");
        displayController.setDisplayPosition(1,3,"Be Worlds No 1");
        displayController.setDisplayPosition(2,3,"In Preventing");
        displayController.setDisplayPosition(3,2,"Theft And Murder");
        standByMsgCounter = 0;
      break;
    case 7:
        displayController.clearAll();
        displayController.setDisplayPosition(0,0,"A.T.A.S Theft Sensor");
        displayController.setDisplayPosition(1,3,"Be Worlds No 1");
        displayController.setDisplayPosition(2,3,"In Preventing");
        displayController.setDisplayPosition(3,2,"Theft And Murder");
        standByMsgCounter = 0;
      break;
  }
}

void displayAlertTypes(int type, int number, int msgStatus, int callStatus){
  displayController.clearAll();
  displayController.setDisplayPosition(0,0,"A.T.A.S Theft Sensor");
  displayController.setDisplayPosition(1,3,"Be Worlds No 1");
  displayController.setDisplayPosition(2,0,"Msg Status: ");

  if(msgStatus == false){
    displayController.setDisplayPosition(2,13,"Failed");
  }else{   
    displayController.setDisplayPosition(2,13,"Success");
  }
  
  displayController.setDisplayPosition(3,2,"Theft And Murder");
}

//
//displayController.lcdInitialize();
//displayController.displaySimpleChars();

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

