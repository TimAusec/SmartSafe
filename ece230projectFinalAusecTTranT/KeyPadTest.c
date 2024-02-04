/*
 * KeyPadTest.c
 *
 *  Created on: Feb 1, 2024
 *      Author: Timothy Ausec
 */
#include "KeyPad.h"
#include <stdio.h>
#include <stdint.h>

void Debounce()
{
    int index;
    for(index=0;index<2500;index++);
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
            printf("\n Open Condition Reached");
            //TODO: open servos
            //TODO: turn on LEDs
            int dummy=0;
            ClearCode(&dummy);
        }
        else
        {
            if(IsExceededMaxTries())
            {
                //TODO: Active security features
                printf("\n Exceeded Tries Condition Reached");
            }
        }
        if (IsStopCode())
        {
            printf("\n Stop Condition Reached");
            //TODO: Deactivate security features
            //TODO: Reset to initial state
        }
        //TODO: Clear interrupt flag
    }

}
