#ifndef Display_h
#define Display_h
#include "Arduino.h"
#include <Wire.h>

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

class Display
{
  public:
    Display();
    void lcdInitialize();
    void displayFirstLine(String data);
    void displaySecondLine(String data);
    void displayThirdLine(String data);
    void displayFourthLine(String data);
    void setDisplayPosition(int row, int column, String data);
    void clearAll() { lcd.clear(); }
  private:
    hd44780_I2Cexp lcd;
};

#endif
