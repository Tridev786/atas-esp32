#ifndef Display_h
#define Display_h
#include "Arduino.h"
#include <Wire.h>

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

class Display
{
  public:

    hd44780_I2Cexp lcd;
    
    Display();
    
    void lcdInitialize(){
      lcd.begin(20, 4); //20 columns and 4 rows
      lcd.init();
      lcd.backlight();
    }

    void displaySimpleChars(){
      lcd.setCursor(0, 0);
      // print message
      lcd.print("   P.Dharshinisri  ");
      delay(1000);
      // clears the display to print new message
      lcd.clear();
      // set cursor to first column, second row
      lcd.setCursor(0,1);
      lcd.print("  P.Tridev Vishkan      Version : 9.1");
      delay(1000);
      lcd.clear(); 
    }

  private:
      
};

#endif
