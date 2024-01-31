#include "msp.h"


/**
 * main.c
 */



//commment commment comment


#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
