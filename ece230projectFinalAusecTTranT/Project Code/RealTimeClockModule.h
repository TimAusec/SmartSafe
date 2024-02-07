/*
 * RealTimeClockModule.h
 *
 *  Created on: Feb 6, 2024
 *      Author: ausectj
 */
#include "msp.h"
#ifndef REALTIMECLOCKMODULE_H_
#define REALTIMECLOCKMODULE_H_

extern void ConfigRTC();

extern void WriteDateRTC(uint16_t year, uint8_t day, uint8_t month, uint8_t hour, uint8_t minute,
                         uint8_t second, uint16_t dayOfWeek);

extern void PrintRTCMessage();

#endif /* REALTIMECLOCKMODULE_H_ */
