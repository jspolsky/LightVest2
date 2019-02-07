#ifndef _LED_H_
#define _LED_H_


#define FASTLED_INTERNAL
#include <FastLED.h>

// PUBLIC:
void InitLEDs(void);
void IncreaseBrightness(void);
void DecreaseBrightness(void);
void ShowAudioLevel(byte scaled, byte scaledPeak);
void ShowWayfinder(void);
void IncreaseWayfinderHue(void);
void DecreaseWayfinderHue(void);
void ShowNoGPSData(void);

// PRIVATE:
#define IxFromScaledLeft(scaledVal) ((scaledVal) + (cstripMain-cstripBack-cstripVU))
#define IxFromScaledRight(scaledVal) ((cstripVU-1) - (scaledVal))
#define IxRing(x) (((x) + 3) % cstripRing)

#endif
