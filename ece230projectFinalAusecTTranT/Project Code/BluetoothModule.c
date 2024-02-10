/*
 * BluetoothModule.c
 *
 *  Created on: Feb 6, 2024
 *      Author: Timothy Ausec
 */

#include "BluetoothModule.h"

#define SYSTEMCLOCK 48e6    //Hz
#define BAUDRATE    9600   //bits per seconds (Hz)
#define ClockPrescalerValue SYSTEMCLOCK/(16*BAUDRATE)
#define FirstModulationStage 2  //48MHz, 9600 Baud
#define SecondModulationStage 16 //48MHz, 9600 Baud
char received=0;
uint32_t newColor = 0;
int8_t digitsReceived = 0;

void InitBluetooth()
{
    //TODO: Implement
    BLUETOOTH_PORT->SEL0 |= BLUETOOTH_PINS; // set 2-UART pins as secondary function
    BLUETOOTH_PORT->SEL1 &= ~(BLUETOOTH_PINS);

    /* Configure UART
     *  Asynchronous UART mode, 8E1 (8-bit data, even parity, 1 stop bit),
     *  LSB first by default, SMCLK clock source
     */
    EUSCI_A2->CTLW0 |= EUSCI_A_CTLW0_SWRST; // Put eUSCI in reset to configure eUSCI
//bit 15 = 1 to enable parity; bit14=1 even parity; bit13=0 for LSB first;
//bit12=0 for 8-bit mode; bit11=0 for one stop bit; bits7-6 = 0b10 for SMCLK
    EUSCI_A2->CTLW0 |= 0b1100000010000000;
    EUSCI_A2->BRW=ClockPrescalerValue;//Section 24.3.10 of Technical Reference manual

    //  set clock prescaler in eUSCI_A0 baud rate control register
    EUSCI_A2->MCTLW = (SecondModulationStage<<8)|(FirstModulationStage<<4)+1;    //enalble oversampling
    //  configure baud clock modulation in eUSCI_A0 modulation control register

    EUSCI_A2->CTLW0 &= ~EUSCI_A_CTLW0_SWRST;    // Initialize eUSCI
    EUSCI_A2->IFG &= ~EUSCI_A_IFG_RXIFG;        // Clear eUSCI RX interrupt flag
    EUSCI_A2->IE |= EUSCI_A_IE_RXIE;            // Enable USCI_A0 RX interrupt

    NVIC->ISER[0] |= (1)<<EUSCIA2_IRQn;
}

void SendCharArrayBluetooth(char *Buffer)
{
    unsigned int count;
    for (count=0; count<strlen(Buffer); count++) {
      while(!(EUSCI_A0->IFG & EUSCI_A_IFG_TXIFG)); //wait for TX buffer to clear
      EUSCI_A0->TXBUF = Buffer[count]; //send character
    }   //end for()
}

char GetCharBluetooth()
{
    return received;
}

void EUSCIA2_IRQHandler(void)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)// Check if receive flag is set (value ready in RX buffer)
    {
        received = EUSCI_A2->RXBUF; // Note that reading RX buffer clears the flag
        digitsReceived++;
        while(!(EUSCI_A2->IFG & EUSCI_A_IFG_TXIFG)); // Wait for TX buffer ready
        EUSCI_A2->TXBUF = received;                 // Echo character to terminal

    }
}

