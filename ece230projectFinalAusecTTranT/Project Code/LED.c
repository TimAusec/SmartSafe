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

        TIMER_A1->CCR[2] = QUARTER_NOTE;
                TIMER_A1->CCTL[2] = 0x0010;                                     // Configures CCR2 for Compare mode with interrupt enabled (no output mode - 0)
                TIMER_A1->CTL = 0b000100000110;                                 //Configures stop mode initially
                NVIC->ISER[0] |= (1 << (TA1_N_IRQn));
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
    TIMER_A1->CTL = 0b000100010110;                                  // Configures Timer_A1 in Up Mode with interrupt enabled
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
