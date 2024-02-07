/*
 * LED.h
 *
 *  Created on: Feb 1, 2024
 *      Author: tysonla
 */

#ifndef LED_H_
#define LED_H_

#include "msp.h"
#include "main.h"

#define LED_PORT P2
#define RED_LED_PIN BIT5
#define GREEN_LED_PIN BIT4
#define ACLK 128e3
#define INTERRUPT_DELAY_SECONDS 0.5
#define FULL_NOTE       32768
#define QUARTER_NOTE    ACLK*INTERRUPT_DELAY_SECONDS

extern void ConfigureLEDs(void);
//TODO: Document
extern void GreenLEDOn(void);
//TODO: Document
extern void RedLEDOn(void);
//TODO: Document
extern void GreenLEDOff(void);
//TODO: Document
extern void RedLEDOff(void);
//TODO: Document
extern void LEDIndicateUnlocked(void);
//TODO: Document
extern void LEDIndicateSecurityMode(void);
//TODO: Document
extern void LEDIndicateLocked(void);
//TODO: Document
extern void LEDIndicateClosed(void);

extern void LEDIndicateOpen(void);

#endif /* LED_H_ */
