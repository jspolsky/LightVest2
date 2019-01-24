#include <math.h>

#include "LightVest2.h"
#include "utils.h"
#include "audio.h"

void ReadMic(uint32_t level)
{
    // "level"  indicates a value read from the pin, 0..1023
    // "db"     is actual decibels (floating point)
    // "scaled" is a scaled level (number of physicial LEDs that will be lit up), 0..cstripVU

    static unsigned long tmStart = 0;       // each sample is dtmMicSample milliseconds long. This is when we started
	static uint16_t levelMax = 0;           // highest level seen during this sample period
	static uint16_t levelMin = 1023;        // lowest level seen during this sample period

    static byte scaledPeak = 0;             // Peak level of column (used for falling dots)
    static byte cycPeak = 0;                // To prevent the peak falling too fast, it only falls when this is 0
    static unsigned long tmAdjusted = 0;    // Last time we adjusted the gain (dbRange)
    static float dbMax = 0;                 // Max db recorded in the last second
    static float dbRange = 2.0;             // Current range of dbs which will be scaled to (0..cstripVU)

    if (tmStart == 0)
    {
        // start a new sample period 

        tmStart = millis();
        levelMax = 0;
        levelMin = 1023;
    }

    // Sampling - check if we have a new high or low and record it
    if (level < 1024)  // toss out spurious readings
    {
        if (level > levelMax)
        {
            levelMax = level;  // save just the max levels
        }
        else if (level < levelMin)
        {
            levelMin = level;  // save just the min levels
        }
    }

    // Is this sample over?
    if (millis() < tmStart + dtmMicSample)
        return;         // no - nothing to do until we collect more sample data.

    // Sample is complete.
    cycPeak = (cycPeak + 1) % 5;
    float db = dbScale(levelMax, levelMin, 0.5);
    dbMax = max(dbMax, db);
    byte scaled = min(cstripVU, 
                      floor(db / dbRange * cstripVU));

    // has 1 second elapsed since we last calculated scaling factor?
    if (tmAdjusted + 1000L < millis())
    {
        tmAdjusted = millis();
        dbRange = max(dbMax, 2.0);
        dbMax = 0.0;
    }

    if (scaled > scaledPeak)
        scaledPeak = scaled;
    else if (scaledPeak > 0 && cycPeak == 0)
        scaledPeak--;

    DebugPrintf("%d,%d\n", scaled, scaledPeak);

    tmStart = 0;        // force another sample to start next time we are called
}


/*
 *		Calculates decibels based on the peak-to-peak voltage 
 *		Subtracts dbFloor so as to return 0 when it's basically quiet
 */
float dbScale(float levelMax, float levelMin, float dbFloor)
{
	float db = 20.0 * log10( levelMax / levelMin ); 
    if (db < dbFloor)
        return 0;
    else
        return db;
}