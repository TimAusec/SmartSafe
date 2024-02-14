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

bool safeSecurityFlag = false;
bool once = false;
bool securityFlag = false;
bool safeOpenFlag = false;
bool openCodeEnteredFlag = false;
bool closeCodeEnteredFlag = false;
bool switchPressedFlag = false;
bool stopCodeEnteredFlag = false;
bool unlockedFlag = false;

bool sendAttemptsBluetoothFlag = false;
bool sendDoorStatusBluetoothFlag = false;
bool sendAccessLogBluetoothFlag = false;
bool sendUnlockBluetoothFlag = false;

int TimeData[6];
int AccessLog[100][6];
int RTCIndex = 0;

void AppendLog()
{
    int index;
    for (index = 0; index < 6; index++)
    {
        AccessLog[RTCIndex - 1][index] = TimeData[index];
    }
}

int length(int array[])
{
    return sizeof(array) / sizeof(array[0]);
}

void ConfigureDevices()
{
    configLFXT_jjs();
    configHFXT();
    ConfigureSW1();
    InitBluetooth();
    ConfigureLEDs();
    ConfigRTC();
    InitServoMotor();
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
    once = false;
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
    if (!once)
    {
        GetDateTimeData(TimeData, &RTCIndex);
        AppendLog();
        PrintDateTime();
        once = true;
    }
}

void IndicateSafeLocked()
{
    LEDIndicateLocked();
}

void PrintDateTime()
{
    int index;
    int Month;
    int Day;
    int Year;
    int Hour;
    int Minute;
    int Second;
    for (index = 0; index < RTCIndex; index++)
    {
        Month = AccessLog[index][0];
        Day = AccessLog[index][1];
        Year = AccessLog[index][2];
        Hour = AccessLog[index][3];
        Minute = AccessLog[index][4];
        Second = AccessLog[index][5];
        printf("\r\n  Time : %2d:%2d:%2d ", Hour, Minute, Second);

        printf("      Date : %d / %d / %d ", Month, Day, Year);
    }
}

void SendAppropriateBluetoothMessage()
{
    if (sendAttemptsBluetoothFlag)
    {
        int currentTries=GetCurrentTriesCount();
        SendAttemptsBluetooth(currentTries);
    }
    if (sendDoorStatusBluetoothFlag)
    {
        SendStatusBluetooth(safeOpenFlag);
    }
    if (sendAccessLogBluetoothFlag)
    {
        SendAccessLogBluetooth(AccessLog,RTCIndex);
    }
    if (sendUnlockBluetoothFlag)
    {
        SendUnlockStatusBluetooth(unlockedFlag);
    }
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    ConfigureDevices();
    __enable_irq();
    int index;
    for(index=0;index<6;index++)
    {
        AccessLog[0][index]=0; //initialize accesslog values
    }

    while (1)
    {
        securityFlag = GetTriesExceededFlag();
        safeOpenFlag = GetServoOpenFlag();
        openCodeEnteredFlag = GetOpenCodeFlag();
        closeCodeEnteredFlag = GetCloseCodeFlag();
        switchPressedFlag = GetSwitch1Flag();
        stopCodeEnteredFlag = GetStopCodeFlag();

        /* Bluetooth-related flags */
        sendAttemptsBluetoothFlag = GetSendAttemptsFlag();
        sendDoorStatusBluetoothFlag = GetSendDoorStatusFlag();
        sendAccessLogBluetoothFlag = GetSendAccessLogFlag();
        sendUnlockBluetoothFlag = GetSendUnlockFlag();

        SendAppropriateBluetoothMessage();

        if (securityFlag) //if access attempt count exceeded
        {
            printf("\n Exceeded Tries Condition Reached");
            ActivateSecurity();
        } //end if access attempt count exceeded
        if (!safeOpenFlag) //if closed
        {
            if (openCodeEnteredFlag) // if open code entered
            {
                IndicateSafeUnlocked();
                unlockedFlag = true;
                ClearCode();
            }
            if (unlockedFlag & switchPressedFlag) //if open code entered and switch pressed
            {
                OpenSafe();
                unlockedFlag = false;
            }
        } // end if closed
        if (safeOpenFlag) //if open
        {
            if (closeCodeEnteredFlag) //if close code entered
            {
                IndicateSafeLocked();
                unlockedFlag = false;
                closeCodeEnteredFlag = false;
                ClearCode();
            }
            if (!unlockedFlag & switchPressedFlag) //if close code entered and switch pressed
            {
                CloseSafe();
            }
        } // end if open
        if (stopCodeEnteredFlag & switchPressedFlag) //if stop security code entered and switch pressed
        {
            printf("\n Stop Condition Reached");
            ResetSafe();
        } // end if stop security code entered and switch pressed
    } //end while(1)
}
