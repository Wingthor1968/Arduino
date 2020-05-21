/*
  TimeString.h - Library for creating TimeStrings.
  Created by Thorsten Herrmann, 2020.
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