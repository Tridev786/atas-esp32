#include "Display.h"

Display::Display(){
}

void Display::displayFirstLine(String data){
  lcd.setCursor(0,0);
  for(int i=0; i<=20; i++){
    lcd.print("");
  }
  lcd.print(data);
}

void Display::displaySecondLine(String data){
  lcd.setCursor(0,1);
  for(int i=0; i<=20; i++){
    lcd.print("");
  }
  lcd.print(data);
}

void Display::displayThirdLine(String data){
  lcd.setCursor(0,2);
  for(int i=0; i<=20; i++){
    lcd.print("");
  }
  lcd.print(data);
}

void Display::displayFourthLine(String data){
  lcd.setCursor(0,3);
  for(int i=0; i<=20; i++){
    lcd.print("");
  }
  lcd.print(data);
}

void Display::setDisplayPosition(int row, int column, String data){
  lcd.setCursor(column,row);
  for(int i=column; i<=20; i++){
    lcd.print("");
  }
  lcd.print(data);
}

void Display::lcdInitialize(){
  lcd.begin(20, 4); //20 columns and 4 rows
  lcd.init();
  lcd.backlight();
}
