#include "LightVest2.h"
#include "utils.h"
#include "audio.h"

void ReadMic(uint32_t level)
{
	unsigned long startMillis = millis();  // Start of sample window

    uint32_t cSamples = 0;

	uint16_t signalMax = 0;
	uint16_t signalMin = 1023;

	// collect data for length of sample window (in mS)
	while (millis() - startMillis < dtmMicSample)
	{
		uint16_t sample = analogRead(pnMic);
		if (sample < 1024)  // toss out spurious readings
		{
			if (sample > signalMax)
			{
				signalMax = sample;  // save just the max levels
			}
			else if (sample < signalMin)
			{
				signalMin = sample;  // save just the min levels
			}
		}
        cSamples++;
	}

    DebugPrintf("%d\n", cSamples);
}