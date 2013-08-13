#include "buttons.h"
#include "setup_time_app.h"
#include "clock.h"
#include "segmentlcd.h"

#define FIRST_SETUP_STATE       minutes
#define LAST_SETUP_STATE        active

void drawSetupAlarmScreen(void);
void nextAlarmSetupState(void);

static struct tm * p_alarm = &alarm1;


void setupAlarmApp(void)
{
    if( button == BUTTON_A)
    {
        button = NO_BUTTON;
        nextState();
    }
    else if(button == BUTTON_B)
    {
        button = NO_BUTTON;
        nextAlarmSetupState();
    }
    else if(button == BUTTON_C)
    {
        button = NO_BUTTON;

        if(clock_setup_state == active)
        {
            if(p_alarm->active == true)
            {
                p_alarm->active = false;
            }
            else
            {
                p_alarm->active = true;
            }
        }
        else
        {
            incrementPoint(clock_setup_state, p_alarm);
        }
    }
    else
    {
        button = NO_BUTTON;
        drawSetupAlarmScreen();
        EMU_EnterEM2(true);
    }
}


void nextAlarmSetupState(void)
{
    if(clock_setup_state == LAST_SETUP_STATE)
    {
        clock_setup_state = FIRST_SETUP_STATE;
    }
    else
    {
        clock_setup_state++;
    }
}


void drawSetupAlarmScreen(void)
{
    SegmentLCD_NumberOff();
    if(p_alarm == &alarm1)
    {
        SegmentLCD_Write("Alarm_1");
    }
    else
    {
        SegmentLCD_Write("Alarm_2");
    }
}
