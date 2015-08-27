/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

/* Enable the adaquate RTC Clock Source */
#define RTC_CLOCK_SOURCE_LSE

/* Comment to disable displaying the time on the debugger */
#define DISPLAY_ON_DUBUGGER

#ifdef RTC_CLOCK_SOURCE_LSI
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
  #define RTC_ASYNCH_PREDIV  0x7F
  #define RTC_SYNCH_PREDIV   0x00FF
#endif
  
void Error_Handler(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
