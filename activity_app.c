/**************************************************************************//**
 * @file activity_app.c
 * @brief log daily activity
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version
 ******************************************************************************/

#include "em_pcnt.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "time_management.h"
#include "activity_app.h"
#include "buttons.h"
#include "ADXL345.h"
#include "segmentlcd.h"
#include "state_machine.h"

void initCounter(void);
void drawActivityScreen(void);
void PCNT1_IRQHandler(void);
void initSPI(void);
void initAccel(void);
void initAccelInterrupt(void);

uint16_t activity[MAX_WEEKS][WEEK_DAYS];
uint32_t average_activity;

/********************************************//**
 * \brief setup counter to count external pulses
 *
 * \param ACTIVITY_PORT - defined in activity.h
 * \param ACTIVITY_PIN  - defined in activity.h
 * \return
 *
 ***********************************************/
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


/********************************************//**
 * \brief init counter and clear all weekly activities
 *
 * \param
 * \param
 * \return
 *
 ***********************************************/
void initActivity(void)
{
    initCounter();

    // init activity variable
    for(uint8_t week = 0; week < 52; week++)
    {
        for(uint8_t day = 0; day < 7; day++)
        {
            activity[week][day] = 0;
        }
    }

    average_activity = 0;

    initSPI();
    initAccel();
    initAccelInterrupt();   //ADXL345 special
}


/********************************************//**
 * \brief ADXL can't automatically clear activity interrupt,
 *  we have to do it manually
 * \param
 * \param
 * \return
 *
 ***********************************************/
void initAccelInterrupt(void)
{
    /* Configure interrurp pin from accel, and enable interrupt on rising edge */
    GPIO_PinModeSet(ACCEL_INT_PORT , ACCEL_INT_PIN, gpioModeInput, 0);
    GPIO_IntConfig(ACCEL_INT_PORT, ACCEL_INT_PIN, true, false, true);

    /* Clear and enable interrupts */
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}


/********************************************//**
 * \brief clear daily activity and count new average_activity
 *
 * \param week - could be 0 <= week < 52
 * \param wday - day of the week between 0 and 6
 * \return
 *
 ***********************************************/
void clearActivity(uint8_t week, uint8_t wday)
{
    average_activity = average_activity * 6 / 7 + activity[week][wday]; // minus average day plus real day
    activity[week][wday] = 0;
}


/********************************************//**
 * \brief draw and output activity app screen
 *  current version use segment LCD
 * \param
 * \param
 * \return
 *
 ***********************************************/
void drawActivityScreen(void)
{
    SegmentLCD_NumberOff();
    SegmentLCD_Write("Actvty");
    SegmentLCD_Number(activity[currentTime.tm_week][currentTime.tm_wday]);
    // show current day - average activity
}


/********************************************//**
 * \brief main loop of activity app check buttons,
 *  change states accordingly and go to EM2 if nothing happens
 *
 * \param void
 * \return void
 *
 ***********************************************/
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
//        previousState();
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

    activity[currentTime.tm_week][currentTime.tm_wday]++;
}


/******************************************************************************
 * @brief init accelerometer to output interrupt on any activity
 *
 *
 ******************************************************************************/
void initAccel(void)
{
    writeRegister(DATA_FORMAT, 0x03);     //16g mode
    writeRegister(THRESH_ACT, SENSITIVITY);   // activity  treshold
    writeRegister(ACT_INACT_CTL, 0x70);  // define active all axes
    writeRegister(INT_ENABLE, 0x10);      // enable Activity interrupt
    writeRegister(INT_MAP, 0x00);         // all active interrupt to pin 1
    //  writeRegister(BW_RATE, 0x11);         // set lowest possible consumption datarate 23 uA
    writeRegister(POWER_CTL, 0x08);  // Measurement mode

    while( !(values[0] & ACT_INT_MASK) )
    {
        readRegister(INT_SOURCE, 1, values);
    }
}



/******************************************************************************
 * @brief init USART1 in SPI mode
 *
 *
 ******************************************************************************/
void initSPI(void)
{
  // Setup clock
  CMU_ClockEnable(CMUCLOCK_ACCEL_USART, true);

  // Setup GPIO's
  GPIO_PinModeSet( ACCEL_MOSI_PORT, ACCEL_MOSI_PIN, gpioModePushPull, 0 );         // mosi
  GPIO_PinModeSet( ACCEL_MISO_PORT, ACCEL_MISO_PIN, gpioModeInput,    0 );         // miso
  GPIO_PinModeSet( ACCEL_CLK_PORT, ACCEL_CLK_PIN, gpioModePushPull, 0 );         // clk
  GPIO_PinModeSet( ACCEL_CS_PORT, ACCEL_CS_PIN, gpioModePushPull, 0 );         // cs

  // init USART as SPI
  USART_InitSync_TypeDef usartInit = USART_INITSYNC_DEFAULT;
  usartInit.baudrate = BAUDRATE;
  usartInit.msbf = true;
  usartInit.clockMode = usartClockMode3;

  USART_InitSync(ACCEL_USART, &usartInit);

  //route USART1 to expansion header
  ACCEL_USART->ROUTE = (USART_ROUTE_CLKPEN | USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | ACCER_USART_LOC);
  ACCEL_USART->CMD |= USART_CMD_RXBLOCKEN;
}



/********************************************//**
 * \brief show small suggestions based on activity statistics
 *
 * \param
 * \param
 * \return
 *
 ***********************************************/
void activitySuggestion(void)
{
    SegmentLCD_Write("Do More");
    screen_notification = true;
}
