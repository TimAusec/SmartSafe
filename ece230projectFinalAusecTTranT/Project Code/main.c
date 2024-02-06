/*
 * main.c
 *
 *  Created on: Jan 31, 2024
 *      Authors:
 *      Lucas Tyson
 *      Timothy Ausec
 *      Tuan Tran
 *
 *      ECE230 - Smart Safe
 *      Rose-Hulman Institute of Technology
 */
#include "msp.h"

#include "main.h"

#define DEFAULT_CLOCK_FREQUENCY_KHZ 500
#define DEBOUNCE_DELAY_TIME_MS      30
#define DEBOUNCE_DELAY_COUNT        DEFAULT_CLOCK_FREQUENCY_KHZ*DEBOUNCE_DELAY_TIME_MS

bool safeSecurityFlag = false;

void Debounce()
{
    volatile int i;
    for (i = 0; i < DEBOUNCE_DELAY_COUNT; i++)
        ;
}

int length(int array[])
{
    return sizeof(array) / sizeof(array[0]);
}

void ConfigureDevices()
{
    ConfigureLEDs();
    configHFXT();
    ConfigureSW1();
    initServoMotor();
    ConfigKeyPad();
}

void OpenSafe()
{
    LEDIndicateOpen();
    OpenServo();
    ClearCode();
}

void ActivateSecurity()
{
    LEDIndicateSecurityMode();
    CloseServo();
    ClearCode();
    safeSecurityFlag = true;
    //TODO: Implement Security Measure
}

void CloseSafe()
{
    LEDIndicateClosed();
    CloseServo();
    ClearCode();
}

void ResetSafe()
{
    ClearCode();
    ConfigureDevices();
    bool safeSecurityFlag = false;
    //TODO: Implement Reset Safe to Initial State
}
void IndicateSafeUnlocked()
{
    LEDIndicateUnlocked();
}

void IndicateSafeLocked()
{
    LEDIndicateLocked();
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    ConfigureDevices();
    __enable_irq();

    while (1)
    {
        if (GetTriesExceededFlag()) //if access attempt count exceeded
        {
            printf("\n Exceeded Tries Condition Reached");
            ActivateSecurity();
        }//end if access attempt count exceeded
        if (!GetServoOpenFlag()) //if closed
        {
            if (GetOpenCodeFlag()) // if open code entered
            {
                IndicateSafeUnlocked();
            }
            if (GetOpenCodeFlag() & GetSwitch1Flag()) ////if open code entered and switch pressed
            {
                printf("\n Open Condition Reached");
                OpenSafe();
            }
        } // end if closed
        if (GetServoOpenFlag()) //if open
        {
            if (GetCloseCodeFlag()) //if close code entered
            {
                printf("\n Close Condition Reached");
                IndicateSafeLocked();
            }
            if (GetCloseCodeFlag() & GetSwitch1Flag())//if close code entered and switch pressed
            {
                printf("\n Close Condition Reached");
                CloseSafe();
            }
        }// end if open
        if (GetStopCodeFlag() & GetSwitch1Flag())//if stop security code entered and switch pressed
        {
            printf("\n Stop Condition Reached");
            ResetSafe();
        } // end if stop security code entered and switch pressed
    }//end while(1)
}
