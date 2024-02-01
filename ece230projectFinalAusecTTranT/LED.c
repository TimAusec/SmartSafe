/*
 * LED.c
 *
 *  Created on: Feb 1, 2024
 *      Author: tysonla
 */


#include "LED.h"

void ConfigureLED(void) {


    // GPIO Setup
        LED_Port->SEL0 &= ~(LED1_Pin | LED2_Pin);                      // Set LED1 pin to GPIO function
        LED_Port->SEL1 &= ~(LED1_Pin | LED2_Pin);
        LED_Port->DIR |= (LED1_Pin | LED2_Pin;
        LED_Port->OUT &= ~(LED1_Pin | LED2_Pin);                       //  LEDs start off

}

void led1On() {
    LED_Port->OUT &= LED1_pin;
}

void led2On() {
    LED_Port->OUT &= LED2_pin;
}
