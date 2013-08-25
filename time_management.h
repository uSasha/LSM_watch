#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED



#endif // CLOCK_H_INCLUDED


#include "bsp.h"


extern struct tm currentTime;
extern struct tm alarm1;
extern struct tm alarm2;
extern struct tm pomodoro;
extern bool screen_notification;        


struct tm
{
    /* date and time components */
    uint8_t tm_sec;
    uint8_t tm_min;
    uint8_t tm_hour;
    uint8_t tm_wday;
    bool active;  // only for alarms
};

extern enum setup_state_t clock_setup_state;
enum setup_state_t { seconds,
                     minutes,
                     hours,
                     week_days,
                     active
                   };


void initClock(void);
void incrementPoint(enum setup_state_t points, struct tm *something);
void decrementPoint(enum setup_state_t points, struct tm *something);
