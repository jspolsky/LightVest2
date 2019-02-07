#ifndef _LED_H_
#define _LED_H_


#define FASTLED_INTERNAL
#include <FastLED.h>

// PUBLIC:
void InitLEDs(void);
void ShowAudioLevel(byte scaled, byte scaledPeak);

// PRIVATE:
#define IxFromScaledLeft(scaledVal) (scaledVal + (cstripMain-cstripBack-cstripVU))
#define IxFromScaledRight(scaledVal) ((cstripVU-1) - scaledVal)

#endif
