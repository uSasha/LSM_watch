Lifestyle Monitoring Watch.


This project was started in autumn 2012 for Energy Micro design contest. (more info:
http://forum.energymicro.com/topic/309-dc12-s1-31-lifestyle-monitoring-wrist-watch/#entry883

http://forum.energymicro.com/topic/577-dc12-s2-9-lifestyle-monitoring-wrist-watch/#entry2325 )

The goal is to make a wrist watch, which can monitor your activity and make you suggestions.
Also I dicided to include pomodoro timer functionality.
the long distanse goal is to add sleep quality monitoring and run alarm in non active sleep phase.

The main goal of this project is to design software.
There is a compatible beautifull hardware design was made by another member of Lizard Lounge - hairykiwi(more info:
http://forum.energymicro.com/topic/582-otm-02-open-source-time-machine-2/ )

Software is designed with easy expandability in mind:
all functionality is separated in different apps():
setup_time_app
setup_alarm_app
pomodoro_app
activity_app
and kernel, that handles app swiching and time keeping, so everybody can add any functionality to this project without whole system understanding.

Development is made in Em::Blocks opensorce IDE(more info:
http://www.emblocks.org/web/ )


Board:  Energy Micro EFM32GG_STK3700 Starter Kit
Device: EFM32GG990F1024
