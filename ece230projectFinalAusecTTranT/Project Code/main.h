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

extern int length(int array[]);

extern void ConfigureDevices();

#endif /* MAIN_H_ */
