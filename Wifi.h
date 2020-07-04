#ifndef Wifi_h
#define Wifi_h

#include "Arduino.h"
#include <WiFi.h>

class Wifi
{
  public:
    Wifi();
    bool connectToWifi();
    String fetchIPAddress();
    bool checkConnection();
    bool setUsernamePassword(String _username, String _password);
    String IpAddress2String(IPAddress ipAddress);
  private:
    String username;
    String password;
};

#endif

