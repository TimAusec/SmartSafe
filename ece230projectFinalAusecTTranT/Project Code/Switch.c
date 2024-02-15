/*
 * Switch.c
 *
 *  Created on: Feb 4, 2024
 *      Author: Timothy Ausec
 *      Contributor: Tuan Tran
 */
#include "Switch.h"
bool switch1Flag=false;

/* Define static variables */
#define DEFAULT_CLOCK_FREQUENCY_KHZ 500
#define DEBOUNCE_DELAY_TIME_MS      30
#define DEBOUNCE_DELAY_COUNT        DEFAULT_CLOCK_FREQUENCY_KHZ*DEBOUNCE_DELAY_TIME_MS

void Debounce()
{
    volatile int i;
    for (i = 0; i < DEBOUNCE_DELAY_COUNT; i++)
        ;
}
void ConfigureSW1(void)
{//see section 12.4 of the MSP432P4XX technical reference manual
    /*Configure GPIO Pins*/
    Switch1_Port->SEL0 &= ~Switch1_Pin;      // set Switch1 for GPIO
    Switch1_Port->SEL1 &= ~Switch1_Pin;      // set Switch1 for GPIO
    Switch1_Port->DIR &= ~Switch1_Pin;       // set Switch1 as input
    Switch1_Port->OUT |= Switch1_Pin;       //select pull-up resistor
    Switch1_Port->REN |= Switch1_Pin;        // enable internal pull-up resistor on Switch1
    Switch1_Port->IFG&=~Switch1_Pin;
    Switch1_Port->IE |= Switch1_Pin;        // enable interrupts Switch1
    Switch1_Port->IES |= Switch1_Pin;       //set input interrupts with a high to low transition
    /*Enable NVIC In The ISER Register*/
    NVIC->ISER[1] |= (1 << (PORT2_IRQn - DMA_INT2_IRQn)); //see msp432P4111.h + Table 4-60 in the MSP432P4111X Data Sheet
}

void WaitForSwitch1ToOpen()
{
    Debounce(); // press debounce
    while(!(CheckSW1()));        // wait for release
    Debounce(); //release debounce
}
void PORT2_IRQHandler(void)
{
    if ((Switch1_Pin) & (Switch1_Port->IFG)) //Check if switch1 pin started IRQ
        {
            HandleSwitch1Pressed();
            Switch1_Port->IFG &= ~Switch1_Pin; //clear interrupt flags
        }
    int pins=Switch1_Port->IFG;
    Switch1_Port->IFG &= ~pins; //clear interrupt flags
}

void HandleSwitch1Pressed()
{
    switch1Flag=true;
    WaitForSwitch1ToOpen();
}

bool CheckSW1()
{
    return ((Switch1_Pin) & (Switch1_Port->IFG));
}

bool GetSwitch1Flag()
{
    bool flag=switch1Flag;
    if(switch1Flag)
    {
        switch1Flag=CheckSW1();
    }
    return flag;
}
