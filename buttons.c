/**************************************************************************//**
 * @file buttons.c
 * @brief simple buttons driver
 * @author Alexandr D.  sasha.engineer@gmail.com
 * @version 
 ******************************************************************************/

#include "em_gpio.h"
#include "em_cmu.h"
#include "state_machine.h"
#include "buttons.h"
#include "time_management.h"


volatile uint8_t button = NO_BUTTON;


/********************************************//**
 * \brief initialize GPIO to use as buttons
 *
 * \param BUTTON_x_PORT defined in buttons.h
 * \param BUTTON_x_PIN defined in buttons.h
 * \return 
 *
 ***********************************************/         
void initButtons(void)
{
    CMU_ClockEnable(cmuClock_GPIO, true);
    GPIO_PinModeSet(BUTTON_A_PORT, BUTTON_A_PIN, gpioModeInput, 1);     // check if DOUT should be set
    GPIO_IntConfig(BUTTON_A_PORT, BUTTON_A_PIN, false, true, true);     // irq on falling edge

    GPIO_PinModeSet(BUTTON_B_PORT, BUTTON_B_PIN, gpioModeInput, 1);     // check if DOUT should be set
    GPIO_IntConfig(BUTTON_B_PORT, BUTTON_B_PIN, false, true, true);     // irq on falling edge

    /* Clear and enable interrupts */
    NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);
}


/********************************************//**
 * \brief handles even half of GPIO interrupts
 *  change button global and clear int flag, turn off screen_notification flag, 
 *  which was turned on by runAlarm function
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/                                  
void GPIO_EVEN_IRQHandler() // button A was pressed
{
    GPIO_IntClear(1 << BUTTON_B_PIN); /**< clear interrupt */
    button = BUTTON_A;
    screen_notification = false;
}


/********************************************//**
 * \brief handles odd half of GPIO interrupts
 *  change button global and clear int flag, turn off screen_notification flag, 
 *  which was turned on by runAlarm function
 * \param 
 * \param 
 * \return 
 *
 ***********************************************/      
void GPIO_ODD_IRQHandler()  // button B was pressed
{
    GPIO_IntClear(1 << BUTTON_A_PIN);
    button = BUTTON_B;
    screen_notification = false;
}
