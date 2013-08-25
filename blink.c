/**************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for EFM32GG_STK3700
 * @author Energy Micro AS
 * @version 3.20.0
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2012 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "bsp.h"
#include "bsp_trace.h"
#include "state_machine.h"
#include "time_management.h"
#include "setup_time_app.h"
#include "buttons.h"
#include "segmentlcd.h"
#include "pomodoro_app.h"
#include "activity_app.h"
#include "variables.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t dlyTicks);

/**************************************************************************//**
 * @brief SysTick_Handler
 * Interrupt Service Routine for system tick counter
 *****************************************************************************/
void SysTick_Handler(void)
{
    msTicks++;       /* increment counter necessary in Delay()*/
}


/**************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 *****************************************************************************/
void Delay(uint32_t dlyTicks)
{
    uint32_t curTicks;

    curTicks = msTicks;
    while ((msTicks - curTicks) < dlyTicks) ;
}

/********************************************//**
 * \brief trace function for energy aware debugging
 *  remove before production
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
void setupSWO(void)
{
    /* Enable GPIO Clock. */
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;
    /* Enable Serial wire output pin */
    GPIO->ROUTE |= GPIO_ROUTE_SWOPEN;
#if defined(_EFM32_GIANT_FAMILY) || defined(_EFM32_WONDER_FAMILY) || defined(_EFM32_LEOPARD_FAMILY)
    /* Set location 0 */
    GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC0;

    /* Enable output on pin - GPIO Port F, Pin 2 */
    GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK);
    GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL;
#else
    /* Set location 1 */
    GPIO->ROUTE = (GPIO->ROUTE & ~(_GPIO_ROUTE_SWLOCATION_MASK)) | GPIO_ROUTE_SWLOCATION_LOC1;
    /* Enable output on pin */
    GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK);
    GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL;
#endif
    /* Enable debug clock AUXHFRCO */
    CMU->OSCENCMD = CMU_OSCENCMD_AUXHFRCOEN;

    while(!(CMU->STATUS & CMU_STATUS_AUXHFRCORDY));

    /* Enable trace in core debug */
    CoreDebug->DHCSR |= 1;
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;

    /* Enable PC and IRQ sampling output */
    DWT->CTRL = 0x400113FF;
    /* Set TPIU prescaler to 16. */
    TPI->ACPR = 0xf;
    /* Set protocol to NRZ */
    TPI->SPPR = 2;
    /* Unlock ITM and output data */
    ITM->LAR = 0xC5ACCE55;
    ITM->TCR = 0x10009;
}

/** \brief the main function of the project
 *  check current state and switch apps respectively
 * \param 
 * \param 
 * \return 
 *
 */     
int main(void)
{
    /* Chip errata */
    CHIP_Init();
// TODO add auto go back to main menu after 20seconds of inactivity
    /* If first word of user data page is non-zero, enable eA Profiler trace */
    BSP_TraceProfilerSetup();

    ////////////////////////////////////////////////////////////////////////
    setupSWO();
    /* Initialize LED driver */
    BSP_LedsInit();
    initButtons();
    initClock();
    initActivity();
    initVariables();
    SegmentLCD_Init(false);  /* Enable LCD without voltage boost */

    state = main_screen;

    /* Infinite blink loop */
    while (1)
    {
        //NOTE maybe button A(change state) must be checked here
        if(screen_notification == true)    // don't update screen until user reaction
        {
            EMU_EnterEM2(true);
            button = NO_BUTTON;
        }
        else
        {
            switch (state)
            {
            case main_screen:
                mainScreenApp();
                break;
            case pomodoro_screen:
                pomodoroApp();
                break;
            case activity_screen:
                activityApp();
                break;
            case time_setup_screen:
                setupTimeApp();
                break;
            case alarm_setup_screen:
                setupAlarmApp();
                break;
            }
        }
    }
}
