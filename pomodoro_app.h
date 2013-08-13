#ifndef POMODORO_H_INCLUDED
#define POMODORO_H_INCLUDED



#endif // POMODORO_H_INCLUDED

//NOTE exclude all internal functions
#define POMODORO_WORK_TIME      25
#define SHORT_BRAKE_TIME        5
#define LONG_BRAKE_TIME         25
#define POMODOROS_TO_LONG_BRAKE 4

void updatePomodoroTime(uint8_t time);
void stopPomodoro(void);
void checkPomodoro(void);
void drawPomodoroScreen(void);
