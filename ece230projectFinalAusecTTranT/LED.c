/*
 * LED.c
 *
 *  Created on: Feb 1, 2024
 *      Author: tysonla
 */


#include "LED.h"
#include <LED.h>

void ConfigureLED(void) {


    // GPIO Setup
        LED_Port->SEL0 &= ~(BIT4 | BIT5);                      // Set LED1 pin to GPIO function
        LED_Port->SEL1 &= ~(BIT4 | BIT5);
        LED_Port->DIR |= (BIT4 | BIT5);
        LED_Port->OUT &= ~(BIT4 | BIT4);                       //  LEDs start off

}

void led1On() {
    LED_Port->OUT &= BIT4;
}

void led2On() {
    LED_Port->OUT &= BIT5;
}
