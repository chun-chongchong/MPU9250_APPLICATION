#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f0xx.h"
#include <stdio.h>

// 串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define  DEBUG_USART_GPIO_AHBClkCmd     RCC_AHBPeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


void USART_Config(void);
void Usart_SendByte( USART_TypeDef * p_USARTx, uint8_t ch);
void Usart_SendData(USART_TypeDef * p_USARTx, uint8_t* str);
void Usart_SendString(USART_TypeDef * p_USARTx, uint16_t* str);
void Usart_SendHalfWord(USART_TypeDef * p_USARTx, uint16_t ch);

#endif

