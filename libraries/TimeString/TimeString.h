/*
  TimeString.h - Library for creating TimeStrings.
  Author: Wingthor1968, 2020.
*/

#ifndef TimeString_h
#define TimeString_h

#include "Arduino.h"

class TimeString
{
  public:
    TimeString();
    String toString(unsigned int hour, unsigned int minute, unsigned int second);
};

#endif