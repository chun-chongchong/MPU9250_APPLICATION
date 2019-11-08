/**
  ******************************************************************************
  * @file    stm32f072b_discovery.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    27-December-2013
  * @brief   This file contains definitions for STM32F072B DISCO's Leds, push-buttons
  *          hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F072B_DISCOVERY_H
#define __STM32F072B_DISCOVERY_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32F072B_DISCOVERY
  * @{
  */
      
/** @addtogroup STM32F072B_DISCOVERY_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32F072B_DISCOVERY_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED3 = 0,
  LED4 = 1,
  LED5 = 2,
  LED6 = 3
} Led_TypeDef;

typedef enum 
{  
  BUTTON_USER = 0,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

/**
  * @}
  */ 

/** @defgroup STM32F072B_DISCOVERY_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** @addtogroup STM32F072B_DISCOVERY_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED3_PIN                         GPIO_Pin_6
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOC

#define LED4_PIN                         GPIO_Pin_8
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOC
  
#define LED5_PIN                         GPIO_Pin_9
#define LED5_GPIO_PORT                   GPIOC
#define LED5_GPIO_CLK                    RCC_AHBPeriph_GPIOC  

#define LED6_PIN                         GPIO_Pin_7
#define LED6_GPIO_PORT                   GPIOC
#define LED6_GPIO_CLK                    RCC_AHBPeriph_GPIOC

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup STM32F072B_DISCOVERY_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void GPIOC6_INPUT(void);
void GPIOA0_INPUT(void);
void stm_eval_gpio_init(void);
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F072B_DISCOVERY_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
