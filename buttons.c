#include "em_gpio.h"
#include "em_cmu.h"
#include "state_machine.h"
#include "buttons.h"
#include "clock.h"



volatile uint8_t button = NO_BUTTON;

void initButtons()
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


void GPIO_EVEN_IRQHandler() // button A was pressed
{
    GPIO_IntClear(1 << BUTTON_B_PIN);
    button = BUTTON_A;
    screen_notification = false;
}


void GPIO_ODD_IRQHandler()  // button B was pressed
{
    GPIO_IntClear(1 << BUTTON_A_PIN);
    button = BUTTON_B;
    screen_notification = false;
}
