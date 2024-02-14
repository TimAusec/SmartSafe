/*
 * BluetoothModule.c
 *
 *  Created on: Feb 9, 2024
 *      Author: Tuan Tran
 */
#include "BluetoothModule.h"

char prompt1[] = { "\n\rHello " };
char invalid[] = { "\n\rInvalid value" };

char attemptsRemaining[] =
        { "\n\r\nOptions: \n\rPress (A) to see how many attempts you have left! \n\r" };
char servoDoorStatus[] = {
        "\n\r\nPress (S) to see if the door is open or closed! \n\r" };
char lastAttemptsMade[] = {
        "\n\r\nPress (M) to view the most recent attempts! \n\r" };
char directionOfSafe[] = {
        "n\r\nPress (D) to see the direction the safe is going! \n\r" };
char invalid1[] = { "\n\rInvalid value \n\r" };

bool sendAttemptsFlag = false;
bool sendStatusFlag = false;
bool sendAccessLogFlag = false;
bool sendUnlockFlag = false;

void InitBluetooth()
{

    /* Configure UART pins */
    BLUETOOTH_PORT->SEL0 |= BLUETOOTH_PINS; // set 2-UART pins as secondary function
    BLUETOOTH_PORT->SEL1 &= ~(BLUETOOTH_PINS);

    /* Configure UART
     *  Asynchronous UART mode, 8N1 (8-bit data, no parity, 1 stop bit),
     *  LSB first, SMCLK clock source
     */
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_A_CTLW0_SSEL__SMCLK; // Configure eUSCI clock source for SMCLK

    /* Baud Rate calculation
     * Refer to Section 24.3.10 of Technical Reference manual
     * BRCLK = 12000000, Baud rate = 9600
     * N = fBRCLK / Baud rate = 12000000/9600 = 1250
     * from Technical Reference manual Table 24-5:
     *
     * DONE lookup values for UCOS16, UCBRx, UCBRFx, and UCBRSx in Table 24-5
     */
    // DONE set clock prescaler in EUSCI_A2 baud rate control register
    EUSCI_A2->MCTLW |= EUSCI_A_MCTLW_OS16 | BIT5;
    EUSCI_A2->BRW = 78;

    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // Initialize eUSCI
    EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;        // Clear eUSCI RX interrupt flag
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;            // Enable USCI_A2 RX interrupt

    // Enable eUSCIA2 interrupt in NVIC module
    NVIC->ISER[0] |= (1 << EUSCIA2_IRQn);

    SendUserPrompt();
}
void SendUserPrompt()
{
    SendCharArrayBluetooth(attemptsRemaining);
    SendCharArrayBluetooth(servoDoorStatus);
    SendCharArrayBluetooth(lastAttemptsMade);
    SendCharArrayBluetooth(directionOfSafe);
}

void SendCharArrayBluetooth(char *Buffer)
{
    unsigned int count;
    sendAttemptsFlag = false;
    sendStatusFlag = false;
    sendAccessLogFlag = false;
    sendUnlockFlag = false;
    for (count = 0; count < strlen(Buffer); count++)
    {
        while (!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG))
            ; //wait for TX buffer to clear
        EUSCI_A2->TXBUF = Buffer[count]; //send character
    }   //end for()
}

char GetCharBluetooth(void)
{ //polling
    char ReceivedChar;
    while (!(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG))
        ;
    ReceivedChar = EUSCI_A2->RXBUF;
    return ReceivedChar;
} //end GetChar(void)

void HandleRecievedValue()
{
    uint8_t Key = EUSCI_A2->RXBUF;
    sendAttemptsFlag = false;
    sendStatusFlag = false;
    sendAccessLogFlag = false;
    sendUnlockFlag = false;
    // Echo character back to screen, otherwise user will not be able to
    //  verify what was typed
    while (!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG))
        ; // Wait for TX buffer ready
    EUSCI_A2->TXBUF = Key;                 // Echo character to terminal
    if (Key != NULL)
    {
        switch (Key)
        {
        case 'A':
            sendAttemptsFlag = true;
            break;
        case 'S':
            sendStatusFlag = true;
            break;
        case 'M':
            sendAccessLogFlag = true;
            break;
        case 'D':
            sendUnlockFlag = true;
            break;
        default:
            SendCharArrayBluetooth(invalid1);
            break;
        }
    }
}

void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG) // Check if receive flag is set (value ready in RX buffer)
    {
        HandleRecievedValue();
    }
}

void SendAttemptsBluetooth(int currentTries)
{
    char AttemptsMessage[] = { "I'm Empty!" };
    sprintf(AttemptsMessage, "\r\n  Current Attempts: %d \n\n\r", currentTries);
    SendCharArrayBluetooth(AttemptsMessage);
}
void SendStatusBluetooth(bool safeOpenFlag)
{
    char SafeOpenMessage[] = { "\n\r\nThe safe is currently open \n\n\r" };
    char SafeClosedMessage[] = { "\n\r\nThe safe is currently closed \n\n\r" };
    if (safeOpenFlag)
    {
        SendCharArrayBluetooth(SafeOpenMessage);
    }
    else
    {
        SendCharArrayBluetooth(SafeClosedMessage);
    }
}
void SendAccessLogBluetooth(int AccessLog[100][6], int RTCIndex)
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
        char TimeMessage[] = { "I'm empty!" };
        char DateMessage[] = { "I'm empty!" };
        //sprintf(TimeMessage, "\r\n  Time : %2d:%2d:%2d ", Hour, Minute, Second); //causes death; cannot be used
        SendCharArrayBluetooth(TimeMessage);
        //sprintf(DateMessage, "      Date : %d / %d / %d \n\n\r", Month, Day, Year); //causes death; cannot be used
        SendCharArrayBluetooth(DateMessage);
    }
}
void SendUnlockStatusBluetooth(bool unlockedFlag)
{
    char SafeUnlockedMessage[] = { "\n\r\nThe safe is currently unlocked \n\n\r" };
    char SafeLockedMessage[] = { "\n\r\nThe safe is currently locked \n\n\r" };
    if (unlockedFlag)
    {
        SendCharArrayBluetooth(SafeUnlockedMessage);
    }
    else
    {
        SendCharArrayBluetooth(SafeLockedMessage);
    }
}

bool GetSendAttemptsFlag()
{
    return sendAttemptsFlag;
}
bool GetSendDoorStatusFlag()
{
    return sendStatusFlag;
}
bool GetSendAccessLogFlag()
{
    return sendAccessLogFlag;
}
bool GetSendUnlockFlag()
{
    return sendUnlockFlag;
}
