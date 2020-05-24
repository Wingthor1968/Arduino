/***
* PixelEffects is a collection of effects for NeoPixel like LED stripes.
*
* Author: Wingthor1968
* License: GPLv3 (see LICENSE file)
***/

#include "Arduino.h"
#include "PixelEffects.h"

/***
* Constructor
***/
PixelEffects::PixelEffects(Adafruit_NeoPixel pixels, unsigned int pixelcount) {
  _pixels = pixels;
  _pixelcount = pixelcount;
};


/***
* Light up the Pixels from the middle to the outer ones. After <totaltime> all Pixels are turned on.
***/
void PixelEffects::curtain_open(unsigned long totaltime) {
  int offset = _pixelcount / 2;
  int delaymillis = totaltime / offset;
  for (int i = 1; i <= offset; i++) {
    _pixels.setPixelColor(offset - i, _pixels.Color(50, 50, 50));
    _pixels.setPixelColor(offset + i - 1, _pixels.Color(50, 50, 50));
    _pixels.show();
    delay(delaymillis);
  }
}


/***
 * Turn off the Pixels from the outer to the middle ones. After <totaltime> all Pixels are turned off.
 */
void PixelEffects::curtain_close(unsigned long totaltime) {
  int delaymillis = totaltime / (_pixelcount / 2);
  for (int i = 0; i < _pixelcount / 2; i++) {
    _pixels.setPixelColor(i, _pixels.Color(0, 0, 0));
    _pixels.setPixelColor(_pixelcount - 1 - i, _pixels.Color(0, 0, 0));
    _pixels.show();
    delay(delaymillis);
  }
}
