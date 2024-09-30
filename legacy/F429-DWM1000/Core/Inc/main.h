/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DW_NSS_Pin GPIO_PIN_15
#define DW_NSS_GPIO_Port GPIOA
#define DW_SCK_Pin GPIO_PIN_10
#define DW_SCK_GPIO_Port GPIOC
#define DW_MISO_Pin GPIO_PIN_11
#define DW_MISO_GPIO_Port GPIOC
#define DW_MOSI_Pin GPIO_PIN_12
#define DW_MOSI_GPIO_Port GPIOC
#define DW_RESET_Pin GPIO_PIN_0
#define DW_RESET_GPIO_Port GPIOD
#define DW_IRQn_Pin GPIO_PIN_1
#define DW_IRQn_GPIO_Port GPIOD
#define DW_IRQn_EXTI_IRQn EXTI1_IRQn
#define DW_WUP_Pin GPIO_PIN_2
#define DW_WUP_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
