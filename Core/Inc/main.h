/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32g0xx_hal.h"

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
#define UART4_485_LEFT_RX_Pin GPIO_PIN_11
#define UART4_485_LEFT_RX_GPIO_Port GPIOC
#define UART2_485_RIGHT_DIR_Pin GPIO_PIN_12
#define UART2_485_RIGHT_DIR_GPIO_Port GPIOC
#define UART4_485_LEFT_DIR_Pin GPIO_PIN_14
#define UART4_485_LEFT_DIR_GPIO_Port GPIOC
#define UART3_485_DISPLAY_DIR_Pin GPIO_PIN_3
#define UART3_485_DISPLAY_DIR_GPIO_Port GPIOC
#define PRESS_INTAKE_IN0_Pin GPIO_PIN_0
#define PRESS_INTAKE_IN0_GPIO_Port GPIOA
#define CURRENT_2_IN1_Pin GPIO_PIN_1
#define CURRENT_2_IN1_GPIO_Port GPIOA
#define CURRENT_1_IN2_Pin GPIO_PIN_2
#define CURRENT_1_IN2_GPIO_Port GPIOA
#define CURRENT_3_IN3_Pin GPIO_PIN_3
#define CURRENT_3_IN3_GPIO_Port GPIOA
#define TEMP_STOREAGE_IN5_Pin GPIO_PIN_5
#define TEMP_STOREAGE_IN5_GPIO_Port GPIOA
#define TEMP_DEF_IN6_Pin GPIO_PIN_6
#define TEMP_DEF_IN6_GPIO_Port GPIOA
#define TEMP_INTAKE_IN7_Pin GPIO_PIN_7
#define TEMP_INTAKE_IN7_GPIO_Port GPIOA
#define UART3_485_DISPLAY_TX_Pin GPIO_PIN_4
#define UART3_485_DISPLAY_TX_GPIO_Port GPIOC
#define UART3_485_DISPLAY_RX_Pin GPIO_PIN_5
#define UART3_485_DISPLAY_RX_GPIO_Port GPIOC
#define TEMP_EXHAUST_IN9_Pin GPIO_PIN_1
#define TEMP_EXHAUST_IN9_GPIO_Port GPIOB
#define TEMP_CONDEN_IN10_Pin GPIO_PIN_2
#define TEMP_CONDEN_IN10_GPIO_Port GPIOB
#define TEMP_AMBIENT_IN11_Pin GPIO_PIN_10
#define TEMP_AMBIENT_IN11_GPIO_Port GPIOB
#define TEMP_SPRAY_IN15_Pin GPIO_PIN_11
#define TEMP_SPRAY_IN15_GPIO_Port GPIOB
#define PRESS_EXHAUST_IN16_Pin GPIO_PIN_12
#define PRESS_EXHAUST_IN16_GPIO_Port GPIOB
#define RELAY_8_Pin GPIO_PIN_10
#define RELAY_8_GPIO_Port GPIOA
#define COMPRESSOR_Pin GPIO_PIN_3
#define COMPRESSOR_GPIO_Port GPIOD
#define DEFROST_Pin GPIO_PIN_4
#define DEFROST_GPIO_Port GPIOD
#define UART2_485_RIGHT_TX_Pin GPIO_PIN_5
#define UART2_485_RIGHT_TX_GPIO_Port GPIOD
#define UART2_485_RIGHT_RX_Pin GPIO_PIN_6
#define UART2_485_RIGHT_RX_GPIO_Port GPIOD
#define ALARM_Pin GPIO_PIN_3
#define ALARM_GPIO_Port GPIOB
#define EVAPORATOR_FAN_Pin GPIO_PIN_4
#define EVAPORATOR_FAN_GPIO_Port GPIOB
#define HEATING_CABLE_Pin GPIO_PIN_5
#define HEATING_CABLE_GPIO_Port GPIOB
#define RELAY_7_Pin GPIO_PIN_6
#define RELAY_7_GPIO_Port GPIOB
#define LIQUID_SUPPLY_Pin GPIO_PIN_7
#define LIQUID_SUPPLY_GPIO_Port GPIOB
#define EEP_SDA_Pin GPIO_PIN_8
#define EEP_SDA_GPIO_Port GPIOB
#define EEP_SCL_Pin GPIO_PIN_9
#define EEP_SCL_GPIO_Port GPIOB
#define UART4_485_LEFT_TX_Pin GPIO_PIN_10
#define UART4_485_LEFT_TX_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

#define BOOTLOADER_ADDR   0x08000000U
#define BOOTLOADER_SIZE   0x00005000U
#define SRAM_START_ADDR   0x20000000U
#define SRAM_SIZE         (35 * 1024)
#define OTA_REQUEST_MAGIC 0x87654321U

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
