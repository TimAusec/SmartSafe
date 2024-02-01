/*
 * KeyPad.c
 *
 *  Created on: Jan 31, 2024
 *      Author: ausectj
 */
#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"
#include "stdbool.h"

uint8_t SCAN_OUTPUT_SEQUENCE[5] = { BIT7, BIT6, BIT5, BIT4, 0 }; //0 is end bit

#define KEYPAD_PORT P2
#define OUTPUT_PINS BIT7|BIT6|BIT5|BIT4
#define INPUT_PINS  BIT3|BIT2|BIT1|BIT0

uint16_t currentCode[4] = { zero, zero, zero, zero };

extern void ConfigKeyPad()
{

}
key ConvertInputToKey(char *input)
{
    key key[4];
    int index;
    for (index = 0; index < 3; index++)
    {
        //
    }
    return key;
}

key GetKeyPressed()
{
    int currentRow;
    char input[4];
    key inputKey;
    KEYPAD_PORT->OUT |= SCAN_OUTPUT_SEQUENCE[0];
    input[0] = KEYPAD_PORT->IN;
    for (currentRow = 1; currentRow <= 3; currentRow++)
    {
        KEYPAD_PORT->OUT &= ~SCAN_OUTPUT_SEQUENCE[currentRow - 1]; //pull previous bit down
        KEYPAD_PORT->OUT |= SCAN_OUTPUT_SEQUENCE[currentRow]; //pull row up
        input[currentRow] = KEYPAD_PORT->IN;
    }
    inputKey = ConvertInputToKey(input);
    return inputKey;
}

void SaveKeyToCode(key key)
{
    static int currentIndex = 0; //initializes only once
    currentCode[currentIndex] = key;
    currentIndex = (currentIndex + 1) % 4;
}

bool IsCorrectCode()
{

}

bool ExceededMaxTries()
{

}

void IRQ()
{
    //check if button pressed
    HandleButtonPressed();
}

void HandleButtonPressed()
{
    key pressed = GetKeyPressed();
    SaveKeyToCode(pressed);
}
