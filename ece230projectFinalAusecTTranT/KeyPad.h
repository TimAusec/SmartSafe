/*
 * KeyPad.h
 *
 *  Created on: Jan 31, 2024
 *      Author: Timothy Ausec
 */
#ifndef KEYPAD_H_
#define KEYPAD_H_
#endif

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
{
    one,
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
//TODO: Document
extern key ConvertInputToKey(char input[]);
//TODO: Document
extern key GetKeyPressed();
//TODO: Document
extern void SaveKeyToCode(key key);
//TODO: Document
extern void ClearCode();
//TODO: Document
extern bool IsOpenCode();
//TODO: Document
extern bool IsCloseCode();
//TODO: Document
extern bool IsExceededMaxTries();
//TODO: Document
extern bool IsStopCode();
//TODO: Document
extern void HandleButtonPressed();
//TODO: Document
extern void SetAllOutputPinsHigh();
//TODO: Document
