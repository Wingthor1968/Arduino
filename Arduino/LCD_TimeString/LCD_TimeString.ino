/***
    A simple test sketch for the TimeString library.
    Author: Wingthor1968, 2020.
*/

/*** Includes ***/
#include <LiquidCrystal.h>
#include <TimeString.h>

/*** Constants ***/
const uint32_t DELAY = 1000;
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2; //LCD pins

/*** Primitive variables ***/
uint32_t timePassed_string = 0;
uint32_t timePassed_String = 0;
char row1[17] = {"00:00"};
char row2[17] = {};

/*** Objects ***/
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
TimeString timeStringLib;

void setup() {
  // init display
  lcd.begin(16, 2);
}


void loop() {
  int secs = 0;
  int mins = 0;
  String ts;
  // run for 5 minutes
  for (int i = 0; i < 60 * 5; i++) {
    //calculate seconds and minutes within one command
    secs = (++secs == 60) ? mins++ & 0 : secs;
    
    //get timeString and measure the duration of that operation
    timePassed_string = micros();
    ts = timeStringLib.toString(0, mins, secs);
    timePassed_string = micros() - timePassed_string;

    //display the timeString
    lcd.setCursor(0, 0);
    lcd.print(ts);
    lcd.setCursor(0, 1);
    lcd.print(String(timePassed_string) + "\344s");

    //and finally wait ~ one second
    delay(DELAY);
  }
}
