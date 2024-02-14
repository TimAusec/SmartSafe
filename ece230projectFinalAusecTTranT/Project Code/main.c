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

char attemptsRemaining[] =
        { "\n\r\nOptions: \n\rPress (A) to see how many attempts you have left! \n\r" };
char servoDoorStatus[] = {
        "\n\r\nPress (S) to see if the door is open or closed! \n\r" };
char lastAttemptsMade[] = {
        "\n\r\nPress (M) to view the most recent attempts! \n\r" };
char directionOfSafe[] = {
        "n\r\nPress (D) to see the direction the safe is going! \n\r" };
char invalid1[] = { "\n\rInvalid value \n\r" };

int TimeData[6];
int AccessLog[100][6];
int RTCIndex = 0;

void AppendLog()
{
    int index;
    for (index = 0; index < 6; index++)
    {
        AccessLog[RTCIndex-1][index] = TimeData[index];
        int debugRTCIndex=RTCIndex;
        int debugTimeData=TimeData[index];
        int debugAccessLog=AccessLog[RTCIndex][index];
    }
    for(index=0;index<RTCIndex;index++)
    {
        int debugAccessLog=0;
        debugAccessLog=AccessLog[index][0];
        int index1=index;
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
    once=false;
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
        Month=AccessLog[index][0];
        Day=AccessLog[index][1];
        Year=AccessLog[index][2];
        Hour=AccessLog[index][3];
        Minute=AccessLog[index][4];
        Second=AccessLog[index][5];
        printf("\r\n  Time : %2d:%2d:%2d ", Hour, Minute, Second);

        printf("      Date : %d / %d / %d ", Month,
               Day, Year);
    }
}

void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    ConfigureDevices();
    char KeyEntered;
    __enable_irq();

    while (1)
    {
        // KeyEntered = GetCharBluetooth();
        KeyEntered = NULL;
        if (KeyEntered != NULL)
        {
            switch (KeyEntered)
            {
            case 'A':
                SendCharArrayBluetooth(attemptsRemaining);
                break;
            case 'S':
                SendCharArrayBluetooth(servoDoorStatus);
                break;
            case 'M':
                SendCharArrayBluetooth(lastAttemptsMade);
                break;
            case 'D':
                SendCharArrayBluetooth(directionOfSafe);
                break;
            default:
                SendCharArrayBluetooth(invalid1);
                SendCharArrayBluetooth(attemptsRemaining);
                SendCharArrayBluetooth(servoDoorStatus);
                SendCharArrayBluetooth(lastAttemptsMade);
                SendCharArrayBluetooth(directionOfSafe);
                break;
            }
        }
        if (GetTriesExceededFlag()) //if access attempt count exceeded
        {
            printf("\n Exceeded Tries Condition Reached");
            ActivateSecurity();
        } //end if access attempt count exceeded
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
            if (GetCloseCodeFlag() & GetSwitch1Flag()) //if close code entered and switch pressed
            {
                printf("\n Close Condition Reached");
                CloseSafe();
            }
        } // end if open
        if (GetStopCodeFlag() & GetSwitch1Flag()) //if stop security code entered and switch pressed
        {
            printf("\n Stop Condition Reached");
            ResetSafe();
        } // end if stop security code entered and switch pressed
    } //end while(1)
}
