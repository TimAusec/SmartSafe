/*! \file */
/*!
 * servoDriver.h
 *
 * Description: Servo motor driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 48MHz HFXT as source.
 *              Uses Timer_A2 and P5.6 (TA2.1)
 *
 * Author: Tuan Tran
 * Contributors:
 *         Timothy Ausec
 * Date: February 1, 2024
 */

#ifndef SERVODRIVER_H_
#define SERVODRIVER_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

#include "msp.h"
#include "KeyPad.h"

// add tick count values for constants
#define ACLK                            32786
#define FREQUENCY                       25
#define SERVO_TMR_PERIOD                ACLK/FREQUENCY
#define SERVO_ZERO_DEGREE_PULSE_WIDTH   500e-6
#define SERVO_NINETY_DEGREE_PULSE_WIDTH  1500e-6
#define SERVO_ZERO_DEGREE_TICKS         ACLK*SERVO_ZERO_DEGREE_PULSE_WIDTH
#define SERVO_NINETY_DEGREE_TICKS       ACLK*SERVO_NINETY_DEGREE_PULSE_WIDTH


/*!
 * \brief This function configures pins and timer for servo motor driver
 *
 * This function configures P5.6 as output pin for servo drive signal and
 *  initializes Timer_A2 CCR1 for PWM output
 *
 * Modified bit 6 of \b P2DIR register and \b P2SEL registers.
 * Modified \b TA2CTL, \b TA2CCTL1 and CCR registers.
 *
 * \return None
 */
extern void InitServoMotor(void);


/*!
 * \brief This increments servo angle 10 degrees, with wrap-around
 *
 * This function increments servo angle by 10 degrees. If new angle exceeds max
 *  angle (+90), it wraps around to min angle (-90)
 *
 * Modified \b TA2CCR1 register.
 *
 * \return None
 */
extern void OpenServo(void);


/*!
 * \brief This function sets angle of servo to 90 degrees
 *
 * Modified \b TA2CCR1 register.
 *
 * \return None
 */
extern void CloseServo(void);
//TODO: Document

extern bool GetServoOpenFlag(void);
//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif /* SERVODRIVER_H_ */
