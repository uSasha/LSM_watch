#define FIRST_STATE     main_screen
#define LAST_STATE      alarm_setup_screen


enum  state_machine
{
    main_screen,
    pomodoro_screen,
    activity_screen,
    time_setup_screen,
    alarm_setup_screen
};

extern volatile enum state_machine state;


void mainScreenApp(void);
void activityApp(void);
void setupTimeApp(void);
void nextState(void);
void previousState(void);
