/*
 * KeyPadTest.c
 *
 *  Created on: Feb 1, 2024
 *      Author: Timothy Ausec
 */
#include "keypad.h"
#include <stdio.h>

void Debounce()
{
    int index;
    for(index=0;index<30000;index++);
}

void main()
{
    ConfigKeyPad();
    __enable_irq();
    while(1)
    {
        //TODO: Implement test
        if (IsOpenCode())
        {
            printf("Open Condition Reached");
            //TODO: open servos
            //TODO: turn on LEDs
        }
        else
        {
            if(IsExceededMaxTries())
            {
                //TODO: Active security features
                printf("Exceeded Tries Condition Reached");
            }
        }
        if (IsStopCode())
        {
            printf("Stop Condition Reached");
            //TODO: Deactivate security features
            //TODO: Reset to initial state
        }
        //TODO: Clear interrupt flag
    }
}
