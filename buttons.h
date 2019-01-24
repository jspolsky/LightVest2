#ifndef _BUTTONS_H_
#define _BUTTONS_H_

// PUBLIC:
void InitButtons();
void HandleButtonClicks(bool fnewMode, bool fnewUp, bool fnewDown, bool fnewPrev, bool fnewNext);

// PRIVATE:
void HandleButtonClick(unsigned long tmNow, bool fnew, int ix);

#endif
