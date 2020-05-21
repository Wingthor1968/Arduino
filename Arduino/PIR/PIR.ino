/***
   Evaluate a HC-SR501 PIR sensor: turn on a LED if the sensors detects motion. Keep the LED on for
   additional TIMER milliseconds. Checks the PIR state ever DELAY milliseconds.
   Author: Wingthor1968
 ***/

/*** Constants ***/
const unsigned int INPUTPIN = 7; //PIR sensor
const unsigned int LEDPIN = 10; //LED
const unsigned long DELAY = 50; //milliseconds
const unsigned long TIMER = 15000; //milliseconds

/*** Primitive variables ***/
long timer = 0;


void setup() {
  pinMode(INPUTPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
  Serial.begin(9600);
}


void loop() {
  if (digitalRead(INPUTPIN) == HIGH) {
    //reset the countdown timer if motion is detected
    timer = TIMER;
  } else {
    //count down
    timer =  timer > 0 ? timer - DELAY : 0;
  }
  Serial.println("Timer: " + String(timer));
  //decide whether to turn LED on or off
  digitalWrite(LEDPIN, ((timer > 0) ? HIGH : LOW));
  delay(DELAY);
}
