/***
* PixelEffects is a collection of effects for NeoPixel like LED stripes.
*
* Author: Wingthor1968
* License: GPLv3 (see LICENSE file)
***/

#ifndef TimeString_h
#define TimeString_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class PixelEffects
{
  public:
    PixelEffects(Adafruit_NeoPixel pixels, unsigned int pixelcount);
    void sunrise(unsigned long totaltime);
    void sunset(unsigned long totaltime);
  private:
    unsigned int _pixelcount;
	Adafruit_NeoPixel _pixels;
};

#endif