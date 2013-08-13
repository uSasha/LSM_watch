#include "bsp.h"
#include "state_machine.h"
#include "buttons.h"
#include "clock.h"
#include "activity_app.h"

// !NOTE    make separate files for all APPs

volatile enum state_machine state = main_screen;


void mainScreenApp(void)
{
    if( button == BUTTON_A)
    {
        button = NO_BUTTON;
        nextState();
    }
    else if(button == BUTTON_B)
    {
        button = NO_BUTTON;
        previousState();
    }
    else
    {
        button = NO_BUTTON;
        SegmentLCD_Write("Main");
        EMU_EnterEM2(true);
    }
}


void nextState(void)
{
    if (state == LAST_STATE)
    {
        state = FIRST_STATE;
    }
    else
    {
        state++;
    }

    //TODO think about "clean globals function" or about pointers to functions
    clock_setup_state = 1;  // start with minutes by default
}


void previousState(void)
{
    if (state == FIRST_STATE)
    {
        state = LAST_STATE;
    }
    else
    {
        state--;
    }
}
