#include "clock.h"
#include <stdbool.h>


initVariables()
{
    currentTime.tm_sec          = 0;
    currentTime.tm_min          = 0;
    currentTime.tm_hour         = 0;
    currentTime.tm_wday         = 0;
    currentTime.active          = false;
    
    alarm1.tm_sec               = 10;
    alarm1.tm_min               = 0;
    alarm1.tm_hour              = 0;
    alarm1.tm_wday              = 0;
    alarm1.active               = false;
        
    alarm2.tm_sec               = 7;
    alarm2.tm_min               = 0;
    alarm2.tm_hour              = 0;
    alarm2.tm_wday              = 8;
    alarm2.active               = false;
    
    pomodoro.tm_sec             = 30;
    pomodoro.tm_min             = 0;
    pomodoro.tm_hour            = 0;
    pomodoro.tm_wday            = 0;
    pomodoro.active             = false;
}
