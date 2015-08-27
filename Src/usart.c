/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
  ******************************************************************************
  *
  * This file is coded by merelda, modified from the tim.c file

*/
#include "usart.h"

UART_HandleTypeDef huart2;

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONEBIT_SAMPLING_DISABLED;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart2);

}


/*----------------------------------------------------------------------------
  SendChar
  Write character to Serial Port.
 *----------------------------------------------------------------------------*/
int SendChar (int ch)  {

  while (!(USART1->ISR & USART_FLAG_TXE));
  USART1->TDR = (ch & 0x1FF);

  return (ch);
}


/*----------------------------------------------------------------------------
  GetKey
  Read character to Serial Port.
 *----------------------------------------------------------------------------*/
int GetKey (void)  {

  while (!(USART1->ISR & USART_FLAG_RXNE));

  return ((int)(USART1->RDR & 0x1FF));
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
