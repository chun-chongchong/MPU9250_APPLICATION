#include "timer.h"

void TIM2_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_DeInit(TIM2);
	
	TIM_TimeBaseStructure.TIM_Period = 1200000-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 2;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_Cmd(TIM2, ENABLE);
	
}	

struct TIM2_COUNT tim2_count;

void TIM2_SNAKE_AUTO_MOVE(void)
{
	if(tim2_count.count%10 == 0){
		if(snake.coordinate.y[0]-1 == snake.coordinate.y[1]){    //shang
			xy_draw(snake.coordinate.x[0],snake.coordinate.y[0]+1,&snake, &food);									
		}
		else if(snake.coordinate.x[0]+1 == snake.coordinate.x[1]){  //zuo
			xy_draw(snake.coordinate.x[0]-1,snake.coordinate.y[0],&snake, &food);
		}
		else if(snake.coordinate.y[0]+1 == snake.coordinate.y[1]){  //xia
			xy_draw(snake.coordinate.x[0],snake.coordinate.y[0]-1,&snake, &food);
		}
		else{ //you
			xy_draw(snake.coordinate.x[0]+1,snake.coordinate.y[0],&snake, &food);
		}		
	}
}



