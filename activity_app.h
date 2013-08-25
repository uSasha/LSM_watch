#ifndef ACTIVITY_H_INCLUDED
#define ACTIVITY_H_INCLUDED


#endif // ACTIVITY_H_INCLUDED


#define ACTIVITY_PORT           gpioPortC
#define ACTIVITY_PIN            4
#define ACTIVITY_COUNTER        cmuClock_PCNT1

#define ACCEL_USART             USART1
#define CMUCLOCK_ACCEL_USART    cmuClock_USART1
#define ACCEL_CS_PORT           gpioPortD
#define ACCEL_CS_PIN            3
#define ACCEL_MOSI_PORT         gpioPortD
#define ACCEL_MOSI_PIN          0
#define ACCEL_MISO_PORT         gpioPortD
#define ACCEL_MISO_PIN          1
#define ACCEL_CLK_PORT          gpioPortD
#define ACCEL_CLK_PIN           2
#define ACCER_USART_LOC         USART_ROUTE_LOCATION_LOC1


void initActivity(void);
void clearActivity(uint8_t wday);
void activityApp(void);

