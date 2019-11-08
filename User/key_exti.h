#ifndef __KEY_EXTI_H
#define __KEY_EXTI_H

#include "stm32f0xx.h" 
#include <stdio.h>

#define KEY_GPIO_CLK GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5
///#define KEY_EXTI_PIN EXTI_PinSource0 | EXTI_PinSource1 | EXTI_PinSource4 | EXTI_PinSource5
#define EXTI_KEY_LINE EXTI_Line0 | EXTI_Line1 | EXTI_Line4 | EXTI_Line5

void EXTI_Key_Config(void);

#endif /*__KEY_EXTI_H*/

