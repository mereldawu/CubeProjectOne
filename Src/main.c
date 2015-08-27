/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2015 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <time.h>
#include "rtc.h"
#include "tim.h"
#include "gpio.h"
#include "usart.h"

//#include "hd44780.h"

/* IO Definitions ----------------------------------------------------------*/
#define LD2_PIN GPIO_PIN_5
#define BTN_PIN GPIO_PIN_13
#define RED_LED_PIN GPIO_PIN_0
#define BICOLOR_RED_PIN GPIO_PIN_4
#define BICOLOR_GREEN_PIN GPIO_PIN_0

#define SYS_TIME 0x11

/* Private variables ---------------------------------------------------------*/
volatile uint32_t blink_period = 500; // in milliseconds
int timerValue = 0;
int EXTI13Flag = 0;

RTC_HandleTypeDef RtcHandle;			// RTC handler declaration

/* Buffers used for displaying Time and Date */
uint8_t aShowTime[50] = {0};
uint8_t aShowDate[50] = {0};
char buffer[50] ={0};
char buffer_msg[50] ={0};
char buffer1[50] ={0};
char buffer2[24]={0};
uint8_t buffer_int[5]={0};
uint8_t buffer_intc[5]={0};

char *endl = "\n\r";

int c;

time_t rawtime;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Interrupt_SetUp(void);

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while(1)
  {
    /* Turn LED2 on */
    HAL_GPIO_TogglePin(GPIOA, LD2_PIN);
    
    /* Delay */
    HAL_Delay(100);
  }
}

/* Set Up Interrupt ----------------------------------------------------------*/
void Interrupt_SetUp(void)
{
	
	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0x0F, 0x00);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/* Set Up Timer Interrupt Handler -----------------------------------------------------*/
void TIM6_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&htim6, TIM_FLAG_UPDATE) != RESET)      //In case other interrupts are also running
    {
        if (__HAL_TIM_GET_ITSTATUS(&htim6, TIM_IT_UPDATE) != RESET)
        {
            __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);
            
						// Timer interrupt on Bicolor LED - RED
					  HAL_GPIO_TogglePin(GPIOA, BICOLOR_RED_PIN);
						
//						HAL_UART_Transmit(&huart2, (char*)aShowTime, strlen((char*)aShowTime), 0xFFFF);
//						HAL_UART_Transmit(&huart2, (char*)endl, strlen(endl), 0xFFFF);
        }
    }
}

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(BTN_PIN);
}

// Hardware Interrupt on RED LED
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == BTN_PIN)
  {
    if (blink_period == 500)
    {
      blink_period = 250;
    }
    else
    {
      blink_period = 500;
    }
  }
	
	HAL_GPIO_TogglePin(GPIOA, RED_LED_PIN);

}

void Print_on_Watch(char* buffer)
{
	sprintf (buffer, "%s", ctime(&rawtime) );
}

void Print_on_Watch_int(uint8_t* buffer_int)
{
	sprintf ((char*) buffer_int, "%d", timerValue );
}

void Print_on_Watch_intc(uint8_t* buffer_intc)
{
	sprintf ((char*) buffer_intc, "%d", c );
}

time_t time(time_t* t) {
    time_t tmp = SYS_TIME;
    if (t) *t = tmp;
    return tmp;
}

int main(void)
{

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM6_Init();
	MX_RTC_Init();
	MX_USART2_UART_Init();
	
  /* USER CODE BEGIN 2 */
	Interrupt_SetUp();

	char *msg = "Hello Nucleo Fun!\n\r";
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);
	
		sprintf (buffer_msg, "%s", msg );
	
	msg = "Press a key.\n\r";
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);
	
//		c = getchar ();
		HAL_UART_Receive(&huart2, (char*)c, strlen((char*)c), 0xFFFF);
		HAL_UART_Transmit(&huart2, (uint8_t*)endl, strlen(endl), 0xFFFF);
		msg = "You pressed this key: ";
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);
//		HAL_UART_Transmit(&huart2, (uint8_t*)c, strlen(c), 0xFFFF);
		HAL_UART_Transmit(&huart2, (char*)c, strlen((char*)c), 0xFFFF);
		
		Print_on_Watch_intc(buffer_intc);
		
		printf ("You pressed '%c'.\r\n\r\n", c);

//	
//	char *msg_receive;
//	HAL_UART_Receive(&huart2, (uint8_t*)msg_receive, strlen(msg_receive), 0xFFFF);
//	msg = msg_receive;
//	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);
  while (1)
  {	
		HAL_UART_Receive(&huart2, (char*)c, strlen((char*)c), 0xFFFF);
		HAL_UART_Transmit(&huart2, (char*)c, strlen((char*)c), 0xFFFF);
		
		time(&rawtime);		// returns Thu Jan 1 00:00:17 1970
		Print_on_Watch(buffer);
							
		RTC_CalendarShow(aShowTime, aShowDate);
		
//		HAL_UART_Transmit(&huart2, buffer, strlen(buffer), 0xFFFF);
		
		Print_on_Watch(buffer);
		
		// Timer counter to blink Bicolor Green
		timerValue = htim6.Instance->CNT;		// Read current timer value
		
		Print_on_Watch_int(buffer_int);
		
		if (timerValue == 0 ){
			HAL_GPIO_TogglePin(GPIOA, LD2_PIN);}
		else if (timerValue >= 50000){
			HAL_GPIO_TogglePin(GPIOA, LD2_PIN);}
		}

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;		// system clock speed = 2MHz
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
