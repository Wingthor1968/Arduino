/*
  TimeString.cpp - Library for creating TimeStrings.
  Created by Thorsten Herrmann, 2020.
*/

#include "Arduino.h"
#include "TimeString.h"

TimeString::TimeString() {};


String lpad(unsigned int number, unsigned int digits) {
  String str = String("0000000000000000").substring(0, digits);
  int pos = digits - 1;
  while (pos >= 0) {
    str[pos--] = number % 10 + 48;
    number /= 10;
  }
  return str;
}

String TimeString::toString(unsigned int hour, unsigned int minute, unsigned int second) {
  if (hour > 23 || minute > 60 || second > 60) return "XX:XX:XX";
  String hourString = lpad(hour, 2);
  String minuteString = lpad(minute, 2);
  String secondString = lpad(second, 2);
  return hourString + ":" + minuteString + ":" + secondString;
}
