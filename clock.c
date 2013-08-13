#include "em_rtc.h"
#include "em_cmu.h"
#include "clock.h"
#include "pomodoro_app.h"
#include "segmentlcd.h"
#include "activity_app.h"

//#define RTC_COUNT_BETWEEN_WAKEUP    59      // interrupt every 60 seconds
#define RTC_COUNT_BETWEEN_WAKEUP    1      // interrupt every 2 seconds


struct tm currentTime;
struct tm alarm1;
struct tm alarm2;
struct tm pomodoro;
bool screen_notification = false;


void initClock(void)
{
    // set RTC to generate interrupts every second time and date will be maintained by software
    /* Starting LFXO and waiting until it is stable */
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);

    /* Routing the LFXO clock to the RTC */
    CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
    CMU_ClockEnable(cmuClock_RTC, true);
    /* Enabling clock to the interface of the low energy modules */
    CMU_ClockEnable(cmuClock_CORELE, true);

    /* Setting up RTC */
    /* Prescaler of 15 = 1 s of resolution and overflow each 194 days */
    CMU_ClockDivSet(cmuClock_RTC,cmuClkDiv_32768);
    RTC_CompareSet(0, RTC_COUNT_BETWEEN_WAKEUP);
    RTC_IntEnable(RTC_IFC_COMP0);

    /* Enabling Interrupt from RTC */
    NVIC_EnableIRQ(RTC_IRQn);

    RTC_Init_TypeDef rtcInit = RTC_INIT_DEFAULT;
    RTC_Init(&rtcInit);
}


void RTC_IRQHandler(void)
{
    RTC_IntClear(RTC_IF_COMP0);
    updateTime();

    uint8_t alarm = checkAlarms();
    if(alarm)
    {
        runAlarm(alarm);        // check alarms and run if there are any
    }

    checkPomodoro();
}


void updateTime(void)
{
    incrementPoint(seconds, &currentTime);
}


uint8_t checkAlarms(void)
{
    if((alarm1.tm_sec == currentTime.tm_sec) &&
            (alarm1.tm_min == currentTime.tm_min) &&
            (alarm1.tm_hour == currentTime.tm_hour) &&
            ((alarm1.tm_wday == currentTime.tm_wday) || alarm1.tm_wday == 8) && // day 8 means everyday alarm
            (alarm1.active))
    {
        return 1;
    }

    if((alarm2.tm_sec == currentTime.tm_sec) &&
            (alarm2.tm_min == currentTime.tm_min) &&
            (alarm2.tm_hour == currentTime.tm_hour) &&
            ((alarm2.tm_wday == currentTime.tm_wday) || alarm2.tm_wday == 8) && // day 8 means everyday alarm
            (alarm2.active))
    {
        return 2;
    }

    return 0;       // there is no alarms must be ring
}


void runAlarm(uint8_t alarm)
{
    switch (alarm)
    {
    case 1:
        alarm = 0;
        alarm1.active = false;
        SegmentLCD_Write("ALARM 1");
        screen_notification = true;
        break;
    case 2:
        alarm = 0;
        alarm2.active = false;
        SegmentLCD_Write("ALARM 2");
        screen_notification = true;
        break;
    }
}


void incrementPoint(enum setup_state_t points, struct tm *something)
{
    switch (points)
    {
    case seconds:
        something->tm_sec++;
        something->tm_sec++;        //NOTE workaround for 2sec counter
        if (something->tm_sec < 60)
        {
            break;
        }
        else
        {
            something->tm_sec = 0;
        }
    case minutes:
        something->tm_min++;
        if (something->tm_min < 60)
        {
            break;
        }
        else
        {
            something->tm_min = 0;
        }
    case hours:
        something->tm_hour++;
        if (something->tm_hour < 24)
        {
            break;
        }
        else
        {
            something->tm_hour = 0;
        }
    case week_days:
        something->tm_wday++;
        something->tm_wday %= 7;
        clearActivity(something->tm_wday);
//      activity[calendar.tm_wday] = 0;   // delete this day activity, othervice we will increment last week activity
        break;
    case active:        // only for alarms
        if(something->active)
        {
            something->active = false;
        }
        else
        {
            something->active = true;
        }
    }
}


void decrementPoint(enum setup_state_t points, struct tm *something)
{
    switch (points)
    {
    case seconds:
        something->tm_sec--;
        if (something->tm_sec >= 0)
        {
            break;
        }
        else
        {
            something->tm_sec = 59;
        }
    case minutes:
        something->tm_min--;
        if (something->tm_min >= 0)
        {
            break;
        }
        else
        {
            something->tm_min = 59;
        }
    case hours:
        something->tm_hour--;
        if (something->tm_hour >= 0)
        {
            break;
        }
        else
        {
            something->tm_hour = 23;
        }
    case week_days:
        something->tm_wday--;
        if (something->tm_wday < 0)
        {
            something->tm_wday = 6;
        }
//      activity[calendar.tm_wday] = 0;   // delete this day activity, othervice we will increment last week activity
        break;
    }
}
