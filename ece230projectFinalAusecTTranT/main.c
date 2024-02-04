#include "msp.h"

#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"
#include "LED.h"
#include <servoDriver.h>
#include "csHFXT.h"
#include "servoDriver.h"

#define Switch1_Port P1
#define Switch1_Pin BIT1

void configureSW1(void) {
    Switch1_Port->SEL0 &= ~Switch1_Pin;      // set P1.1 for GPIO
    Switch1_Port->SEL1 &= ~Switch1_Pin;
    Switch1_Port->DIR &= ~Switch1_Pin;       // set P1.1 as input
    Switch1_Port->OUT |= Switch1_Pin;        // enable internal pull-up resistor on P1.1
    Switch1_Port->REN |= Switch1_Pin;
}

void Debounce() {
    volatile int i;
    for(i = 0; i < 15000; i++);
}

void WaitForSwitchToOpen()
{
    Debounce();
    while(!(Switch1_Port->IN & Switch1_Pin));        // wait for release
    Debounce();
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	ConfigureLED();
	configHFXT();
	configureSW1();
	initServoMotor();
    ConfigKeyPad();
    __enable_irq();

	while(1){
	    if (!(Switch1_Port->IN & Switch1_Pin) & (IsOpenCode())) {
	        incrementNinetyDegree();
	        WaitForSwitchToOpen();
            int dummy=0;
            ClearCode(&dummy);
            led1On();
	    }
        else
        {
            if(IsExceededMaxTries())
            {
                led1Off();
                led2On();
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
	}
}
