/**************************************************************************//**
 * @file setup_time_app.c
 * @brief this file handles everything in set alarm app, 
 * but alarm check and run functions are in time_management.c(time_management)
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version 
 ******************************************************************************/


#include "buttons.h"
#include "setup_time_app.h"
#include "bsp.h"
#include "time_management.h"
#include "state_machine.h"
#include "segmentlcd.h"
#include "em_emu.h"

#define FIRST_SETUP_STATE       minutes
#define LAST_SETUP_STATE        week_days

void nextTimeSetupState(void);

enum setup_state_t clock_setup_state;


/********************************************//**
 * \brief state machine handles buttons, show information on the screen,
 * clock_setup_state is shared with setup_alarm_app
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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


/********************************************//**
 * \brief switch states in safe way, check for overflow
 * the difference  between this function and the same one in setup_alarm_app.c 
 * is max and min conditions
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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
