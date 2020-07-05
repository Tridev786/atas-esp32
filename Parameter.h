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
    //String names[10] = {"Husband","Wife","Person3","Person4","Person5","Person6","Person7","Person8","Person9","Person10"};
    
    String msg1 = "Warning SMJ Factory intruder alert - Main Door is broken.";
    String msg2 = "Flat No 134, Door No: 253, Rajam Nagar, 4th street, Kovalan Nagar Main Road, TVS Nagar, Madurai 625003";
    String msg3 = "Location is http://googlemaps....";
    String msg4 = "Warning SMJ Factory intruder alert - Main Door is broken. B6 Anna nagar police station number is +918825958660";


    // **** DONT TOUCH***** //
    String fetchNumber(int no);
    String fetchMsgForNumber(int no);
    bool checkIfSecondMsgRequired(int no);

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
