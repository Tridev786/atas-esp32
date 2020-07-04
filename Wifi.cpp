#include "Arduino.h"
#include "Wifi.h"
#include <WiFi.h>

Wifi::Wifi(){
  
}

bool Wifi::connectToWifi(){
  Serial.println(username);
  Serial.println(password);
  WiFi.begin(username.c_str(),password.c_str());
  int attempt = 0;
  Serial.println("Connecting");
  while(true){
    if(WiFi.status() == WL_CONNECTED){
      Serial.print("Connection success in ");
      Serial.print(attempt);
      Serial.println(" tries");

      Serial.println(fetchIPAddress());
      return true;
    }

    if(attempt>=9){
      return false;
    }
    attempt++;
    delay(500);
  }
  return true;
}

String Wifi::fetchIPAddress()
{
  if(WiFi.status() != WL_CONNECTED){
    return "0";
  }
  return IpAddress2String(WiFi.localIP());
}

bool Wifi::checkConnection()
{
  if(WiFi.status() != WL_CONNECTED){
    return false;
  }
  return true;
}

bool Wifi::setUsernamePassword(String _username, String _password)
{
  username = _username;
  password = _password;

  return connectToWifi();
}

String Wifi::IpAddress2String(IPAddress ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}
