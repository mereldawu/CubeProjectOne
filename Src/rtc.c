/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  *
  * This file is coded by merelda, modified from the tim.c file

*/
#include "rtc.h"

RTC_HandleTypeDef hrtc;
char buffertime[50] = {0};

/* RTC init function */
void MX_RTC_Init(void)
{
  /**Initialize RTC and set the Time and Date **/
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_PB14;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  HAL_RTC_Init(&hrtc);	// Calls HAL_RTC_MspInit
	
  if(HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    /* Error */
    Error_Handler(); 
  }
   
	time_t rawtime;
	sprintf (buffertime, "%s", ctime(&rawtime) );
	
	/* UNCOMMENT HERE TO RECONFIGURE RTC*/
  /*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
  /* Read the BackUp Register 0 Data */
//  if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR0) != 0x32F2)
//  {  
    /* Configure RTC Calendar */
    RTC_CalendarConfig();
//  }
}

time_t rtc_read(void) {
    RTC_DateTypeDef dateStruct;
    RTC_TimeTypeDef timeStruct;
    struct tm timeinfo;

    hrtc.Instance = RTC;
  
    // Read actual date and time
    // Warning: the time must be read first!
    HAL_RTC_GetTime(&hrtc, &timeStruct, FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &dateStruct, FORMAT_BIN);
    
    // Setup a tm structure based on the RTC
    timeinfo.tm_wday = dateStruct.WeekDay;
    timeinfo.tm_mon  = dateStruct.Month - 1;
    timeinfo.tm_mday = dateStruct.Date;
    timeinfo.tm_year = dateStruct.Year + 100;
    timeinfo.tm_hour = timeStruct.Hours;
    timeinfo.tm_min  = timeStruct.Minutes;
    timeinfo.tm_sec  = timeStruct.Seconds;

    // Convert to timestamp
    time_t t = mktime(&timeinfo);
		
    return t;    
}


//void rtc_write(time_t t) {
//    RTC_DateTypeDef dateStruct;
//    RTC_TimeTypeDef timeStruct;

//    hrtc.Instance = RTC;
//  
//    // Convert the time into a tm
//    struct tm *timeinfo = localtime(&t);
//    
//    // Fill RTC structures
//    dateStruct.WeekDay        = timeinfo->tm_wday;
//    dateStruct.Month          = timeinfo->tm_mon + 1;
//    dateStruct.Date           = timeinfo->tm_mday;
//    dateStruct.Year           = timeinfo->tm_year - 100;
//    timeStruct.Hours          = timeinfo->tm_hour;
//    timeStruct.Minutes        = timeinfo->tm_min;
//    timeStruct.Seconds        = timeinfo->tm_sec;
//    timeStruct.TimeFormat     = RTC_HOURFORMAT12_PM;
//    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
//    timeStruct.StoreOperation = RTC_STOREOPERATION_RESET;
//  
//    // Change the RTC current date/time
//    HAL_RTC_SetDate(&hrtc, &dateStruct, FORMAT_BIN);
//    HAL_RTC_SetTime(&hrtc, &timeStruct, FORMAT_BIN);
//}



/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_CalendarConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Monday August 27th 2014 */
  sdatestructure.Year = 0x15;
  sdatestructure.Month = RTC_MONTH_AUGUST;
  sdatestructure.Date = 0x27;
  sdatestructure.WeekDay = RTC_WEEKDAY_THURSDAY;
  
  if(HAL_RTC_SetDate(&hrtc,&sdatestructure,FORMAT_BCD) != HAL_OK)
  {
    /* Error */
    Error_Handler(); 
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 08:07 */
  stimestructure.Hours = 0x08;
  stimestructure.Minutes = 0x10;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&hrtc,&stimestructure,FORMAT_BCD) != HAL_OK)
  {
    /* Error */
    Error_Handler(); 
  }
  
  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
  HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0x32F2);  
	

}


/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, FORMAT_BIN);
  
#ifdef DISPLAY_ON_DUBUGGER
  /* Display time Format : hh:mm:ss */
  sprintf((char*)showtime,"%02d:%02d:%02d",stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char*)showdate,"%02d-%02d-%02d",sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
#endif
}



/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
