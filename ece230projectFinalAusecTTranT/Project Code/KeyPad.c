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

int currentIndex = 0;

/*Define Security Threshold*/
const int MAX_TRIES_ALLOWED = 2;
int currentTriesCount = 0;
/*Declare Flags*/
bool stopCodeFlag = false;
bool closeCodeFlag = false;
bool triesExceededFlag = false;
bool openCodeFlag = false;
bool codeClearedFlag = false;
bool wrongCodeFlag = false;

/*Define Codes*/
const key CORRECT_CODE[4] = { one, two, three, four };
const key STOP_KEY = asterisk;
const key CLOSE_KEY = pound;
const int CODE_LENGTH = sizeof(CORRECT_CODE) / sizeof(CORRECT_CODE[0]);
key currentCode[4] = { INVALID, INVALID, INVALID, INVALID };

void ConfigKeyPad()
{                //see section 12.4 of the MSP432P4XX technical reference manual
    /*Configure GPIO Pins*/
    KEYPAD_PORT->DIR |= OUTPUT_PINS;            //set output pins to output mode
    KEYPAD_PORT->SEL0 &= ~(OUTPUT_PINS);
    KEYPAD_PORT->SEL1 &= ~(OUTPUT_PINS);

    KEYPAD_PORT->DIR &= ~(INPUT_PINS);            //set input pins to input mode
    KEYPAD_PORT->SEL0 &= ~(INPUT_PINS); //primary function for all pins
    KEYPAD_PORT->SEL1 &= ~(INPUT_PINS); //primary function for all pins

    KEYPAD_PORT->OUT |= (INPUT_PINS);      // select pullup resistors
    KEYPAD_PORT->REN |= (INPUT_PINS); //enable pullup resistors for input pins

    KEYPAD_PORT->IE &= ~(OUTPUT_PINS);       //disable interrupts on output pins
    KEYPAD_PORT->IE |= INPUT_PINS;             //enable interrupts on input pins
    KEYPAD_PORT->IES |= (INPUT_PINS); //set input interrupts with a high-to-low transition

    KEYPAD_PORT->OUT &= ~(OUTPUT_PINS); //set the output pins low to start reading
    KEYPAD_PORT->IFG &= ~(INPUT_PINS);   //clear interrupt flags
    /*Clear Code*/
    ClearCode();
    currentTriesCount = 0;
    /*Set Flags*/
    stopCodeFlag = false;
    closeCodeFlag = false;
    triesExceededFlag = false;
    openCodeFlag = false;
    codeClearedFlag = false;
    /*Enable NVIC In The ISER Register*/
    NVIC->ISER[1] |= (1 << (PORT4_IRQn - DMA_INT2_IRQn)); //see msp432P4111.h + Table 4-60 in the MSP432P4111X Data Sheet
}

key ConvertInputToKey(char input[])
{
    int row = 0;
    key key = INVALID;
    for (row = 0; row < NUMBER_OF_ROWS; row++)
    {
        //each row has NUMBER_OF_COLUMNS columns. Since the key enum is numbered left to right and then top to bottom with 0 being the one key,
        //the first key in each row is the row * number of columns
        switch (input[row])
        {
        case 7:
            key = NUMBER_OF_COLUMNS * row;
            return key;
        case 11:
            key = NUMBER_OF_COLUMNS * row + 1;
            return key;
        case 13:
            key = NUMBER_OF_COLUMNS * row + 2;
            return key;
        case 14:
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
    char input[NUMBER_OF_ROWS] = { 0 };
    const uint8_t SCAN_OUTPUT_SEQUENCE[NUMBER_OF_COLUMNS] = { BIT7, BIT6,
    BIT5,
                                                              BIT4 };
    key inputKey = INVALID;
    KEYPAD_PORT->OUT |= (OUTPUT_PINS); //set all output high

    KEYPAD_PORT->OUT &= ~(SCAN_OUTPUT_SEQUENCE[0]); //pull first row low
    input[0] = (KEYPAD_PORT->IN) & (INPUT_PINS); //read input values
    for (currentRow = 1; currentRow <= NUMBER_OF_ROWS; currentRow++)
    {
        KEYPAD_PORT->OUT |= (SCAN_OUTPUT_SEQUENCE[currentRow - 1]); //pull previous bit high
        KEYPAD_PORT->OUT &= ~(SCAN_OUTPUT_SEQUENCE[currentRow]); //pull current row low
        input[currentRow] = (KEYPAD_PORT->IN) & (INPUT_PINS); // read input values
    }
    KEYPAD_PORT->OUT &= ~(OUTPUT_PINS); //set the output pins low to start reading
    inputKey = ConvertInputToKey(input);
    return inputKey;

}

void ClearCode()
{
    int index;
    for (index = 0; index < CODE_LENGTH; index++)
    { // Reset code
        currentCode[index] = INVALID;
    }
    if (triesExceededFlag | openCodeFlag)
    { // if the tries exceeded flag or open code flag are activated when this method is called, rest access attempt count
        currentTriesCount = 0;
        triesExceededFlag = false;
    }
    currentIndex = 0;
    codeClearedFlag = true; //let everyone know the code is clear
    openCodeFlag = false; //let everyone know the openCode is not set
}

void SaveKeyToCode(key inputKey)
{
    if (codeClearedFlag)
    {
        currentIndex = 0;
        codeClearedFlag = false;
    }
    if (currentIndex == 4)
    {
        ClearCode();
        currentIndex = 0;
        currentTriesCount++;
        if (currentTriesCount > MAX_TRIES_ALLOWED)
        {
            triesExceededFlag = true;
        }
    }
    else
    {
        currentCode[currentIndex] = inputKey;
        currentIndex = (currentIndex + 1);
    }
}

void CalculateIsOpenCode()
{
    int index;
    bool match[CODE_LENGTH];
    bool truth = true;
    if (triesExceededFlag)
    {
        truth = false;
    }
    else
    {
        for (index = 0; index < CODE_LENGTH; index++)
        {
            match[index] = (currentCode[index] == CORRECT_CODE[index]); //check each entry
            truth &= match[index];
        }
    }
    openCodeFlag = truth;
    if (openCodeFlag)
    {
        currentIndex = 0;
        wrongCodeFlag = false;
    }
    if (!openCodeFlag & (currentIndex == 4))
    {
        wrongCodeFlag = true;
    }
    if (wrongCodeFlag & (currentIndex != 4))
    {
        wrongCodeFlag = false;
    }
}

void PORT4_IRQHandler(void) //Check startup_msp432p4111 file
{
    if ((INPUT_PINS) & (KEYPAD_PORT->IFG)) //Check if input pins started IRQ
    {
        HandleKeyPadButtonPressed();
        KEYPAD_PORT->IFG &= ~(INPUT_PINS); //clear interrupt flags
    }
    int pins = (KEYPAD_PORT->IFG);
    (KEYPAD_PORT->IFG) &= ~pins;

}
void PrintMessageToConsole(key pressed)
{
    printf("\n new call: \n %d pressed", pressed);
    printf("\n %d is the first key in the current code", currentCode[0]);
    printf("\n %d is the second key in the current code", currentCode[1]);
    printf("\n %d is the third key in the current code", currentCode[2]);
    printf("\n %d is the fourth key in the current code", currentCode[3]);
}

void HandleKeyPadButtonValue(key pressed)
{
    if ((pressed != STOP_KEY) & (pressed != CLOSE_KEY))
    {
        SaveKeyToCode(pressed);
        CalculateIsOpenCode();
        stopCodeFlag = false;
        closeCodeFlag = false;
    }
    else if ((pressed == STOP_KEY) & !openCodeFlag)
    {
            stopCodeFlag = true;
            if (currentIndex == 4)
            {
                wrongCodeFlag = false;
            }
            ClearCode();
    }
    else if ((pressed == CLOSE_KEY) & !openCodeFlag)
    {
        closeCodeFlag = true;
        if (currentIndex == 4)
        {
            wrongCodeFlag = false;
        }
        ClearCode();
    }
}
void HandleKeyPadButtonPressed()
{
    Debounce();
    key pressed = GetKeyPressed();
    HandleKeyPadButtonValue(pressed);
    while (GetKeyPressed() != INVALID)
        ; //wait for release
    Debounce();
    PrintMessageToConsole(pressed);
}

bool GetWrongCodeFlag()
{
    return wrongCodeFlag;
}
bool GetCloseCodeFlag()
{
    return closeCodeFlag;
}
bool GetStopCodeFlag()
{
    return stopCodeFlag;
}
bool GetTriesExceededFlag()
{
    return triesExceededFlag;
}
bool GetOpenCodeFlag()
{
    return openCodeFlag;
}
int GetCurrentTriesCount()
{
    return currentTriesCount;
}
int GetCurrentCodeIndex()
{
    return currentIndex;
}
