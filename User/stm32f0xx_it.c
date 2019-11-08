/**
  ******************************************************************************
  * @file    Template/stm32f0xx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "main.h"
#include "keypad.h"
#include "timer.h"
/** @addtogroup STM32F0xx_DISCOVERY_Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}




#if 0
void EXTI0_1_IRQHandler(void)                        
{
	int key,a = 17;
	if(EXTI_GetITStatus(EXTI_Line0 | EXTI_Line1) != RESET)
	{                            
		key = valuekeypad();
		if(a != key)
		{	
//			key_text(key);
//			xy_Clear(&snake);
			key_diraction(key);
		}
		a = key;
	}
	EXTI_ClearITPendingBit(EXTI_Line0 | EXTI_Line1);
}	


void EXTI4_15_IRQHandler(void)                        
{
	int key,a = 17;
	if(EXTI_GetITStatus(EXTI_Line4 | EXTI_Line5) != RESET)
	{                           
		key = valuekeypad();
		if(a != key)
		{	
//			key_text(key);
//			xy_Clear(&snake);
			key_diraction(key);
		}
		a = key;
	}
	EXTI_ClearITPendingBit(EXTI_Line4 | EXTI_Line5);
}	


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_FLAG_Update) != RESET)
	{
		if(snake.coordinate.y[0]-1 == snake.coordinate.y[1])    //shang
		{	
			xy_draw(snake.coordinate.x[0],snake.coordinate.y[0]+1,&snake, &food);									
		}
		else if(snake.coordinate.x[0]+1 == snake.coordinate.x[1])  //zuo
		{	
			xy_draw(snake.coordinate.x[0]-1,snake.coordinate.y[0],&snake, &food);
		}
		else if(snake.coordinate.y[0]+1 == snake.coordinate.y[1])  //xia
		{
			xy_draw(snake.coordinate.x[0],snake.coordinate.y[0]-1,&snake, &food);
		}
		else //you
		{
			xy_draw(snake.coordinate.x[0]+1,snake.coordinate.y[0],&snake, &food);
		}
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}

#else


void TIM2_IRQHandler(void)
{
	int key,a = 17;
	if(TIM_GetITStatus(TIM2, TIM_FLAG_Update) != RESET){
		key = valuekeypad();
		if(a != key){	
			key_diraction(key);
			a = key;
			if(key != 16){
				tim2_count.count = 1;
			}
		}
		tim2_count.count++;
		TIM2_SNAKE_AUTO_MOVE();
	}
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
}

#endif

/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
