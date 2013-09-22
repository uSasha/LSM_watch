/**************************************************************************//**
 * @file pedometer.c
 * @brief count steps based on acceleration peaks
 * based on Analog Devices Application Note http://www.analog.com/library/analogdialogue/archives/44-06/pedometer.pdf
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version
 ******************************************************************************/

#include "em_letimer.h"
#include "em_cmu.h"
#include "em_letimer.h"
#include "activity_app.h"
#include "ADXL345.h"
#include "bsp.h"

#define LETIMER_TOP 327
#define MIN_TIME 10
#define MAX_TIME 100
#define STEP_TIME_VARIATION 0.3
#define MIN_THRESHOLD 55555

void initPedometerTimer(void);

uint8_t accelData[100];
uint32_t steps = 0;

/********************************************//**
 * \brief turn on and setup accelerometer and timer interrupt
 * to make periodic measurement
 * \param
 * \param
 * \return
 *
 ***********************************************/
void initPedometer(void)
{
//    initAccel();  // already done
    initPedometerTimer();
}

void initPedometerTimer(void)
{
    /* Enable necessary clocks */
//    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
    /* The CORELE clock is also necessary for the RTC and all
       low energy peripherals, but since this function
       is called before RTC_setup() the clock enable
       is only included here */
//    CMU_ClockEnable(cmuClock_CORELE, true);
    CMU_ClockDivSet(cmuClock_LETIMER0, 1);
    CMU_ClockEnable(cmuClock_LETIMER0, true);

    /* Set initial compare values for COMP0 */
    LETIMER_CompareSet(LETIMER0, 0, LETIMER_TOP);

    LETIMER_IntClear(LETIMER0, 0xFF);         // clear all flags
    LETIMER_IntEnable(LETIMER0, 4);	        // enable Underflow Interrupt
    NVIC_EnableIRQ(LETIMER0_IRQn);

    /* Set configurations for LETIMER 0 */
    const LETIMER_Init_TypeDef letimerInit =
    {
        .enable         = true,                   /* start counting when init completed - only with RTC compare match */
        .debugRun       = false,                  /* Counter shall not keep running during debug halt. */
        .rtcComp0Enable = false,                  /* don't Start counting on RTC COMP0 match. */
        .rtcComp1Enable = false,                  /* Don't start counting on RTC COMP1 match. */
        .comp0Top       = true,                   /* Load COMP0 register into CNT when counter underflows. COMP is used as TOP */
        .bufTop         = false,                  /* Don't load COMP1 into COMP0 when REP0 reaches 0. */
        .out0Pol        = 0,                      /* Idle value for output 0. */
        .out1Pol        = 0,                      /* Idle value for output 1. */
        .ufoa0          = letimerUFOANone,        /* Pulse output on output 0 */
        .ufoa1          = letimerUFOANone,        /* No output on output 1*/
        .repMode        = letimerRepeatFree       /* Count while REP != 0 */
    };

    /* Initialize LETIMER */
    LETIMER_Init(LETIMER0, &letimerInit);
}


void LETIMER0_IRQHandler(void)
{
    static uint32_t i = 0;
    static uint32_t oldResult = 0;
    static uint32_t result = 0;
    static uint32_t lol = 0;
    static uint32_t minResult = 0;
    static uint32_t maxResult = 0;
    static uint32_t threshold = 0;
    static bool leadEdge = false;
    static uint32_t timePassed = 0;
    static uint32_t oldTimePassed = 0;

    LETIMER_IntClear(LETIMER0, 0xFF);         // clear all flags

    readRegister(DATAX0, 1, &accelData[i]);
    lol += accelData[i];
    readRegister(DATAX1, 1, &accelData[i]);
    lol += accelData[i] << 8;
    i++;
//TODO find rhythmic moves
//TODO implement minimum threshold value
    if( i >= 4)
    {
        i = 0;
        lol /= 4;

        oldResult = result;
        result = lol;
        lol = 0;

        if(result < minResult)
        {
            minResult = result;
        }else
        if(result > maxResult)
        {
            maxResult = result;
        }

        if(leadEdge)
        {
            timePassed++;

            if((result < oldResult)  && (result > threshold))
            {
                if((MIN_TIME < timePassed) && (timePassed < MAX_TIME))
                {
                    if((oldTimePassed * (1.0 - STEP_TIME_VARIATION) < timePassed)
                      && (timePassed < oldTimePassed * (1.0 + STEP_TIME_VARIATION)))
                    {
                        int rofl = oldTimePassed * (float)(1 - STEP_TIME_VARIATION);
                        steps++;
                        BSP_LedSet(1);
                    }

                    oldTimePassed = timePassed;

                    threshold = result/2;
                    if(threshold < MIN_THRESHOLD)
                    {
                        threshold = MIN_THRESHOLD;
                    }

                    leadEdge = false;
                }

                timePassed = 0;
            }else
            {
                BSP_LedClear(1);
            }
        }
        else
        {
            if( result > oldResult)
            {
                leadEdge = true;
            }
        }
    }
}
