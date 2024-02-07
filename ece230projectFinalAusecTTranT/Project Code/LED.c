/*
 * LED.c
 *
 *  Created on: Feb 1, 2024
 *      Author: tysonla edit
 */


#include "LED.h"

void ConfigureLEDs(void)
{
    // GPIO Setup
        LED_PORT->SEL0 &= ~(GREEN_LED_PIN | RED_LED_PIN);                      // Set LED1 pin to GPIO function
        LED_PORT->SEL1 &= ~(GREEN_LED_PIN | RED_LED_PIN);
        LED_PORT->DIR |= (GREEN_LED_PIN | RED_LED_PIN);
        LED_PORT->OUT &= ~(GREEN_LED_PIN | RED_LED_PIN);                       //  LEDs start off
}

void TA1_N_IRQHandler(void)
{
    /* Check if interrupt triggered by timer overflow */
    if(TIMER_A1->CTL & TIMER_A_CTL_IFG)
    {

        LED_PORT->OUT ^= LED_MASK;                    // toggle LED
        TIMER_A1->CTL &= ~TIMER_A_CTL_IFG;    // clears TA1CTL flag

    }
    /* Check if interrupt triggered by CCR2 */
    if(TIMER_A1->CCTL[2] & TIMER_A_CCTLN_CCIFG)
    {

        LED_PORT->OUT ^= LED_MASK;                    // toggle LED
        TIMER_A1->CCTL[2] &= 0xFFFE;               // clears TA1CCTL2 flag

    }
}

void GreenLEDOn() {
    LED_PORT->OUT |= GREEN_LED_PIN;
}

void RedLEDOn() {
    LED_PORT->OUT |= RED_LED_PIN;
}

void GreenLEDOff() {
    LED_PORT->OUT &= ~GREEN_LED_PIN;
}

void RedLEDOff() {
    LED_PORT->OUT &= ~RED_LED_PIN;
}

void LEDIndicateOpen()
{
    GreenLEDOn();
    RedLEDOff();
}
void LEDIndicateSecurityMode()
{
    RedLEDOn();
    GreenLEDOff();
}
void LEDIndicateClosed()
{
    GreenLEDOff();
    RedLEDOff();
}
void LEDIndicateUnlocked()
{
    GreenLEDOn(); //PLACEHOLDER: DELETE AFTER IMPLEMENTATION
    //TODO: Implement Blinking
}
void LEDIndicateLocked()
{
    RedLEDOn(); //PLACEHOLDER: DELETE AFTER IMPLEMENTATION
    //TODO: Implement Blinking
}
