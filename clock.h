#ifndef CLOCK_H_INCLUDED
#define CLOCK_H_INCLUDED



#endif // CLOCK_H_INCLUDED


#include "bsp.h"


extern struct tm currentTime;
extern struct tm alarm1;
extern struct tm alarm2;
extern struct tm pomodoro;
extern bool screen_notification;        //NOTE is it the right place?


struct tm
{
    /* date and time components */
    int tm_sec;
    int tm_min;
    int tm_hour;
    int tm_wday;
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
void updateTime(void);
void incrementPoint(enum setup_state_t points, struct tm *something);
void decrementPoint(enum setup_state_t points, struct tm *something);

void runAlarm(uint8_t alarm);
uint8_t checkAlarms(void);
