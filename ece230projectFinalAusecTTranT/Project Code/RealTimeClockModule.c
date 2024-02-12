/*
 * RealTimeClockModule.c
 *
 *  Created on: Feb 6, 2024
 *      Author: Timothy Ausec
 */

#include "RealTimeClockModule.h"

int Day,Month,Year,Hours,Min,Sec; // Declare Calendar Variables to Read RTC

void ConfigRTC()
{
    RTC_C->CTL0 = 0xA500;

//clear this bit to enable writing into registers
//Real time clock starts operational
    RTC_C->CTL13 &= ~RTC_C_CTL13_HOLD;

//     RTC_C->CTL0 = 0xA500;
//     RTC_C->CTL0 &= ~(RTC_C_CTL0_TEVIFG);
    //Real time clock control 1, 3.
    //RTCBCD=1 binary coded decimal (BCD) code selected
    //RTCBCD=0 binary code selected
    //RTCMODE=1 to choose calendar mode
    //RTCHOLD=1 the calendar is stopped and the prescaler counters RT0PS and RT1PS are don't care.
    RTC_C->CTL13 = RTC_C_CTL13_MODE;

//set date and time
    RTC_C->YEAR = 2024;     // Year
    RTC_C->DATE = (5 << RTC_C_DATE_MON_OFS) | // Month
            (10 | RTC_C_DATE_DAY_OFS);    // Day
    RTC_C->TIM1 = (3 << RTC_C_TIM1_DOW_OFS) | // Day of weeK
            (4 << RTC_C_TIM1_HOUR_OFS);  // Hour
    RTC_C->TIM0 = (50 << RTC_C_TIM0_MIN_OFS) | // Minute
            (0 << RTC_C_TIM0_SEC_OFS);   // SecondS
    RTC_C->CTL0 &= 0x00FF;   //Lock the RTC Register after initialization

// Enable the RTC_C in the NVIC module
    NVIC->ISER[0] |= (1) << RTC_C_IRQn;
}

void WriteDateRTC(uint16_t year, uint8_t day, uint8_t month, uint8_t hour, uint8_t minute,
                  uint8_t second, uint16_t dayOfWeek)
{
    //     RTC_C->CTL0 = RTC_C_KEY;   //0xA500
    RTC_C->CTL0 = 0xA540;

    //clear this bit to enable writing into registers
    //Real time clock starts operational
    RTC_C->CTL13 &= ~RTC_C_CTL13_HOLD;
    //set date and time
    RTC_C->YEAR = year;    // Year
    RTC_C->DATE = (month << RTC_C_DATE_MON_OFS) | // Month
            (day | RTC_C_DATE_DAY_OFS);    // Day
    RTC_C->TIM1 = (dayOfWeek << RTC_C_TIM1_DOW_OFS) | // Day of weeK
            (hour << RTC_C_TIM1_HOUR_OFS);  // Hour
    RTC_C->TIM0 = (minute << RTC_C_TIM0_MIN_OFS) | // Minute
            (second << RTC_C_TIM0_SEC_OFS);   // SecondS
    RTC_C->CTL0 &= 0x00FF;   //Lock the RTC Register after initialization
}
void PrintRTCMessage()
{
    printf("\r\n  Time : %2d:%2d:%2d ", Hours, Min, Sec);
    printf("      Date : %d / %d / %d ", Month, Day, Year);
}

void GetDateTimeData(int* TimeData,int* currentIndex)
{
    Day = (RTC_C->DATE)&0x00FF;
    Month = RTC_C->DATE>>RTC_C_DATE_MON_OFS;
    Year = RTC_C->YEAR;
    Hours =  (RTC_C->TIM1>>RTC_C_TIM1_HOUR_OFS)&0x00FF;
    Min = (RTC_C->TIM0)>>RTC_C_TIM0_MIN_OFS;
    Sec = (RTC_C->TIM0) & 0x00FF;
    if(*currentIndex>200)
    {

    }
    else
    {
    *(TimeData)=Month;
    *(TimeData+1)=Day;
    *(TimeData+2)=Year;
    *(TimeData+3)=Hours;
    *(TimeData+4)=Min;
    *(TimeData+5)=Sec;
    }

    RTC_C->CTL0 = RTC_C_KEY;
    RTC_C->CTL0 &= 0x00FF;
    PrintRTCMessage();
    *(currentIndex)++;
}
