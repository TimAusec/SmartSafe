/*
 * LED.c
 *
 *  Created on: Feb 1, 2024
 *      Author: tysonla edit
 */

#include "LED.h"

bool blinking=false;

void ConfigureLEDs(void)
{
    // GPIO Setup
    LED_PORT->SEL0 &= ~(GREEN_LED_PIN | RED_LED_PIN ); // Set LED1 pin to GPIO function
    LED_PORT->SEL1 &= ~(GREEN_LED_PIN | RED_LED_PIN );
    LED_PORT->DIR |= (GREEN_LED_PIN | RED_LED_PIN );
    LED_PORT->OUT &= ~(GREEN_LED_PIN | RED_LED_PIN );         //  LEDs start off

    TIMER_A1->CCR[0] = QUARTER_NOTE;
    TIMER_A1->CCTL[0] = 0b0;
    TIMER_A1->CTL = 0b000100000110;  //Configures stop mode initially with SMCLK
    NVIC->ISER[0] |= (1 << (TA1_N_IRQn));
}

void TA1_N_IRQHandler(void)
{
    /* Check if interrupt triggered by timer overflow */
    if (TIMER_A1->CTL & TIMER_A_CTL_IFG)
    {

        LED_PORT->OUT ^= GREEN_LED_PIN;                    // toggle LED
        TIMER_A1->CTL &= ~TIMER_A_CTL_IFG;    // clears TA1CTL flag

    }

}

void GreenLEDOn()
{
    LED_PORT->OUT |= GREEN_LED_PIN;
}

void GreenLEDBlinking()
{
    if (!blinking)
    {
        TIMER_A1->CTL = 0b000100010111; // Configures Timer_A1 in Up Mode with interrupt enabled
        blinking=true;
    }
}

void GreenLEDStopBlinking()
{
    if (blinking)
    {
        TIMER_A1->CTL = 0b000100000110; // Configures Timer_A1 in Up Mode with interrupt enabled
        blinking=false;
    }
}

void RedLEDOn()
{
    LED_PORT->OUT |= RED_LED_PIN;
}

void GreenLEDOff()
{
    LED_PORT->OUT &= ~GREEN_LED_PIN;
}

void RedLEDOff()
{
    LED_PORT->OUT &= ~RED_LED_PIN;
}

void LEDIndicateOpen()
{
    GreenLEDStopBlinking();
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
    GreenLEDBlinking();
}
void LEDIndicateLocked()
{
    RedLEDOn();
}
