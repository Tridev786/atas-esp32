#include "Arduino.h"
#include "Communication.h"
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 18
#define FONA_TX 19
#define FONA_RST 4

SoftwareSerial fonaSS(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

Communication::Communication(){
}

bool Communication::setupGSMModule(){
  fonaSerial->begin(4800);
  if (! fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find A.T.A.S GSM Module"));
  }
}

uint8_t Communication::getIMEINumber(){
  // Print module IMEI number.
  Serial.println(F("Test SIM - retrieve IMEI number")); 
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print("Module IMEI: "); Serial.println(imei);
  }
  return imeiLen;
}

uint8_t Communication::getNetworkStatus(){
  uint8_t n = fona.getNetworkStatus();
  if (n == 0) Serial.println(F("Not registered"));
  if (n == 1) Serial.println(F("Registered (home)"));
  if (n == 2) Serial.println(F("Not registered (searching)"));
  if (n == 3) Serial.println(F("Denied"));
  if (n == 4) Serial.println(F("Unknown"));
  if (n == 5) Serial.println(F("Registered roaming"));

  return n;
}

bool Communication::beginSequence(){

  // Msg first user
  Serial.print("Current number is");
  Serial.println(currentNumber);
  String fetchedNumber = parameter.fetchNumber(currentNumber);

  if(fetchedNumber == ""){
    Serial.print("Next number is inactive: ");
    Serial.print(currentNumber);
    Serial.print(" , skipping to the next number: ");
    currentNumber = currentNumber + 1;
    Serial.println(currentNumber);
    return false;
  }
  
  Serial.print("Fetched number is: ");
  Serial.println(fetchedNumber);
  bool result = contactUser(fetchedNumber);

  if(result == true){
    Serial.println("Call has been attended");
    currentNumber = 0;
    
  }else{
    Serial.println("Call has not been attended");
    if(currentNumber == 9){
      currentNumber = 0;
    }
    currentNumber = currentNumber + 1;
  }
  return result;
}

bool Communication::contactUser(String fetchedNumber){
  delay(5000);
  Serial.print("SensorTriggered: ");
  Serial.println(sensorTriggered);
  if(sendSms(fetchedNumber, parameter.fetchMsgForNumber(currentNumber, sensorTriggered))){
    Serial.println("SMS sent successfully!!");
  }else{
    Serial.println("SMS could not be sent!!");
  }

  if(parameter.checkIfSecondMsgRequired(currentNumber)){
    delay(5000);
    if(sendSms(fetchedNumber, parameter.msg1)){
      Serial.println("Address as SMS sent successfully!!");
    }else{
      Serial.println("Address as SMS could not be sent!!");
    }

    delay(5000);
    if(sendSms(fetchedNumber, parameter.msg2)){
      Serial.println("Location as SMS sent successfully!!");
    }else{
      Serial.println("Location as SMS could not be sent!!");
    }
  }
  
  delay(parameter.intervalBtwnMsgToCall);
  
  bool callStat = makeCall(fetchedNumber);
   if(callStat) {
      Serial.println("Call could not be made. Trying to reconnect to the same caller.");
   }

   return callStat;
}

bool Communication::sendSms(String fetchedNumber, String msgToSend){
  char sendto[21], message[300];
  flushSerial();
  fetchedNumber.toCharArray(sendto, 20);
  msgToSend.toCharArray(message, 300);

  if (!fona.sendSMS(sendto, message)) {
    Serial.println(F("Failed"));
    return false;
  } else {
    Serial.println(F("Sent!"));
    return true;
  }
}

bool Communication::makeCall(String fetchedNumber){
  char callto[21];
  fetchedNumber.toCharArray(callto, 20);

  while(true){
    if(! checkIfReadyForCall()){
      Serial.println("GSM not ready for the call..Please wait...");
      delay(500); 
      continue;
    }
    Serial.println("GSM is ready for the call...");
    break;
  }

  long currentTime = millis();
  bool callStatus = true;
  while(true){

    long latestTime = millis();
    if(latestTime-currentTime >= parameter.durationToAttempt){
      Serial.println("Time has expired");
      Serial.println(parameter.durationToAttempt);
      callStatus = false;
      break;
    }
    
    if(! fona.callPhone(callto)){
      delay(500);
      Serial.println("Call failed...");
      continue;
    }
    Serial.println("Call success...");
    break;
  }

  if(! callStatus){ return false; }
   
  // check if picked up
  return checkIfCallPickedUp();

}

void Communication::flushSerial() {
  while (Serial.available())
    Serial.read();
}

bool Communication::checkIfReadyForCall(){
  int8_t callstat = fona.getCallStatus();
  if(callstat == 0){
    return true;
  }else if (callstat == 1){
    return false;
  }
}

bool Communication::checkIfCallPickedUp(){
  flushSerial();
  int checkIfCallAttended = 0;
  int count = 0;
  
  while(true){
    char readBuffer[180];
    int i = 0;
    fonaSerial->write("AT+CLCC\r\n");
    while (fona.available() > 0) {
      byte x = fona.read();
      if (x != 0x04)                 //0x04 code to mark the end of Text message
      {
         readBuffer[i] = x;
         i++;
      }
      else
      {
         i = 0;
      }                                          
    }
    Serial.println(readBuffer);
    if(strlen(readBuffer) == 0) { delay(500); continue; }
    String result;
    result = String(result + parseInformation(readBuffer));
    Serial.println(result.length());
    Serial.println(result);
     if (result.length()==0 ) { 
      Serial.println("Unknown");
     }
     if(isSpace(result.toInt())){
      Serial.println("Call not attended");
     }
   Serial.println(result.toInt());
   
    switch(result.toInt()){
      case 0:
        Serial.println("Call has been attended");
        /*
          1. Play the audio
          2. Check if still phone picked up
          3. Repeat audio again
          4. Set the trigger to 0
        */
        checkIfCallAttended = 2;
        break;
      case 1:
        Serial.println("Call could not be reached");
        // Cut the call in 5 seconds
        // Call the next person
        //numberToDial = numberToDial + 1;
        break;
      case 2:
        Serial.println("Call not attended");
        // Cut the call in 5 seconds
        // Call the next person
        if(checkIfCallAttended == 2){
          checkIfCallAttended = 3;
        }

        if(count >= 40){
          checkIfCallAttended = 0;
        }
        
        break;
      case 3:
      Serial.println("Ringing...");
        checkIfCallAttended = 1;
        break;
      default:
        Serial.println("Unknown");
        // Cut the call in 5 seconds
        // Call the next person
        Serial.println("Status unknown");
        break;
    }
    
    delay(1000);

    if(count >= 40 && checkIfCallAttended == 0){
      Serial.print("Count is: ");
      Serial.println(count);
      return false;
    }

   count = count + 1;
    if(checkIfCallAttended == 3){
      Serial.println("Call has been attended and cut...");
      return true;
    }
  }
}

/*
 * 3 - Ringing
 * 2 - Call not attended
 * 0 - Called Attended
 * 1 - Unreachable phone
 * 
 */
char Communication::parseInformation(char str[]){

  char delim = ',';
  char splittedValuesTT[10];
  splittedValuesTT[2] = 0;
  
  char* pch;
  pch = strtok (str,",");
  int i =0;
  while (pch != NULL) { 
     char tmp = *pch;
     splittedValuesTT[i] = tmp;
     pch = strtok(NULL, ","); 
     i++;
  } 
  //Serial.println("--------------");
  Serial.print("Status is --> "); Serial.println(splittedValuesTT[2]);
  //Serial.println("--------------");
  Serial.print("array length is --> "); Serial.println();
  Serial.println("--------------");
  if(strlen(splittedValuesTT) < 3){
    Serial.println("Empty recorded");
    return '2';
  }
  
  return splittedValuesTT[2];
}

