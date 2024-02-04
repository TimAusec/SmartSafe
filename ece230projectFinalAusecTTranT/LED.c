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
