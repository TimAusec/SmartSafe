/*
 * KeyPad.c
 *
 *  Created on: Jan 31, 2024
 *      Author: Timothy Ausec
 */
#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"
#include <stdio.h>

/*Define ports and pins*/
#define KEYPAD_PORT P4
#define OUTPUT_PINS   BIT7|BIT6|BIT5|BIT4
#define INPUT_PINS    BIT3|BIT2|BIT1|BIT0

/*Define Hardware Requirements*/
const int NUMBER_OF_ROWS = 4;
const int NUMBER_OF_COLUMNS = 4;

/*Define Security Threshold*/
const int MAX_TRIES_ALLOWED = 4;
int currentTriesCount = 0;

/*Define Codes*/
const key CORRECT_CODE[4] = { one, two, three, four };
const int CODE_LENGTH = sizeof(CORRECT_CODE) / sizeof(CORRECT_CODE[0]);
key currentCode[4]={INVALID,INVALID,INVALID,INVALID};

int length(key array[])
{
    return sizeof(array) / sizeof(array[0]);
}

void ConfigKeyPad()
{                           //see section 12.4 of the technical reference manual
    KEYPAD_PORT->DIR |= OUTPUT_PINS;            //set output pins to output mode
    KEYPAD_PORT->DIR &= ~INPUT_PINS;              //set input pins to input mode
    KEYPAD_PORT->REN &= ~INPUT_PINS;     //disable pullup resistors for input pins
    KEYPAD_PORT->SEL0 |= ~(OUTPUT_PINS | INPUT_PINS ); //primary function for all pins
    KEYPAD_PORT->SEL1 |= ~(OUTPUT_PINS | INPUT_PINS ); //primary function for all pins
    KEYPAD_PORT->IE &= ~(OUTPUT_PINS);       //disable interrupts on output pins
    KEYPAD_PORT->IE |= INPUT_PINS;             //enable interrupts on input pins
    KEYPAD_PORT->IES &= ~INPUT_PINS; //set input interrupts with a low-to-high transition
    KEYPAD_PORT->OUT |= OUTPUT_PINS; //set the output pins low to start reading
    KEYPAD_PORT->IFG = 0; //clear interrupt flags
    int dummy=0;
    ClearCode(dummy);
    NVIC->ISER[1] = (1 << (PORT4_IRQn-DMA_INT2_IRQn));                     //see msp432P4111.h
}

key ConvertInputToKey(char input[])
{
    int row=0;
    key key=INVALID;
    for (row = 0; row < NUMBER_OF_ROWS ; row++)
    {
        //each row has NUMBER_OF_COLUMNS columns. Since the key enum is numbered left to right and then top to bottom with 0 being the one key,
        //the first key in each row is the row * number of columns
        switch (input[row])
        {
        case 8 :
            key = NUMBER_OF_COLUMNS * row;
            return key;
        case 4 :
            key = NUMBER_OF_COLUMNS * row + 1;
            return key;
        case 2 :
            key = NUMBER_OF_COLUMNS * row + 2;
            return key;
        case 1 :
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
    char input[NUMBER_OF_ROWS]={0};
    const uint8_t SCAN_OUTPUT_SEQUENCE[NUMBER_OF_COLUMNS] = { BIT7, BIT6,
                                                              BIT5, BIT4 };
    int index=0;
    key inputKey = INVALID;
    KEYPAD_PORT->OUT &= ~(OUTPUT_PINS); //set all output low

    KEYPAD_PORT->OUT |= SCAN_OUTPUT_SEQUENCE[0]; //pull first row high
    input[0] = (KEYPAD_PORT->IN) & (INPUT_PINS); //read input values
    for (currentRow = 1; currentRow <= NUMBER_OF_ROWS ; currentRow++)
    {
        KEYPAD_PORT->OUT &= ~(SCAN_OUTPUT_SEQUENCE[currentRow - 1]); //pull previous bit low
        KEYPAD_PORT->OUT |= (SCAN_OUTPUT_SEQUENCE[currentRow]); //pull current row high
        input[currentRow] = (KEYPAD_PORT->IN) & (INPUT_PINS); // read input values
    }
    KEYPAD_PORT->OUT |= OUTPUT_PINS; //set the output pins high to start reading
    inputKey = ConvertInputToKey(input);
    return inputKey;

}

void ClearCode(int *currentIndex)
{
    int index;
    *currentIndex = 0;
    for (index = 0; index < CODE_LENGTH ; index++)
    {
        currentCode[index] = INVALID;
    }
}

void SaveKeyToCode(key inputKey)
{
    static int currentIndex = 0; //initializes only once
    if (currentIndex == 4)
    {
        ClearCode(&currentIndex);
        currentTriesCount++;
    }
    else{
    currentCode[currentIndex] = inputKey;
    currentIndex = (currentIndex + 1);
    }
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
        for (index = 0; index < CODE_LENGTH ; index++)
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
        KEYPAD_PORT->OUT |= OUTPUT_PINS; //set the output pins high to start reading
        KEYPAD_PORT->IFG = 0; //clear interrupt flags
}

void HandleButtonPressed()
{
    Debounce();
    key pressed = GetKeyPressed();
    SaveKeyToCode(pressed);
    while (GetKeyPressed() != INVALID); //wait for release
    Debounce();
    printf("\n new call: \n %d pressed", pressed);
    printf("\n %d is the first key in the current code", currentCode[0]);
    printf("\n %d is the second key in the current code", currentCode[1]);
    printf("\n %d is the third key in the current code", currentCode[2]);
    printf("\n %d is the fourth key in the current code", currentCode[3]);
}

void SetAllOutputPinsHigh()
{
    KEYPAD_PORT->OUT |= OUTPUT_PINS;
}
