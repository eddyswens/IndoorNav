#ifndef __BUTTON_H__
#define __BUTTON_H__

#define BUTTON_PRESSED 1UL
#define BUTTON_RELEASED 0UL

#define BUTTON_LONGPRESS_TICK 1000

typedef enum {buttonIdle=0, buttonShortPress, buttonLongPress} ButtonEvent;

void buttonInit(ButtonEvent initialEvent);

void buttonProcess();

ButtonEvent buttonGetState();

#endif //__BUTTON_H__
