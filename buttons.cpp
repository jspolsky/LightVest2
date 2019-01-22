#include "LightVest2.h"
#include "utils.h"
#include "buttons.h"

bool fbuttonStateCurr[5];       // the current reading from the input pin
bool fButtonStatePrev[5];   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long tmDebounce = 0L;  // the last time the output pin was toggled


void InitButtons() {

    for (int ix = 0; ix < 5; ix++)
    {
        fbuttonStateCurr[ix] = fButtonStatePrev[ix] = false;
    }

}

void HandleButtonClicks( bool fnewMode, bool fnewUp, bool fnewDown, bool fnewPrev, bool fnewNext ) {

    HandleButtonClick( millis(), fnewMode, 0, "Mode");
    HandleButtonClick( millis(), fnewUp, 1, "Up");
    HandleButtonClick( millis(), fnewDown, 2, "Down");
    HandleButtonClick( millis(), fnewPrev, 3, "Prev");
    HandleButtonClick( millis(), fnewNext, 4, "Next");
}

void HandleButtonClick( unsigned long tmNow, bool fnew, int ix, char* pszButtonName )
{

    if (fnew != fButtonStatePrev[ix])
        tmDebounce = tmNow;

    if ((tmNow - tmDebounce) > dtmDebounce)
    {
        if (fnew != fbuttonStateCurr[ix])
         {
             fbuttonStateCurr[ix] = fnew;
             if (fbuttonStateCurr[ix])
                DebugPrintf("%s at %u\n", pszButtonName, tmNow);
         }
    }

    fButtonStatePrev[ix] = fnew;

}