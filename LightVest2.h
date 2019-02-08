#ifndef _LIGHTVEST2_H_
#define _LIGHTVEST2_H_

#include <Arduino.h>
#include <stdarg.h>

/*  Burning Man "Golden Spike" location https://innovate.burningman.org/datasets-page/
 *  10:30 in Black Rock City is North
 */

#define latMan 40.7864
#define lngMan -119.2065

// #define TEST_GPS_MODE

#define pnstripMain 5
#define pnstripRing 6

#define pnMic A0
#define pnMode A1
#define pnNext A2
#define pnDown A3
#define pnPrev A4
#define pnUp A5

#define pnBat A7

#define cstripRing 24    /* number of LEDs on ring */
#define cstripMain 72    /* number of LEDs in main vest */
#define cstripVU 16      /* number of LEDs to use for VU meter */
#define cstripBack 10    /* number of LEDs on the back of the jacket */

#define dtmDebounce 25L  /* debounce time for buttons */
#define dtmMicSample 10L /* milliseconds to sample audio */
#define cPeakFall 7      /* how slow the peak indicator should fall */

#define brightMax 60     /* how bright to allow the LEDs to get */
#define brightInc 3      /* how much to change the brightness on up/down click */

// Messages which can be sent to the message() function to 
// indicate that something has happened
typedef enum 
{ 
    M_BUTTON_MODE = 0,      // the five buttons must be 0-4
    M_BUTTON_UP = 1,
    M_BUTTON_DOWN = 2,
    M_BUTTON_PREV = 3,
    M_BUTTON_NEXT = 4,

    M_NEW_GPS_DATA,
    M_NEW_AUDIO_LEVEL
} message_t;

void message( message_t m );

// Different modes that the vest can be in
typedef enum
{
    VESTMODE_WAYFINDER = 0,
    VESTMODE_VU,
    VESTMODE_FLASHLIGHT,

    VESTMODE_FENCE                     /* not a real mode */
} vestmode_t;

#endif
