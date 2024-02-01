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

void debounce() {
    volatile int i;
    for(i = 0; i < 15000; i++);
}

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	ConfigureLED();
	led1on();
	configHFXT();
	configureSW1();
	initServoMotor();
	while(1){
	    if (!(Switch1_Port->IN & Switch1_Pin)) {
	        incrementNinetyDegree();
	        debounce();
	        while(!(Switch1_Port->IN & Switch1_Pin));        // wait for release
	        debounce();
	    }
	}
}
