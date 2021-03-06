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
#include "stm32l0xx_hal.h"
#include "stm32l0xx.h"
#include "tim.h"
#include "gpio.h"

/* Private variables ---------------------------------------------------------*/
volatile uint32_t blink_period = 500; // in milliseconds
int EXTI13Flag = 0;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

 /* Set Up Interrupt ----------------------------------------------------------*/
void interrupt_set_up(void)
{
	
//  GPIO_InitTypeDef GPIO_InitStruct;
//	
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;

//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//	
//	/* Enable clock for GPIOC */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	/* Enable clock for SYSCFG */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//	
//	
//	/* Tell system that you will use PD0 for EXTI_Line0 */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);
//	

//	SYSCFG->EXTICR[3]  &= (0010) ;
//	//1. clear bits 3:0 in the SYSCFG_EXTICR4 (Line4-15 is in EXTIR[3]) reg to amp EXTI Line to NVIC

//	EXTI->RTSR = EXTI_RTSR_TR13;
//	// 2.Set interrupt trigger to rising edge on Line 13

//	EXTI->IMR = EXTI_IMR_IM13; // 3. unmask EXTI13 line
//	NVIC_SetPriority(EXTI4_15_IRQn, 1); //4. Set Priority to 1
//	NVIC_EnableIRQ(EXTI4_15_IRQn);  // 5. Enable EXTI0_1 interrupt in NVIC (do 4 first)

	HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0x0F, 0x00);
	HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

}

// /* Set Up Interrupt Handler ----------------------------------------------------------*/
//void EXTI4_15_IRQHandler(void)
//{
//    if( (EXTI->IMR & EXTI_IMR_IM13) && (EXTI->PR & EXTI_PR_PR13))
//        {
//         EXTI13Flag = 1;
//         HAL_Delay(500);
//         while(GPIOA->IDR & GPIO_IDR_ID13){}
//         EXTI->PR |= EXTI_PR_PR13 ;
//        }
//}

void EXTI4_15_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_13)
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
	
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
//	GPIOA->BSRR = (1<<0);
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

  /* USER CODE BEGIN 2 */
	interrupt_set_up();
  /* USER CODE END 2 */

	
  while (1)
  {
//			GPIOA->BSRR	= (1<<0);
//			HAL_Delay(blink_period);
//			GPIOA->BRR = (1<<0);
//			HAL_Delay(blink_period);
			
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			HAL_Delay(blink_period);
				
  }
  /* USER CODE END 3 */

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
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
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
