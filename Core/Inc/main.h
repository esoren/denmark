/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#define BUTTON5_INPUT_Pin GPIO_PIN_13
#define BUTTON5_INPUT_GPIO_Port GPIOC
#define BUTTON4_INPUT_Pin GPIO_PIN_14
#define BUTTON4_INPUT_GPIO_Port GPIOC
#define BUTTON3_INPUT_Pin GPIO_PIN_15
#define BUTTON3_INPUT_GPIO_Port GPIOC
#define MUTE_HF_AMP_Pin GPIO_PIN_0
#define MUTE_HF_AMP_GPIO_Port GPIOC
#define STANDBY_HF_AMP_Pin GPIO_PIN_1
#define STANDBY_HF_AMP_GPIO_Port GPIOC
#define I2S_DI_Pin GPIO_PIN_2
#define I2S_DI_GPIO_Port GPIOC
#define I2S_DO_Pin GPIO_PIN_3
#define I2S_DO_GPIO_Port GPIOC
#define THERM1_ANALOG_INPUT_Pin GPIO_PIN_0
#define THERM1_ANALOG_INPUT_GPIO_Port GPIOA
#define THERM2_ANALOG_INPUT_Pin GPIO_PIN_1
#define THERM2_ANALOG_INPUT_GPIO_Port GPIOA
#define THERM3_ANALOG_INPUT_Pin GPIO_PIN_2
#define THERM3_ANALOG_INPUT_GPIO_Port GPIOA
#define STANDBY_MF_AMP_Pin GPIO_PIN_4
#define STANDBY_MF_AMP_GPIO_Port GPIOA
#define IR_SIGNAL_OUT_Pin GPIO_PIN_5
#define IR_SIGNAL_OUT_GPIO_Port GPIOA
#define IR_CARRIER_OUT_Pin GPIO_PIN_6
#define IR_CARRIER_OUT_GPIO_Port GPIOA
#define LED_MODE_4_Pin GPIO_PIN_7
#define LED_MODE_4_GPIO_Port GPIOA
#define LED_MODE_3_Pin GPIO_PIN_4
#define LED_MODE_3_GPIO_Port GPIOC
#define LED_MODE_2_Pin GPIO_PIN_5
#define LED_MODE_2_GPIO_Port GPIOC
#define LED_MODE_1_Pin GPIO_PIN_0
#define LED_MODE_1_GPIO_Port GPIOB
#define LED_STATUS_1_Pin GPIO_PIN_1
#define LED_STATUS_1_GPIO_Port GPIOB
#define LED_STATUS_2_Pin GPIO_PIN_2
#define LED_STATUS_2_GPIO_Port GPIOB
#define USER_LED_PWM_Pin GPIO_PIN_10
#define USER_LED_PWM_GPIO_Port GPIOB
#define I2S_WCLK_Pin GPIO_PIN_12
#define I2S_WCLK_GPIO_Port GPIOB
#define I2S_BCLK_Pin GPIO_PIN_13
#define I2S_BCLK_GPIO_Port GPIOB
#define LED_FAULT_6_Pin GPIO_PIN_14
#define LED_FAULT_6_GPIO_Port GPIOB
#define LED_MONITOR_4_Pin GPIO_PIN_15
#define LED_MONITOR_4_GPIO_Port GPIOB
#define I2S_MCLK_Pin GPIO_PIN_6
#define I2S_MCLK_GPIO_Port GPIOC
#define LED_FAULT_5_Pin GPIO_PIN_7
#define LED_FAULT_5_GPIO_Port GPIOC
#define LED_FAULT_4_Pin GPIO_PIN_8
#define LED_FAULT_4_GPIO_Port GPIOC
#define LED_FAULT_3_Pin GPIO_PIN_9
#define LED_FAULT_3_GPIO_Port GPIOC
#define FAN_PWM_Pin GPIO_PIN_8
#define FAN_PWM_GPIO_Port GPIOA
#define LED_FAULT_2_Pin GPIO_PIN_9
#define LED_FAULT_2_GPIO_Port GPIOA
#define LED_FAULT_1_Pin GPIO_PIN_10
#define LED_FAULT_1_GPIO_Port GPIOA
#define LED_MONITOR_3_Pin GPIO_PIN_11
#define LED_MONITOR_3_GPIO_Port GPIOA
#define LED_MONITOR_2_Pin GPIO_PIN_12
#define LED_MONITOR_2_GPIO_Port GPIOA
#define LED_MONITOR_1_Pin GPIO_PIN_10
#define LED_MONITOR_1_GPIO_Port GPIOC
#define STANDBY_LF_AMP_Pin GPIO_PIN_11
#define STANDBY_LF_AMP_GPIO_Port GPIOC
#define SOFTCLIP_LF_AMP_Pin GPIO_PIN_12
#define SOFTCLIP_LF_AMP_GPIO_Port GPIOC
#define MUTE_MF_AMP_Pin GPIO_PIN_2
#define MUTE_MF_AMP_GPIO_Port GPIOD
#define BUTTON1_INPUT_Pin GPIO_PIN_6
#define BUTTON1_INPUT_GPIO_Port GPIOB
#define BUTTON2_INPUT_Pin GPIO_PIN_7
#define BUTTON2_INPUT_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_8
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_9
#define I2C_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
