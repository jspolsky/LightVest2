#ifndef _LED_H_
#define _LED_H_

// PUBLIC:
void InitLEDs(void);
void IncreaseBrightness(void);
void DecreaseBrightness(void);
void ShowAudioLevel(byte scaled, byte scaledPeak);
void ShowWayfinder(void);
void ShowFlashlight(void);
void ShowGPSOnly(void);
void IncreaseWayfinderHue(void);
void DecreaseWayfinderHue(void);
void ShowNoGPSData(void);
void ShowGPSData(double lat, double lng);
void TestGPSData(double lat, double lng, const char* pszDescription);
void SetVestFrontColor(byte ix, CRGB rgb);
void SetVestFrontColorDbl(byte ix, CRGB rgb);

// PRIVATE:
#define IxFromScaledLeft(scaledVal) ((scaledVal) + (cstripMain-cstripBack-cstripVU))
#define IxFromScaledRight(scaledVal) ((cstripVU-1) - (scaledVal))
#define IxRing(x) (((x) + 3) % cstripRing)

double DmFromMan(double lat, double lng);
double DmFromLatLng(double lat1, double long1, double lat2, double long2);
double BearingFromMan(double lat, double lng);
double BearingFromLatLng (double lat1, double lng1, double lat2, double lng2);

#endif
