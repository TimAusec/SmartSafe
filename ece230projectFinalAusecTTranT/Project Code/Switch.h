/*
 * Switch.h
 *
 *  Created on: Feb 4, 2024
 *      Author: ausectj
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "main.h"

#define Switch1_Port P1
#define Switch1_Pin BIT1

extern void Debounce();
//TODO: Document
extern void ConfigureSW1(void);
//TODO: Document
extern void WaitForSwitch1ToOpen(void);
//TODO: Document
extern void HandleSwitch1Pressed(void);
//TODO: Document
extern bool GetSwitch1Flag(void);
//TODO: Document
extern bool CheckSW1(void);

#endif /* SWITCH_H_ */
