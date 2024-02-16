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

#define CODE_LED_PORT P5
#define FIRST_CODE_LED_PIN BIT4
#define SECOND_CODE_LED_PIN BIT2
#define THIRD_CODE_LED_PIN BIT1
#define FOURTH_CODE_LED_PIN BIT0

#define ACLK 32786
#define INTERRUPT_DELAY_SECONDS 0.5
#define QUARTER_NOTE    ACLK*INTERRUPT_DELAY_SECONDS

/*
 * \brief This function configures the pins and ports for many LEDs
 * Configures 5.0, 5.1, 5.2, and 5.4 for GPIO, and enables interrupts
 *
 * Modified various bits of \b P5SEL0, P5SEL1, P5DIR, and P5OUT registers
 *
 * \return None
 */
extern void ConfigureLEDs(void);

/*
 * \brief Sets P2.4 (Green LED) to on
 *
 * \return None
 */
extern void GreenLEDOn(void);

/*
 * \brief Sets P2.5 (Red LED) to on
 *
 * \return None
 */
extern void RedLEDOn(void);

/*
 * \brief Sets P2.4 (Green LED) to off
 *
 * \return None
 */
extern void GreenLEDOff(void);

/*
 * \brief Sets P2.5 (Red LED) to off
 *
 * \return None
 */
extern void RedLEDOff(void);

/*
 * \brief Turns the red LED off and starts the green LED blinking to indicate unlocked
 * Configures TIMER_A1 CTL in up mode with interrupts enabled
 * Sets blinking flag to true
 *
 * \return None
 */
extern void LEDIndicateUnlocked(void);

/*
 * \brief Turns the red LED on and green LED off to indicate locked (security mode)
 *
 * \return None
 */
extern void LEDIndicateSecurityMode(void);

/*
 * \brief Turns the red LED on and green LED off to indicate safe is locked
 *
 * \return None
 */
extern void LEDIndicateLocked(void);

/*
 * \brief Turns both LED's off to indicate closed
 *
 * \return None
 */
extern void LEDIndicateClosed(void);

/*
 * \brief Turns the green LED from blinking to solid to indicate open and turns the red LED off
 *
 * \return None
 */
extern void LEDIndicateOpen(void);

/*
 * \brief Turns on LED's sequentially when a button is pressed on the keypad
 * Indicates how many digits have been entered
 *
 * \return None
 */
extern void ActivateAppropriateCodeLEDs(int codeIndex);
#endif /* LED_H_ */
