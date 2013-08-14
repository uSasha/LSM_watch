/**************************************************************************//**
 * @file pomodoro_app.c
 * @brief handles all pomodoro timer application, 
 * more info: http://en.wikipedia.org/wiki/Pomodoro_Technique
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version 
 ******************************************************************************/


#include "bsp.h"
#include "clock.h"
#include "state_machine.h"
#include "buttons.h"
#include "pomodoro_app.h"
#include "segmentlcd.h"


bool pomodoro_timeout = false;

enum pomodoro_state_t
{
    pomodoro_off,
    pomodoro_active,
    pomodoro_short_brake,
    pomodoro_long_brake
};
static enum pomodoro_state_t pomodoro_state = pomodoro_off;


/********************************************//**
 * \brief state machine which switch pomodoro_state and show info on the screen
 *
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
void pomodoroApp(void)
{
    static uint8_t pomodoros = 0;

    switch(pomodoro_state)
    {
    case pomodoro_off:
        if(button == BUTTON_B)
        {
            button = NO_BUTTON;
            updatePomodoroTime(POMODORO_WORK_TIME);
            pomodoro_state = pomodoro_active;
        }
        break;
    case pomodoro_active:
        if(button == BUTTON_B)
        {
            button = NO_BUTTON;
            pomodoros = 0;
            stopPomodoro();
        }
        if(pomodoro_timeout == true)
        {
            pomodoro_timeout = false;
            pomodoros++;
            if(pomodoros >= POMODOROS_TO_LONG_BRAKE)  //TODO check number of pomodoros till long break
            {
                pomodoros = 0;
                updatePomodoroTime(LONG_BRAKE_TIME);
                pomodoro_state = pomodoro_long_brake;
            }
            else
            {
                updatePomodoroTime(SHORT_BRAKE_TIME);
                pomodoro_state = pomodoro_short_brake;
            }
        }
        break;
    case pomodoro_short_brake:
        if(button == BUTTON_B)
        {
            button = NO_BUTTON;
            stopPomodoro();
        }
        if(pomodoro_timeout == true)
        {
            pomodoro_timeout = false;
            updatePomodoroTime(POMODORO_WORK_TIME);
            pomodoro_state = pomodoro_active;
        }
        break;
    case pomodoro_long_brake:
        if(button == BUTTON_B)
        {
            button = NO_BUTTON;
            stopPomodoro();
        }
        if(pomodoro_timeout == true)
        {
            pomodoro_timeout = false;
            updatePomodoroTime(POMODORO_WORK_TIME);
            pomodoro_state = pomodoro_active;
        }
        break;
    }

    if( button == BUTTON_A)
    {
        button = NO_BUTTON;
        nextState();
    }
    else
    {
        button = NO_BUTTON;
        drawPomodoroScreen();
        EMU_EnterEM2(true);
    }
}


/********************************************//**
 * \brief show current pomodoro conter state and remaining time on the screen,
 * current version use segment LCD screen
 * \param 
 * \param 
 * \return 
 * 
 ***********************************************/      
void drawPomodoroScreen(void)
{
    switch(pomodoro_state)
    {
    case pomodoro_active:
        SegmentLCD_Write("Work");
        break;
    case pomodoro_short_brake:
        SegmentLCD_Write("Break");
        break;
    case pomodoro_long_brake:
        SegmentLCD_Write("Break");
        break;
    case pomodoro_off:
        SegmentLCD_Write("OFF");
        break;
    }
    int8_t time_before_next_state = pomodoro.tm_min - currentTime.tm_min;
    if(time_before_next_state < 0)
    {
        time_before_next_state += 59;
    }
    if(pomodoro_state != pomodoro_off)
    {
        SegmentLCD_Number(time_before_next_state);
    }
    else
    {
        SegmentLCD_NumberOff();
    }
}


/********************************************//**
 * \brief set time to switch pomodoro state, similar to alarm
 * turn timer on
 * \param uint8_t time  - shows how long this state will be active
 * \param 
 * \return 
 *
 ***********************************************/      
void updatePomodoroTime(uint8_t time)
{
    pomodoro.active = true;
    pomodoro.tm_sec = currentTime.tm_sec;

    if(currentTime.tm_min + time >= 59)    // 60min - 25min - 1(0 <= min <= 59)
    {
        pomodoro.tm_min = currentTime.tm_min + time - 59;
        pomodoro.tm_hour = currentTime.tm_hour;
        incrementPoint(hours, &pomodoro);   // safe update hours with check for overflow
        pomodoro.tm_wday = 0;   // don't care
    }
    else
    {
        pomodoro.tm_min = currentTime.tm_min + time;
        pomodoro.tm_hour = currentTime.tm_hour;
    }
}


/********************************************//**
 * \brief turn pomodoro timer off
 *
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
void stopPomodoro(void)
{
    pomodoro_state = pomodoro_off;  // TODO maybe replace .active with _state
    pomodoro.active = false;
}


/********************************************//**
 * \brief check how much current state will be active,
 * if it's time to switch to another state show pomodoro screen with notification,
 * can switch to this screen from another applications
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
void checkPomodoro(void)
{
    if(pomodoro.active &&
            (pomodoro.tm_sec <= currentTime.tm_sec) &&
            (pomodoro.tm_min <= currentTime.tm_min) &&
            (pomodoro.tm_hour == currentTime.tm_hour))   // exclude 00:01 < 23:59 situation
    {
        pomodoro_timeout = true;
        state = pomodoro_screen;
    }
}
