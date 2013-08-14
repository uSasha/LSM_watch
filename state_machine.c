/**************************************************************************//**
 * @file state_machine.c
 * @brief root menu state machine handling
 * 
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version 
 ******************************************************************************/


#include "bsp.h"
#include "state_machine.h"
#include "buttons.h"
#include "clock.h"
#include "activity_app.h"

// !NOTE    make separate files for all APPs

volatile enum state_machine state = main_screen;


/********************************************//**
 * \brief the screen which is active by default or after some time without user actions,
 * show main screen, handle buttons
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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


/********************************************//**
 * \brief increment states in safe way, check for overflow
 *
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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


/********************************************//**
 * \brief decrement states in safe way, check for underflow
 *
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
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
