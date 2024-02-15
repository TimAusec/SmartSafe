/*
 * Switch.h
 *
 *  Created on: Feb 4, 2024
 *      Author: ausectj
 */

#ifndef SWITCH_H_
#define SWITCH_H_

#include "main.h"

/*Define ports and pins*/
#define Switch1_Port P2
#define Switch1_Pin BIT6


/*
 * \brief This function is a standard debounce with preset delay time
 *
 * \return None
 */
extern void Debounce();

/*
 * \brief This function configures the pin and port for switch1
 * Configures P2.6 for GPIO, also enables interrupts and NVIC In The ISER Register
 *
 * Modified bit6 of \b P2SEL0, P2SEL1, P2DIR, P2OUT registers etc.
 *
 * \return None
 */
extern void ConfigureSW1(void);

/*
 * \brief This function waits for switch1 to be released once it is pressed
 *
 * \return None
 */
extern void WaitForSwitch1ToOpen(void);

/*
 * \brief This function handles switch1 presses
 *  function sets the switch1Flag = true and calls WaitForSwitch1ToOpen, which waits for switch1 to be released
 *
 * \return None
 */
extern void HandleSwitch1Pressed(void);

/*
 * \brief This function checks the state of switch1flag and returns it
 *
 * \return bool switch1Flag
 */
extern bool GetSwitch1Flag(void);

/*
 * This function returns true if switch1 is pressed and false otherwise
 *
 * \return bool
 */
extern bool CheckSW1(void);

#endif /* SWITCH_H_ */
