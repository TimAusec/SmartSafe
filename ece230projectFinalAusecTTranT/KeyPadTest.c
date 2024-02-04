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
    for (index = 0; index < 2500; index++)
        ;
}

void main()
{
    ConfigKeyPad();
    __enable_irq();
    while (1)
    {
        if (GetOpenCodeFlag())
        {
            printf("\n Open Condition Reached");
        }
        else if (GetTriesExceededFlag())
        {
            printf("\n Exceeded Tries Condition Reached");

        }
        if (GetCloseCodeFlag())
        {
            printf("\n Close Condition Reached");
        }
        if (GetStopCodeFlag())
        {
            printf("\n Stop Condition Reached");
        }
    }

}
