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
#include "time_management.h"
#include "activity_app.h"
#include "main_screen_app.h"


volatile enum state_machine state = main_screen;



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
