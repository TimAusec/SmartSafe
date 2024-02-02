<<<<<<< Updated upstream
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

typedef enum _key {one, two, three, A, four, five, six, B, seven, eight, nine, C, asterisk, zero, pound, D} key;

#define CORRECT_CODE;

extern void ConfigKeyPad();

extern unsigned char GetKeyPressed();

extern void SaveKeyToCode(key);

extern bool IsCorrectCode();

extern bool ExceededMaxTries();

extern void IRQ();

extern void HandleButtonPressed();

extern key ConvertInputToKey(char* input);
=======
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

typedef enum _bool
{
false, true
} bool;
typedef enum _key
{one,
two,
three,
A,
four,
five,
six,
B,
seven,
eight,
nine,
C,
asterisk,
zero,
pound,
D,
INVALID
} key;

extern void ConfigKeyPad();

extern key ConvertInputToKey(char input[]);

extern key GetKeyPressed();

extern void SaveKeyToCode(key key);

extern void ClearCode();

extern bool IsCorrectCode();

extern bool IsExceededMaxTries();

extern void IRQ();

extern void HandleButtonPressed();

extern void SetAllOutputPinsHigh();
>>>>>>> Stashed changes
