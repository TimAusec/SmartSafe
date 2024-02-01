/*
 * LED.h
 *
 *  Created on: Feb 1, 2024
 *      Author: tysonla
 */

#ifndef LED_H_
#define LED_H_

#include "msp.h"

#define LED_Port P2

extern void ConfigureLED(void);
extern void led1On(void);
extern void led2On(void);
extern void led1Off(void);
extern void led2Off(void);

#endif /* LED_H_ */
