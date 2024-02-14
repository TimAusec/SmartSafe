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
#include "main.h"
#include "enums.h"

extern void ConfigKeyPad(void);
//TODO: Document
extern key ConvertInputToKey(char input[]);
//TODO: Document
extern key GetKeyPressed(void);
//TODO: Document
extern void ClearCode(void);
//TODO: Document
extern void SaveKeyToCode(key key);
//TODO: Document
extern void CalculateIsOpenCode(void);
//TODO: Document
extern void HandleKeyPadButtonValue(key pressed);
//TODO: Document
extern void HandleKeyPadButtonPressed(void);
//TODO: Document
extern bool GetCloseCodeFlag(void);
//TODO: Document
extern bool GetStopCodeFlag(void);
//TODO: Document
extern bool GetTriesExceededFlag(void);
//TODO: Document
extern bool GetOpenCodeFlag(void);
//TODO: Document
extern int GetCurrentTriesCount();

extern int GetCurrentCodeIndex();

extern bool GetWrongCodeFlag();
