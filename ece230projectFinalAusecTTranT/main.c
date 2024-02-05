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

bool safeOpenFlag = false;
bool safeClosedFlag = true;
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
    safeOpenFlag = true;
    safeClosedFlag = false;
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
    safeOpenFlag = false;
    safeClosedFlag = true;
}

void ResetSafe()
{
    ClearCode();
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
        if (GetTriesExceededFlag())
        {
            printf("\n Exceeded Tries Condition Reached");
            ActivateSecurity();
        }
        if (safeClosedFlag)
        {
            if (GetOpenCodeFlag())
            {
                IndicateSafeUnlocked();
            }
            if (GetOpenCodeFlag() & GetSwitch1Flag())
            {
                printf("\n Open Condition Reached");
                OpenSafe();
            }
        }
        if (safeOpenFlag)
        {
            if (GetCloseCodeFlag())
            {
                printf("\n Close Condition Reached");
                IndicateSafeLocked();
            }
            if (GetCloseCodeFlag() & GetSwitch1Flag())
            {
                printf("\n Close Condition Reached");
                CloseSafe();
            }
            if (GetStopCodeFlag())
            {
                printf("\n Stop Condition Reached");
                ResetSafe();
            }
        }
    }
}
