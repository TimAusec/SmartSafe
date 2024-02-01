/*! \file */
/*!
 * servoDriverTemplate_jjs.c
 * ECE230-01/02 Winter 2023-2024
 * Date: January 11, 2024
 * Description: Servo motor driver for MSP432P4111 Launchpad.
 *              Assumes SMCLK configured with 48MHz HFXT as source.
 *              Uses Timer_A2 and P5.6 (TA2.1)
 *
 *                 MSP432P4111
 *             -------------------
 *         /|\|                   |
 *          | |                   |
 *          --|RST                |
 *            |      (TA2.1) P5.6 |----> Servo
 *            |                   |
 *            |              PJ.2 |------
 *       S1-->|P1.1               |     |
 *            |                   |    HFXT @ 48MHz
 *            |                   |     |
 *            |              PJ.3 |------
 *
 */

#include <servoDriver.h>
#include "msp.h"

/* Global Variables  */
uint16_t pulseWidthTicks = SERVO_MIN_ANGLE;


void initServoMotor(void) {
    // DONE configure servo pin (P5.6) for primary module function (TA2.1),
    //  output, initially LOW
    P5->OUT &= ~BIT6;
    P5->SEL1 &= ~BIT6;
    P5->SEL0 |= BIT6;
    P5->DIR |= BIT6;

    /* Configure Timer_A2 and CCR1 */
    // Set period of Timer_A2 in CCR0 register for Up Mode
    TIMER_A2->CCR[0] = SERVO_TMR_PERIOD;
    // Set initial positive pulse-width of PWM in CCR1 register
    TIMER_A2->CCR[1] = SERVO_MIN_ANGLE;

    // DONE configure TA2CCR1 for Compare mode, Reset/Set output mode, with interrupt disabled
    TIMER_A2->CCTL[1] |= BIT7 | BIT6 | BIT5;
    TIMER_A2->CCTL[1] &= ~(BIT8 | BIT4);

    // Configure Timer_A2 in Up Mode, with source SMCLK, prescale 16:1, and
    //  interrupt disabled  -  tick rate will be 3MHz (for SMCLK = 48MHz)
    // DONE configure Timer_A2 (requires setting control AND expansion register)
    TIMER_A2->CTL |= BIT9 | BIT7 | BIT4;
    TIMER_A2->CTL &= ~BIT1;
    TIMER_A2->EX0 |= BIT0 | BIT1;
}

void increment100Degree(void) {
    // update pulse-width for <current angle> + <10 degrees>
    pulseWidthTicks += FIFTY_DEGREE_TICK;
    if (pulseWidthTicks > SERVO_MAX_ANGLE) {
        pulseWidthTicks = SERVO_MIN_ANGLE;
    }
    // DONE update CCR1 register to set new positive pulse-width
    TIMER_A2->CCR[1] = pulseWidthTicks;
}

//void setServoAngle(uint16_t angle) {
    // NOT NEEDED FOR EXERCISE - but would be useful function for driver
//}
