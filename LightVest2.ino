#include <Adafruit_NeoPixel.h>

/*
 * 
 * LED Light Vest 2.0
 * 
 * Requires:
 *      Adafruit Feather M0 Basic Prototype
 *      Five-way switch to control mode, color scheme, and brightness
 *      Adafruit Electret Microphone (https://www.adafruit.com/product/1063)
 *      Adafruit Ultimate GPS Featherwing (https://learn.adafruit.com/adafruit-ultimate-gps-featherwing)
 */


#define pinstripMain 5
#define pinstripRing 6 

#define cstripMain 72
#define cstripRing 24

typedef uint32_t COLOR;

Adafruit_NeoPixel stripMain = Adafruit_NeoPixel(cstripMain, pinstripMain, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripRing = Adafruit_NeoPixel(cstripRing, pinstripRing, NEO_GRB + NEO_KHZ800);

void setup() {

  stripMain.begin();
  stripMain.setBrightness(5);
  stripMain.show(); // Initialize all pixels to 'off'

  stripRing.begin();
  stripRing.setBrightness(5);
  stripRing.show(); // Initialize all pixels to 'off'

}

void loop() {

  COLOR c = stripMain.Color(0, 0, 255);
  
  for (int i = 0; i < cstripMain; i++)
    stripMain.setPixelColor(i, c);

  for (int i = 0; i < cstripRing; i++)
    stripRing.setPixelColor(i, c);

  stripMain.show();
  stripRing.show();

}
