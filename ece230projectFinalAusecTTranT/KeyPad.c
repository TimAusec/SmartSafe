/*
 * KeyPad.c
 *
 *  Created on: Jan 31, 2024
 *      Author: Timothy Ausec
 */
#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"
#include "KeyPadTest.h"
#include <stdio.h>

/*Define ports and pins*/
#define KEYPAD_PORT P4
#define OUTPUT_PINS   BIT7|BIT6|BIT5|BIT4
#define INPUT_PINS    BIT3|BIT2|BIT1|BIT0

/*Define Codes*/
const key CORRECT_CODE[] = { one, two, three, four };
const int CODE_LENGTH = sizeof(CORRECT_CODE) / sizeof(CORRECT_CODE[0]);
key currentCode[CODE_LENGTH] = { INVALID, INVALID, INVALID, INVALID };

/*Define Hardware Requirements*/
const int NUMBER_OF_ROWS = 4;
const int NUMBER_OF_COLUMNS = 4;

/*Define Security Threshold*/
const int MAX_TRIES_ALLOWED = 4;
int currentTriesCount = 0;

int length(key array[])
{
    return sizeof(array) / sizeof(array[0]);
}

void ConfigKeyPad()
{                           //see section 12.4 of the technical reference manual
    KEYPAD_PORT->DIR |= OUTPUT_PINS;            //set output pins to output mode
    KEYPAD_PORT->DIR &= ~INPUT_PINS;              //set input pins to input mode
    KEYPAD_PORT->REN |= INPUT_PINS;     //enable pullup resistors for input pins
    KEYPAD_PORT->REN &= ~OUTPUT_PINS;     //disable pullup resistors for input pins
    KEYPAD_PORT->SEL0 |= ~(OUTPUT_PINS | INPUT_PINS ); //primary function for all pins
    KEYPAD_PORT->SEL1 |= ~(OUTPUT_PINS | INPUT_PINS ); //primary function for all pins
    KEYPAD_PORT->IE &= ~(OUTPUT_PINS);       //disable interrupts on output pins
    KEYPAD_PORT->IE |= INPUT_PINS;             //enable interrupts on input pins
    KEYPAD_PORT->IES |= INPUT_PINS; //set input interrupts with a low-to-high transition
    KEYPAD_PORT->OUT &= ~OUTPUT_PINS; //set the output pins low to start reading

    NVIC->ISER[1] = (1 << (PORT4_IRQn-DMA_INT2_IRQn));                     //see msp432P4111.h
}

key ConvertInputToKey(char input[])
{
    int row;
    key key=INVALID;
    for (row = 0; row < NUMBER_OF_ROWS - 1; row++)
    {
        //each row has NUMBER_OF_COLUMNS columns. Since the key enum is numbered left to right and then top to bottom with 0 being the one key,
        //the first key in each row is the row * number of columns
        switch (input[row])
        {
        case 7 :
            key = NUMBER_OF_COLUMNS * row;
            return key;
        case 11 :
            key = NUMBER_OF_COLUMNS * row + 1;
            return key;
        case 13 :
            key = NUMBER_OF_COLUMNS * row + 2;
            return key;
        case 14 :
            key = NUMBER_OF_COLUMNS * row + 3;
            return key;
        default:
            key = INVALID;
            break;
        }
    }
    return key;
}

key GetKeyPressed()
{
    int currentRow;
    char input[NUMBER_OF_ROWS];
    const uint8_t SCAN_OUTPUT_SEQUENCE[NUMBER_OF_COLUMNS] = { ~BIT7, ~BIT6,
                                                              ~BIT5, ~BIT4 };
    key inputKey = INVALID;

    KEYPAD_PORT->OUT &= SCAN_OUTPUT_SEQUENCE[0];
    input[0] = INPUT_PINS & KEYPAD_PORT->IN;
    for (currentRow = 1; currentRow <= NUMBER_OF_ROWS - 1; currentRow++)
    {
        KEYPAD_PORT->OUT |= ~SCAN_OUTPUT_SEQUENCE[currentRow - 1]; //pull previous bit up
        KEYPAD_PORT->OUT &= SCAN_OUTPUT_SEQUENCE[currentRow]; //pull current row down
        input[currentRow] = INPUT_PINS & KEYPAD_PORT->IN; // get input pin values
    }
    KEYPAD_PORT->OUT &= ~OUTPUT_PINS; //set the output pins low to start reading
    inputKey = ConvertInputToKey(input);
    return inputKey;

}

void ClearCode(int *currentIndex)
{
    int index;
    *currentIndex = 0;
    for (index = 0; index < CODE_LENGTH - 1; index++)
    {
        currentCode[index] = INVALID;
    }
}

void SaveKeyToCode(key inputKey)
{
    static int currentIndex = 0; //initializes only once
    if (currentIndex % CODE_LENGTH == 0)
    {
        ClearCode(&currentIndex);
    }
    key temp1[4]=currentCode;
    currentCode[currentIndex] = inputKey;
    currentIndex = (currentIndex + 1);
    key temp2[4]=currentCode;
}

bool IsOpenCode()
{
    int index;
    bool match[CODE_LENGTH];
    bool truth = true;
    if (IsExceededMaxTries())
    {
        truth = false;
    }
    else
    {
        for (index = 0; index < CODE_LENGTH - 1; index++)
        {
            match[index] = (currentCode[index] == CORRECT_CODE[index]); //check each entry
            truth &= match[index];
        }
        return truth;
    }
}
bool IsCloseCode()
{
    //TODO: Implement
}
bool IsStopCode()
{
    //TODO: Implement
}
bool IsExceededMaxTries()
{
    if (currentTriesCount > MAX_TRIES_ALLOWED)
    {
        return true;
    }
    return false;
}

void PORT4_IRQHandler(void) //Check startup_msp432p4111 file
{
//TODO: check if button press caused interrupt
        HandleButtonPressed();
        KEYPAD_PORT->OUT &= ~OUTPUT_PINS; //set the output pins low to start reading
        KEYPAD_PORT->IFG = 0; //clear interrupt flags
}

void HandleButtonPressed()
{
    currentTriesCount++;
    Debounce();
    key pressed = GetKeyPressed();
    SaveKeyToCode(pressed);
    while (GetKeyPressed() != INVALID)
        ; //wait for release
    Debounce();
    key gamer[4]=currentCode;
    printf("%d pressed", pressed);
}

void SetAllOutputPinsHigh()
{
    KEYPAD_PORT->OUT |= OUTPUT_PINS;
}
