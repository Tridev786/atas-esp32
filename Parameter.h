#ifndef Parameter_h
#define Parameter_h
#include <EEPROM.h>
#include "Display.h"

class Parameter
{
  public:
    String ssid = "Ionic2.4G";
    String password = "8825958660";
    const static int secPriorityCount = 5;
    
    int numbersActive[10] = {1,1,1,0,1,1,1,1,1,1};
    String lineToken = "qEe9tA1WDJiKCq0V9PxOv0dAmNNFzC0zoP6QbezU11E";
    
    int secondPriorityNumbers[secPriorityCount] = {2,3,7,8,9}; // Send address and Location to police and fire station

    int thirdPriorityNumbers[10] = {1,1,0,1,1,1,1,0,0,1}; // Fire station conditions
    int durationToAttempt = 10000; //10 seconds is the maximum time to try a number
    int numberOfAttempts = 2;
    int numberToDial = 0;
    int intervalBtwnMsgToCall = 20 * 1000; // 10000 means 10 seconds 
    int standByMsgInterval = 5 * 1000; // 10000 means 10 seconds 

    int doorOpenedSensitivity = 15 * 1000; // Time to wait before triggering the PIR sensor after closing the door
    //String names[10] = {"Husband","Wife","Person3","Person4","Person5","Person6","Person7","Person8","Person9","Person10"};
    
    String msg1 = "Lalitha Jewellery\n\nNo: 48, South Masi Street, Madurai - 625001\n\nCurrent GPS Location - https://goo.gl/maps/J6BfHmt8xHeHr6P1A";
    String msg2 = "Area B5 - South Gate POLICE Station - Limit, Madurai - 625001";
    String msg3 = "Area B5 - South Gate FIRE Station - Limit, Madurai - 625001";

    String template_part1 = "Alert From A.T.A.S - Device\n\n";
    String alert_types[5] = {"Theft @ Lalitha Jewellery", "Anti - Theft ALERT System - Sensor Is Triggered", "Fire Sensor Is Activated @ Lalitha Jewellery", "VIBRATION Sensor Is Activated @ Lalitha Jewellery", "Emergency I Need Help"};
    String template_part2 = "\n\nArea C2 - POLICE Station, Phone No: 0452123456 Or Call - 100, Madurai";
    String template_part3 = "\n\nArea FIRE Station, Phone No: 0452123456 Or Call - 101, Madurai";

    // **** DONT TOUCH ***** //
    String fetchNumber(int no, int type);
    String fetchMsgForNumber(int no, int type);
    bool checkIfSecondMsgRequired(int no);
    void standByMsgs();
    
    void resetStandByMsg(){
      standByMsgCounter = 0;
    }

    void resetDisplay(){
      
    }

    void setupDisplay(){
      displayController.lcdInitialize();
      displayController.setDisplayPosition(0,1,"Safety Starts From");
      displayController.setDisplayPosition(1,6,"A.T.A.S");
      displayController.setDisplayPosition(2,0,"Please wait...");
      displayController.setDisplayPosition(2,0,"System Initializing.");
    }

    void setupScreen(String message){
      displayController.clearAll();
      displayController.setDisplayPosition(0,1,"Safety Starts From");
      displayController.setDisplayPosition(1,6,"A.T.A.S");
      displayController.setDisplayPosition(2,0,"Please wait...");
      displayController.setDisplayPosition(3,0,message);
    }

  private:

    Display displayController;
    
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
    int standByMsgCounter = 0;
    long displayResetTimer = 0;
    void displayDefault(int displayValue);
};

#endif
