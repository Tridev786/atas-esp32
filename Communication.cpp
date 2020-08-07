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
    parameter.setupScreen("GSM connected");
    Serial.println(F("Couldn't find A.T.A.S GSM Module"));
  }
}

uint8_t Communication::getIMEINumber(){
  // Print module IMEI number.
  Serial.println(F("Test SIM - retrieve IMEI number")); 
  char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
  uint8_t imeiLen = fona.getIMEI(imei);
  if (imeiLen > 0) {
    Serial.print(F("Module IMEI: ")); Serial.println(imei);
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
  Serial.print(F("Current number is"));
  Serial.println(currentNumber);
  String fetchedNumber = parameter.fetchNumber(currentNumber, sensorTriggered);

  if(fetchedNumber == ""){
    Serial.print(F("Next number is inactive: "));
    Serial.print(currentNumber);
    Serial.print(F(" , skipping to the next number: "));
    currentNumber = currentNumber + 1;
    Serial.println(currentNumber);
    return false;
  }
  
  Serial.print(F("Fetched number is: "));
  Serial.println(fetchedNumber);
  bool result = contactUser(fetchedNumber);

  if(result){
    Serial.println(F("Call has been attended"));
    currentNumber = 0;
  }else{
    Serial.println(F("Call has not been attended"));
    if(currentNumber == 9){
      currentNumber = 0;
    }
    currentNumber = currentNumber + 1;
  }
  return result;
}

bool Communication::contactUser(String fetchedNumber){
  delay(5000);
  if(sendSms(fetchedNumber, parameter.fetchMsgForNumber(currentNumber, sensorTriggered))){
    Serial.println(F("SMS sent successfully!!"));
  }else{
    Serial.println(F("SMS could not be sent!!"));
  }

  if(parameter.checkIfSecondMsgRequired(currentNumber)){
    delay(5000);
    if(sendSms(fetchedNumber, parameter.msg1)){
      Serial.println(F("Address sent!!"));
    }else{
      Serial.println(F("Address not sent!!"));
    }

    delay(5000);
    if(sensorTriggered == 2){
      if(sendSms(fetchedNumber, parameter.msg3)){
      Serial.println(F("Location sent!!"));
      }else{
        Serial.println(F("Location not sent!!"));
      }
    }else{
      if(sendSms(fetchedNumber, parameter.msg2)){
      Serial.println(F("Location sent!!"));
      }else{
        Serial.println(F("Location not sent!!"));
      }
    }
  }
  
  delay(parameter.intervalBtwnMsgToCall);
  
  bool callStat = makeCall(fetchedNumber);
   if(callStat) {
      Serial.println(F("Call could not be made. Trying to reconnect to the same caller."));
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

//  while(true){
//    if(! checkIfReadyForCall()){
//      Serial.println(F("GSM not ready for the call..Please wait..."));
//      delay(500); 
//      continue;
//    }
//    Serial.println(F("GSM is ready for the call..."));
//    break;
//  }

  long currentTime = millis();
  bool callStatus = true;
  while(true){

    long latestTime = millis();
    if(latestTime-currentTime >= parameter.durationToAttempt){
      callStatus = false;
      break;
    }
    
    if(! fona.callPhone(callto)){
      delay(500);
      Serial.println(F("Call failed..."));
      continue;
    }
    Serial.println(F("Call success..."));
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
    //Serial.println(readBuffer);
    if(strlen(readBuffer) == 0) { delay(500); continue; }
    String result;
    result = String(result + parseInformation(readBuffer));
    //Serial.println(result.length());
    //Serial.println(result);
     if (result.length()==0 ) { 
      Serial.println(F("Unknown"));
     }
     if(isSpace(result.toInt())){
      Serial.println(F("Call not attended"));
     }
   //Serial.println(result.toInt());
   
    switch(result.toInt()){
      case 0:
        Serial.println(F("Call has been attended"));
        /*
          1. Play the audio
          2. Check if still phone picked up
          3. Repeat audio again
          4. Set the trigger to 0
        */
        checkIfCallAttended = 2;
        break;
      case 1:
        Serial.println(F("Call could not be reached"));
        // Cut the call in 5 seconds
        // Call the next person
        //numberToDial = numberToDial + 1;
        break;
      case 2:
        Serial.println(F("Call not attended"));
        // Cut the call in 5 seconds
        // Call the next person
        if(checkIfCallAttended == 2){
          checkIfCallAttended = 3;
          break;
        }

        if(count >= 40){
          Serial.println(F("here"));
          checkIfCallAttended = 0;
        }
        
        break;
      case 3:
      Serial.println(F("Ringing..."));
        checkIfCallAttended = 1;
        break;
      default:
        Serial.println(F("Unknown"));
        // Cut the call in 5 seconds
        // Call the next person
        Serial.println(F("Status unknown"));
        break;
    }
    
    delay(1000);

    Serial.print("Count is: ");
    Serial.println(count);

    if(count >= 40 && checkIfCallAttended == 0){
      Serial.print("Count is: ");
      Serial.println(count);
      return false;
    }

   count = count + 1;
    if(checkIfCallAttended == 3){
      Serial.println(F("Call has been attended and cut..."));
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
  //Serial.print("Status is --> "); Serial.println(splittedValuesTT[2]);
  //Serial.println("--------------");
  //Serial.print("array length is --> "); Serial.println();
  //Serial.println("--------------");
  if(strlen(splittedValuesTT) < 3){
    Serial.println(F("Empty recorded"));
    return '2';
  }
  
  return splittedValuesTT[2];
}


