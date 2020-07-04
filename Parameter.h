#ifndef Parameter_h
#define Parameter_h

#include "Arduino.h"
#include <EEPROM.h>

class Parameter
{
  public:
    String ssid = "ionic3dp";
    String password = "8825958660";
    const static int firstPriorityCount = 3;
    const static int secPriorityCount = 6;

    int numbersActive[10] = {1,0,1,0,1,1,1,1,1,1};
    int firstPriorityNumbers[firstPriorityCount] = {0,1,2};
    int secondPriorityNumbers[secPriorityCount] = {4,5,6,7,8,9}; // Police station, fire station - send address 

    int durationToAttempt = 20000; //20 seconds is the maximum time to try a number
    int numberOfAttempts = 2;
    
    int numberToDial = 0;
    
    int intervalBtwnMsgToCall = 20000; // 2000 means 2 seconds 

    String fetchNumber(int no){
      if(no == 0){
        if(numbersActive[no] == 1){
          return number0;
        }else{
          return "";
        }
      }else if(no == 1){
        if(numbersActive[no] == 1){
          return number1;
        }else{
          return "";
        }
      }else if(no == 2){
        if(numbersActive[no] == 1){
          return number2;
        }else{
          return "";
        }
      }else if(no == 3){
        if(numbersActive[no] == 1){
          return number3;
        }else{
          return "";
        }
      }else if(no == 4){
        if(numbersActive[no] == 1){
          return number4;
        }else{
          return "";
        }
      }else if(no == 5){
        if(numbersActive[no] == 1){
          return number5;
        }else{
          return "";
        }
      }else if(no == 6){
        if(numbersActive[no] == 1){
          return number6;
        }else{
          return "";
        }
      }else if(no == 7){
        if(numbersActive[no] == 1){
          return number7;
        }else{
          return "";
        }
      }else if(no == 8){
        if(numbersActive[no] == 1){
          return number8;
        }else{
          return "";
        }
      }else if(no == 9){
        if(numbersActive[no] == 1){
          return number9;
        }else{
          return "";
        }
      }
    }

    String fetchMsgForNumber(int no){
      if(no == 0 || no == 1 || no == 4 || no == 5 || no == 6){
        return msg4;
      }else {
        return msg1;
      }
    }

    //String names[10] = {"Husband","Wife","Person3","Person4","Person5","Person6","Person7","Person8","Person9","Person10"};

    bool checkIfSecondMsgRequired(int no){

      if(no == 0) return false;

      for(int i=0; i<=secPriorityCount-1; i++){
        int number = secondPriorityNumbers[i];

        Serial.print("number in series is: ");
        Serial.print(no);
        Serial.print(" || second priority number in series is: ");
        Serial.println(number);
        if(number == no){
          return true;
        }
      }
      return false;
    }

    String msg1 = "Warning SMJ Factory intruder alert - Main Door is broken.";
    String msg2 = "Flat No 134, Door No: 253, Rajam Nagar, 4th street, Kovalan Nagar Main Road, TVS Nagar, Madurai 625003";
    String msg3 = "Location is http://googlemaps....";
    String msg4 = "Warning SMJ Factory intruder alert - Main Door is broken. B6 Anna nagar police station number is +918825958660";

  private:

    String number0 = "8825958660";
    String number1 = "8825958660";
    String number2 = "8825958660";
    String number3 = "8825958660";
    String number4 = "8825958660";
    String number5 = "8825958660";
    String number6 = "8825958660";
    String number7 = "8825958660";
    String number8 = "8825958660";
    String number9 = "8825958660";

};

#endif
