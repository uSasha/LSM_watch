#ifndef ACTIVITY_H_INCLUDED
#define ACTIVITY_H_INCLUDED


#endif // ACTIVITY_H_INCLUDED


#define ACTIVITY_PORT gpioPortC
#define ACTIVITY_PIN 4
#define ACTIVITY_COUNTER cmuClock_PCNT1


void initActivity(void);
void clearActivity(uint8_t wday);
void activityApp(void);

