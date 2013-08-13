#ifndef BUTTONS_H_INCLUDED
#define BUTTONS_H_INCLUDED

#endif // BUTTONS_H_INCLUDED


#define BUTTON_A_PORT   gpioPortB
#define BUTTON_A_PIN    9

#define BUTTON_B_PORT   gpioPortB
#define BUTTON_B_PIN    10

#define NO_BUTTON       0
#define BUTTON_A        1
#define BUTTON_B        2
#define BUTTON_C        3

#include "bsp.h"    //NOTE check is it really needed



extern volatile uint8_t button;


void initButtons(void);
