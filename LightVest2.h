#ifndef _LIGHTVEST2_H_
#define _LIGHTVEST2_H_

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <stdarg.h>

#define pnstripMain 5
#define pnstripRing 6

#define pnMode A1
#define pnNext A2
#define pnDown A3
#define pnPrev A4
#define pnUp A5

#define pnBat A7

#define cstripMain 72   /* number of LEDs in main vest */
#define cstripRing 24   /* number of LEDs on ring */
#define dtmDebounce 50L /* debounce time for buttons */

typedef uint32_t COLOR;

#endif
