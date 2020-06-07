/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define BAT_ADC_Pin GPIO_PIN_1
#define BAT_ADC_GPIO_Port GPIOA
#define BT_UART_TX_Pin GPIO_PIN_2
#define BT_UART_TX_GPIO_Port GPIOA
#define BT_UART_RX_Pin GPIO_PIN_3
#define BT_UART_RX_GPIO_Port GPIOA
#define DAC_VT_Pin GPIO_PIN_4
#define DAC_VT_GPIO_Port GPIOA
#define RADAR_POWER_Pin GPIO_PIN_5
#define RADAR_POWER_GPIO_Port GPIOA
#define ADC_IF_Pin GPIO_PIN_6
#define ADC_IF_GPIO_Port GPIOA
#define BT_INT_Pin GPIO_PIN_7
#define BT_INT_GPIO_Port GPIOA
#define BT_INT_EXTI_IRQn EXTI9_5_IRQn
#define BT_POWER_Pin GPIO_PIN_0
#define BT_POWER_GPIO_Port GPIOB
#define VERSION_P2_Pin GPIO_PIN_1
#define VERSION_P2_GPIO_Port GPIOB
#define DBG_TX_Pin GPIO_PIN_10
#define DBG_TX_GPIO_Port GPIOB
#define DBG_RX_Pin GPIO_PIN_11
#define DBG_RX_GPIO_Port GPIOB
#define SPI_CS_Pin GPIO_PIN_12
#define SPI_CS_GPIO_Port GPIOB
#define NB_POWER_Pin GPIO_PIN_8
#define NB_POWER_GPIO_Port GPIOA
#define NB_UART_TX_Pin GPIO_PIN_9
#define NB_UART_TX_GPIO_Port GPIOA
#define NB_UART_RX_Pin GPIO_PIN_10
#define NB_UART_RX_GPIO_Port GPIOA
#define NB_RESET_Pin GPIO_PIN_11
#define NB_RESET_GPIO_Port GPIOA
#define VERSION_P1_Pin GPIO_PIN_12
#define VERSION_P1_GPIO_Port GPIOA
#define HW_WDT_Pin GPIO_PIN_5
#define HW_WDT_GPIO_Port GPIOB
#define MAG_POWER_Pin GPIO_PIN_6
#define MAG_POWER_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_8
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_9
#define I2C_SDA_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
