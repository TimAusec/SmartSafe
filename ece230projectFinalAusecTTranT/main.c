#include "msp.h"


/**
 * main.c
 * Edit
 */



//commment commment comment


#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"
#include "LED.h"

void main(void)
{

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	ConfigureLED();
	    led1on();


}
