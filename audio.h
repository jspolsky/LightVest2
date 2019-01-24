#ifndef _AUDIO_H_
#define _AUDIO_H_

// PUBLIC:
void ReadMic(uint32_t level);

// PRIVATE:
float dbScale(float signalMax, float signalMin, float dbFloor);

#endif
