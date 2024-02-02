<<<<<<< Updated upstream
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
=======
/*
 * KeyPad.c
 *
 *  Created on: Jan 31, 2024
 *      Author: ausectj
 */
#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"

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

/*Define ports and pins*/
#define KEYPAD_PORT P2
#define OUTPUT_PINS BIT7|BIT6|BIT5|BIT4
#define INPUT_PINS  BIT3|BIT2|BIT1|BIT0

/*Define Codes*/
const key CORRECT_CODE[] = { zero, zero, zero, one };
const uint8_t SCAN_OUTPUT_SEQUENCE[5] = { BIT7, BIT6, BIT5, BIT4, 0 }; //0 is end bit
const int CODE_LENGTH = sizeof(CORRECT_CODE) / sizeof(CORRECT_CODE[0]);

key currentCode[CODE_LENGTH] = { zero, zero, zero, zero };

/*Define Hardware Requirements*/
const int NUMBER_OF_ROWS = 4;
const int NUMBER_OF_COLUMNS = 4;

/*Define Security Threshold*/
const int MAX_TRIES_ALLOWED = 4;
const int currentTriesCount = 0;

void ConfigKeyPad()
{ //see section 12.4 of the technical reference manual
    KEYPAD_PORT->DIR |= OUTPUT_PINS | (~INPUT_PINS ); //set output pins to output mode
    KEYPAD_PORT->DIR &= ~INPUT_PINS; //set input pins to input mode
    KEYPAD_PORT->REN |= ~(OUTPUT_PINS) | INPUT_PINS; //enable pullup resistors for input pins
    KEYPAD_PORT->SEL0 |= ~(OUTPUT_PINS | INPUT_PINS ); //primary function for all pins
    KEYPAD_PORT->SEL1 |= ~(OUTPUT_PINS | INPUT_PINS ); //primary function for all pins
    KEYPAD_PORT->IE &= ~(OUTPUT_PINS); //disable interrupts on output pins
    KEYPAD_PORT->IE |= INPUT_PINS;//enable interrupts on input pins
    KEYPAD_PORT->IES |= ~(OUTPUT_PINS) | INPUT_PINS; //set input interrupts with a low-to-high transition
    KEYPAD_PORT->OUT |= OUTPUT_PINS; //set the output pins high to start reading
}

key ConvertInputToKey(char input[])
{
    int index;
    key key;
    for (index = 0; index < NUMBER_OF_ROWS - 1; index++)
    {
        switch (input[index])
        {
        case BIT0 :
            key = NUMBER_OF_COLUMNS * index;
            break;
        case BIT1 :
            key = NUMBER_OF_COLUMNS * index + 1;
            break;
        case BIT2 :
            key = NUMBER_OF_COLUMNS * index + 2;
            break;
        case BIT3 :
            key = NUMBER_OF_COLUMNS * index + 3;
            break;
        default:
            //Invalid input
            break;
        }
    }
    return key;
}

key GetKeyPressed()
{
    int currentRow;
    char input[NUMBER_OF_ROWS];
    key inputKey;

    KEYPAD_PORT->OUT |= SCAN_OUTPUT_SEQUENCE[0];
    input[0] = KEYPAD_PORT->IN;
    for (currentRow = 1; currentRow <= NUMBER_OF_ROWS - 1; currentRow++)
    {
        KEYPAD_PORT->OUT &= ~SCAN_OUTPUT_SEQUENCE[currentRow - 1]; //pull previous bit down
        KEYPAD_PORT->OUT |= SCAN_OUTPUT_SEQUENCE[currentRow]; //pull row up
        input[currentRow] = KEYPAD_PORT->IN;
    }
    inputKey = ConvertInputToKey(input);
    return inputKey;
}

void ClearCode(int* currentIndex)
{
    int index;
    *currentIndex=0;
    for (index = 0; index < CODE_LENGTH - 1; index++)
    {
        currentCode[index] = INVALID;
    }
}

void SaveKeyToCode(key key)
{
    static int currentIndex = 0; //initializes only once
    if (currentIndex % CODE_LENGTH == 0)
    {
        ClearCode(&currentIndex);
    }
    currentCode[currentIndex] = key;
    currentIndex = (currentIndex + 1);
}

bool IsCorrectCode()
{
    int index;
    key match[];
    bool truth;
    for (index = 0; index < CODE_LENGTH - 1; index++)
    {
        match[index] = (currentCode[index] == CORRECT_CODE[index]); //check each entry
    }

    truth = match[0];
    for (index = 1; index < CODE_LENGTH - 1; index++)
    {
        truth &= match[index]; //if all entries are 1, true. if any entry is 0, false.
    }
    return truth;
}

bool IsExceededMaxTries()
{
    if (currentTriesCount > MAX_TRIES_ALLOWED)
    {
        return true;
    }
    return false;
}

void PORT2_IRQHandler() //Check startup_msp432p4111 file
{
//TODO: check if button press caused interrupt
    HandleButtonPressed();
}

void HandleButtonPressed()
{
//TODO: Implement
    currentTriesCount++;
    key pressed = GetKeyPressed();
    SaveKeyToCode(pressed);
}

void SetAllOutputPinsHigh()
{
    KEYPAD_PORT->OUT |= OUTPUT_PINS;
}
>>>>>>> Stashed changes
