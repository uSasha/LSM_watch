/**************************************************************************//**
 * @file main_screen_app.c
 * @brief handles all related to main screen: drawing etc.
 * 
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version 
 ******************************************************************************/
#include "time_management.h"
#include "activity_app.h"
#include "buttons.h"

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
