#include "LightVest2.h"
#include "utils.h"
#include "buttons.h"

bool fbuttonStateCurr[5]; // the current reading from the input pin
bool fButtonStatePrev[5]; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long tmDebounce = 0L; // the last time the output pin was toggled

void InitButtons()
{
    for (int ix = 0; ix < 5; ix++)
    {
        fbuttonStateCurr[ix] = fButtonStatePrev[ix] = false;
    }
}

void HandleButtonClicks(bool fnewMode, bool fnewUp, bool fnewDown, bool fnewPrev, bool fnewNext)
{
    unsigned long tm = millis();

    HandleButtonClick(tm, fnewMode, 0);
    HandleButtonClick(tm, fnewUp, 1);
    HandleButtonClick(tm, fnewDown, 2);
    HandleButtonClick(tm, fnewPrev, 3);
    HandleButtonClick(tm, fnewNext, 4);
}

void HandleButtonClick(unsigned long tmNow, bool fnew, int ix)
{
    if (fnew != fButtonStatePrev[ix])
        tmDebounce = tmNow;

    if ((tmNow - tmDebounce) > dtmDebounce)
    {
        if (fnew != fbuttonStateCurr[ix])
        {
            fbuttonStateCurr[ix] = fnew;
            if (fbuttonStateCurr[ix])
                message((message_t) ix);
        }
    }

    fButtonStatePrev[ix] = fnew;
}
