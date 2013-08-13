#include "buttons.h"
#include "setup_time_app.h"
#include "bsp.h"
#include "clock.h"


#define FIRST_SETUP_STATE       minutes
#define LAST_SETUP_STATE        week_days


enum setup_state_t clock_setup_state;



void setupTimeApp(void)
{
    if( button == BUTTON_A)
    {
        button = NO_BUTTON;
        nextState();
    }
    else if(button == BUTTON_B)
    {
        button = NO_BUTTON;
        nextTimeSetupState();
    }
    else if(button == BUTTON_C)
    {
        button = NO_BUTTON;
        incrementPoint(clock_setup_state, &currentTime);
    }
    else
    {
        SegmentLCD_NumberOff();
        button = NO_BUTTON;
        SegmentLCD_Write("Setup_T");
        EMU_EnterEM2(true);
    }
}


void nextTimeSetupState(void)
{
    if(clock_setup_state == week_days)
    {
        clock_setup_state = minutes;
    }
    else
    {
        clock_setup_state++;
    }
}
