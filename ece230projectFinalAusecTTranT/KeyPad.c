/*
 * KeyPad.c
 *
 *  Created on: Jan 31, 2024
 *      Author: ausectj
 */
#include <msp.h>
#include <stdint.h>
#include "KeyPad.h"
uint8_t SCAN_OUTPUT_SEQUENCE[8]={0b01111111,0b10111111,0b11011111
                                 ,0b11101111,0b11110111,0b11111011,0b11111101,0b11111110};
#define KEYPAD_PORT P2

uint16_t currentCode[4]={zero,zero,zero,zero};

extern void ConfigKeyPad(){

}

key GetKeyPressed(){
int currentRow;
for(currentRow=1;currentRow<=4;currentRow++){
    KEYPAD_PORT->OUT=SCAN_OUTPUT_SEQUENCE[currentRow]; //pull column down

}
}

void SaveKeyToCode(key key){
    static int currentIndex=0; //initializes only once
    currentCode[currentIndex]=key;
    currentIndex=(currentIndex+1)%4;
}

bool IsCorrectCode(){

}

bool ExceededMaxTries(){

}

void IRQ(){
    //check if button pressed
    HandleButtonPressed();
}

void HandleButtonPressed(){
    key pressed=GetKeyPressed();
    SaveKeyToCode(pressed);
    if()
}
