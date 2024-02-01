/*
 * KeyPad.h
 *
 *  Created on: Jan 31, 2024
 *      Author: ausectj
 */
#ifndef KEYPAD_H_
#define KEYPAD_H_

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"

#endif
#include "msp.h"
#include "stdbool.h"

uint16_t SCAN_OUTPUT_SEQUENCE[8]  = {0b01111111,0b10111111,0b11011111
                                 ,0b11101111,0b11110111,0b11111011,0b11111101,0b11111110};

typedef enum _key {one, two, three, A, four, five, six, B, seven, eight, nine, C, asterisk, zero, pound, D} key;

#define CORRECT_CODE;

extern void ConfigKeyPad();

extern unsigned char GetKeyPressed();

extern void SaveKeyToCode(key);

extern bool IsCorrectCode();

extern bool ExceededMaxTries();

extern void IRQ();

extern void HandleButtonPressed();


#endif
