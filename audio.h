#ifndef _AUDIO_H_
#define _AUDIO_H_

// PUBLIC:
void ReadMic(uint32_t level);

// GLOBALS:
extern byte scaled;             // Current level of column (0 .. cStripVU)    
extern byte scaledPeak;         // Peak level of column (used for falling dots) (0 .. cStripVU) 

// PRIVATE:
float dbScale(float signalMax, float signalMin, float dbFloor);

#endif
