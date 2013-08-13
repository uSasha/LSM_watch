#include "em_pcnt.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "clock.h"
#include "activity_app.h"
#include "buttons.h"


uint8_t activity[7];

void initCounter(void)
{
    /* Enabling all necessary clocks */
    CMU_ClockEnable(cmuClock_CORELE, true);     /* Enable CORELE clock */
    CMU_ClockEnable(cmuClock_GPIO, true);       /* Enable clock for GPIO module */
    CMU_ClockEnable(cmuClock_PCNT1, true);      /* Enable clock for PCNT module */


    GPIO_PinModeSet(ACTIVITY_PORT, ACTIVITY_PIN, gpioModeInputPull, 0);

    /* Set configuration for pulse counter */
    PCNT_Init_TypeDef pcntInit =
    {
        .mode       = pcntModeOvsSingle,  /* clocked by LFACLK */
        .counter    = 0,                  /* Set initial value to 0 */
        .top        = 10,                 /* Set top to max value */
        .negEdge    = false,              /* positive edges */
        .countDown  = false,              /* up count */
        .filter     = false,               /* filter enabled */
    };

    /* Initialize Pulse Counter */
    PCNT_Init(PCNT1, &pcntInit);

    /* Enable PCNT overflow interrupt */
    PCNT_IntEnable(PCNT1, 0x2);

    /* Enable PCNT1 interrupt vector in NVIC */
    NVIC_EnableIRQ(PCNT1_IRQn);

    /* Route PCNT1 input to location 0 -> PCNT1_S0IN on PC4 */
    PCNT1->ROUTE = PCNT_ROUTE_LOCATION_LOC0;
}

void initActivity(void)
{
    initCounter();

    // init activity variable
    uint8_t i;
    for(i = 0; i < 7; i++)
    {
        activity[i] = 0;
    }
}

void clearActivity(uint8_t wday)
{
    activity[wday] = 0;
}


void drawActivityScreen(void)
{
    SegmentLCD_NumberOff();
    SegmentLCD_Write("Actvty");
    SegmentLCD_Number(activity[currentTime.tm_wday]);
}

void activityApp(void)
{
    if( button == BUTTON_A)
    {
        button = NO_BUTTON;
        nextState();
    }
    else if(button == BUTTON_B)
    {
        button = NO_BUTTON;
        previousState();
    }
    else
    {
        button = NO_BUTTON;
        drawActivityScreen();
        EMU_EnterEM2(true);
    }
}


/**************************************************************************//**
 * @brief PCNT1_IRQHandler
 * Interrupt Service Routine for PCNT1 Interrupt Line
 *****************************************************************************/
void PCNT1_IRQHandler(void)
{
    /* Clear PCNT1 overflow interrupt flag */
    PCNT_IntClear(PCNT1, 0x2);

    activity[currentTime.tm_wday]++;
}
