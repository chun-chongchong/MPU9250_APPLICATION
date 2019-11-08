#ifndef __KEYPAD_H
#define	__KEYPAD_H

#include "stm32f0xx.h" 
#include <stdio.h>


#define KEY_GPIO_OP_CLK RCC_AHBPeriph_GPIOC
#define KEY_GPIO_IP_CLK RCC_AHBPeriph_GPIOA
#define KEY_GPIO_OP_PIN GPIO_Pin_0 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5
#define KEY_GPIO_IP_PIN GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5
#define KEY_GPIO_OP GPIOC
#define KEY_GPIO_IP GPIOA


 

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

void keypad_IO_Init(void);
int valuekeypad(void);
void key_text(int Value);
void GPIO_UP(void);
void key_up(void);
void key_left(void);
void key_down(void);
void key_right(void);
void key_diraction(int Value);

#endif 

