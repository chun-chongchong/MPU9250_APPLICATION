#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f0xx.h" 
#include "oled.h"

extern struct TIM2_COUNT tim2_count;

struct TIM2_COUNT
{
	int count;
};

void TIM2_Init(void);
void TIM2_SNAKE_AUTO_MOVE(void);

#endif
