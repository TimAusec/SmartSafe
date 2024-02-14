/*
 * main.h
 *
 *  Created on: Feb 4, 2024
 *      Author: ausectj
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <msp.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "LED.h"
#include "servoDriver.h"
#include "csHFXT.h"
#include "servoDriver.h"
#include "keypad.h"
#include "switch.h"
#include "enums.h"
#include "BluetoothModule.h"
#include "RealTimeClockModule.h"
#include "csLFXT_jjs.h"
#include "RealTimeClockModule.h"

extern int length(int array[]);

extern void ConfigureDevices();

extern void PrintDateTime();

extern void AppendLog();

extern void SendAppropriateBluetoothMessage();

#endif /* MAIN_H_ */
