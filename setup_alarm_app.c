/**************************************************************************//**
 * @file setup_alarm_app.c
 * @brief this file handles everything in set alarm app, 
 * but alarm check and run functions are in clock.c(time_management)
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version 
 ******************************************************************************/


#include "buttons.h"
#include "setup_time_app.h"
#include "clock.h"
#include "segmentlcd.h"

#define FIRST_SETUP_STATE       minutes
#define LAST_SETUP_STATE        active


static struct tm * p_alarm = &alarm1;


void drawSetupAlarmScreen(void);
void nextAlarmSetupState(void);


/********************************************//**
 * \brief state machine handles buttons, show information on the screen,
 * clock_setup_state is shared with setup_time_app
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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


/********************************************//**
 * \brief switch states in safe way, check for overflow
 *
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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


/********************************************//**
 * \brief draw current alarm information on the screen,
 * current version use segment LCD
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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


