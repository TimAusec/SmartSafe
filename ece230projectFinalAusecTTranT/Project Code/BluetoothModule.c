/*
 * BluetoothModule.c
 *
 *  Created on: Feb 9, 2024
 *      Author: Tuan Tran
 */
#include "stdio.h"  //printf(); sprintf();
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "BluetoothModule.h"

char prompt1[] = {"\n\rHello "};
char invalid[] = {"\n\rInvalid value"};

void InitBluetooth()
{
    CS->KEY = CS_KEY_VAL;                   // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_3;           // Set DCO to 12MHz (nominal, center of 8-16MHz range)
    CS->CTL1 = CS_CTL1_SELS_3 |                // SMCLK = DCO
               CS_CTL1_SELM_3;                 // MCLK = DCO
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    /* Configure UART pins */
    BLUETOOTH_PORT->SEL0 |= BLUETOOTH_PINS;                // set 2-UART pins as secondary function
    BLUETOOTH_PORT->SEL1 &= ~(BLUETOOTH_PINS);

    /* Configure UART
     *  Asynchronous UART mode, 8N1 (8-bit data, no parity, 1 stop bit),
     *  LSB first, SMCLK clock source
     */
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset
    EUSCI_A2->CTLW0 = EUSCI_A_CTLW0_SWRST | // Remain eUSCI in reset
            EUSCI_A_CTLW0_SSEL__SMCLK;      // Configure eUSCI clock source for SMCLK

    /* Baud Rate calculation
     * Refer to Section 24.3.10 of Technical Reference manual
     * BRCLK = 12000000, Baud rate = 9600
     * N = fBRCLK / Baud rate = 12000000/9600 = 1250
     * from Technical Reference manual Table 24-5:
     *
     * DONE lookup values for UCOS16, UCBRx, UCBRFx, and UCBRSx in Table 24-5
     */
    // DONE set clock prescaler in EUSCI_A2 baud rate control register
    EUSCI_A2->MCTLW|= EUSCI_A_MCTLW_OS16 | BIT5;
    EUSCI_A2->BRW = 78;

    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // Initialize eUSCI
    EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;        // Clear eUSCI RX interrupt flag
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;            // Enable USCI_A2 RX interrupt

    // Enable eUSCIA2 interrupt in NVIC module
    NVIC->ISER[0] |= (1 << EUSCIA2_IRQn );
}

void SendCharArrayBluetooth(char *Buffer)
{
    unsigned int count;
    for (count=0; count<strlen(Buffer); count++) {
      while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG)); //wait for TX buffer to clear
      EUSCI_A2->TXBUF = Buffer[count]; //send character
    }   //end for()
}

char GetCharBluetooth(void) { //polling
    char ReceivedChar;
    while(!(EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG));
    ReceivedChar = EUSCI_A2->RXBUF;
    return ReceivedChar;
} //end GetChar(void)

void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)// Check if receive flag is set (value ready in RX buffer)
    {
        uint8_t Key = EUSCI_A2->RXBUF;
        // Echo character back to screen, otherwise user will not be able to
        //  verify what was typed
        while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG)); // Wait for TX buffer ready
        EUSCI_A2->TXBUF = Key;                 // Echo character to terminal
    }
}

