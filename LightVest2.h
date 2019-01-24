#ifndef _LIGHTVEST2_H_
#define _LIGHTVEST2_H_

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <stdarg.h>

#define pnstripMain 5
#define pnstripRing 6

#define pnMic A0
#define pnMode A1
#define pnNext A2
#define pnDown A3
#define pnPrev A4
#define pnUp A5

#define pnBat A7

#define cstripMain 72    /* number of LEDs in main vest */
#define cstripRing 24    /* number of LEDs on ring */
#define dtmDebounce 25L  /* debounce time for buttons */
#define dtmMicSample 10L /* milliseconds to sample audio */

typedef uint32_t COLOR;

// Messages which can be sent to the message() function to 
// indicate that something has happened
typedef enum 
{ 
    M_BUTTON_MODE = 0,      // the five buttons must be 0-4
    M_BUTTON_UP = 1,
    M_BUTTON_DOWN = 2,
    M_BUTTON_PREV = 3,
    M_BUTTON_NEXT = 4,

    M_NEW_GPS_DATA
} message_t;

void message( message_t m );

#endif
