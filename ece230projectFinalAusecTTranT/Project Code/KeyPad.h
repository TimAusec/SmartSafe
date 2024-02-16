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

/*
 * \brief Configures the keypad
 * Sets input pins (P4.0-P4.3) to inputs and output pins (P4.4-P4.7) to outputs
 * Enables interrupts on input pins and disables them on output pins
 * Sets flags to appropriate start conditions
 *
 * Modified P4.0-P4.3 DIR, SEL0, and SEL1 registers
 * Modified P4.4-P4.7 DIR, SEL0, and SEL1 registers
 *
 * \return None
 */
extern void ConfigKeyPad(void);

/*
 * \brief Converts keypad inputs to its associated enum key
 * Sets the key to the appropriate value based on what keypad is pressed
 *
 * \return Key
 */
extern key ConvertInputToKey(char input[]);

/*
 * \brief Determines which row and column the button press came from to translate into a key
 * Sets outputs to high
 * Reads input values from the keypad and stores them in input array
 * Calls convertInputToKey and inputs the updated input array to convert Input to keys
 *
 * Modified P4IN & P4OUT
 *
 * \return None
 */
extern key GetKeyPressed(void);

/*
 * \brief Sets all entries in the currentCode array to INVALID/NULL
 * If tries exceeded when this is called, it resets the access attempts counter
 *
 * Modified triesExceededFlag, codeClearedFlag, and openCodeFlag
 *
 * \return None
 */
extern void ClearCode(void);

/*
 * \brief Saves the current input key into the currentCode array
 *
 * Modified codeClearedFlag and triesExceededFlag
 *
 * \return None
 */
extern void SaveKeyToCode(key key);

/*
 * \brief Checks each entry in the currentCode array to see if the correct code has been inputted
 *
 * Modified match array, bool truth, openCodeFlag, and wrongCodeFlag
 */
extern void CalculateIsOpenCode(void);

/*
 * \brief Handles the operation of the above functions
 * Determines if a key has been pressed that is not the stop key has been pressed
 * If it's a valid key, call SaveKeyToCode and CalculateIsOpenCode
 * Also handles the stop and close keys
 *
 * Modified stopCodeFlag, closeCodeFlag, and wrongCodeFlag
 *
 * \return None
 */
extern void HandleKeyPadButtonValue(key pressed);

/*
 * \brief Handles keypad button presses
 * Calls handleKeyPadButtonValue once pressed and Waits for button to be released
 * Prints key to the console
 *
 * \return None
 */
extern void HandleKeyPadButtonPressed(void);

/*
 * \brief Returns CloseCodeFlag
 *
 * \return bool
 */
extern bool GetCloseCodeFlag(void);

/*
 * \brief Returns StopCodeFlag
 *
 * \return bool
 */
extern bool GetStopCodeFlag(void);

/*
 * \brief Returns TriesExceededFlag
 *
 * \return bool
 */
extern bool GetTriesExceededFlag(void);

/*
 * \brief Returns OpenCodeFlag
 *
 * \return bool
 */
extern bool GetOpenCodeFlag(void);

/*
 * \brief Returns CurrentTriesCount
 *
 * \return int
 */
extern int GetCurrentTriesCount();

/*
 * \brief Returns CurrentIndex
 *
 * \return int
 */
extern int GetCurrentCodeIndex();

/*
 * \brief Returns WrongCodeFlag
 *
 * \return bool
 */
extern bool GetWrongCodeFlag();
