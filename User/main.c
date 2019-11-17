/**
  ******************************************************************************
  * @file    GPIO_IOToggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    17-January-2014
  * @brief   Main program body
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
#include "main.h"

void delay_user( uint32_t count )
{
	for(;count!=0;count--);
}

int asasas(int t)
{
		t=t-4;
		return t;
}

#if 0
int main(void)
{
	int y=0;
	Led_TypeDef x;

	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	STM_EVAL_LEDInit(LED5);
	STM_EVAL_LEDInit(LED6);

	x=(Led_TypeDef)y;
	
	while (1)
	{
		delay_user(0xFFFF);
		STM_EVAL_LEDToggle(x);
		x++;
		if((int)x>=4)
			while((int)x>=4)
				x=(Led_TypeDef)asasas((int)x);
	}
	while(1);
}

#else

#include<stdlib.h>
#include<time.h>
#include "OLED.h"
#include "keypad.h"
#include "key_exti.h"
#include "timer.h"
#include "ADC_random.h"



struct Snake snake;
struct Food food;

void snake_Init(int a, int b, struct Snake *p_snake, struct Food *p_food)
{
	int z;
	p_snake->Long = 2;
	p_food->x =255;
	p_food->y =255;
	for(z=SNAKE_Max_Long-1; z>1; z--){
		p_snake->coordinate.x[z] = 255;
		p_snake->coordinate.y[z] = 255;
	}
		p_snake->coordinate.x[0] = a;
		p_snake->coordinate.y[0] = (b-1);
	xy_draw(p_snake->coordinate.x[0],p_snake->coordinate.y[0]+1,&snake, &food);
	p_food->count = 0;
	tim2_count.count = 1;
}

void snake_eat_food(struct Snake *p_snake, struct Food *p_food)
{
	if(p_food->count + p_snake->Long != p_snake->Long){
		if(snake.coordinate.y[p_snake->Long-2]-1 == snake.coordinate.y[p_snake->Long-1]){    //下（蛇尾的相对位置）
			snake.coordinate.x[p_snake->Long] = snake.coordinate.x[p_snake->Long-1];
			snake.coordinate.y[p_snake->Long] = snake.coordinate.y[p_snake->Long-1]-1;
		}
		else if(snake.coordinate.x[p_snake->Long-2]+1 == snake.coordinate.x[p_snake->Long-1]){  //右
			snake.coordinate.x[p_snake->Long] = snake.coordinate.x[p_snake->Long-1]+1;
			snake.coordinate.y[p_snake->Long] = snake.coordinate.y[p_snake->Long-1];
		}
		else if(snake.coordinate.y[p_snake->Long-2]+1 == snake.coordinate.y[p_snake->Long-1]){  //上
			snake.coordinate.x[p_snake->Long] = snake.coordinate.x[p_snake->Long-1];
			snake.coordinate.y[p_snake->Long] = snake.coordinate.y[p_snake->Long-1]+1;
		}
		else{ //左
			snake.coordinate.x[p_snake->Long] = snake.coordinate.x[p_snake->Long-1]-1;
			snake.coordinate.y[p_snake->Long] = snake.coordinate.y[p_snake->Long-1];
		}	
		p_snake->Long ++;
	}
}

void play(struct Snake *p_snake, struct Food *p_food)
{
	int n;
	p_food->need = 1;																																	//需要生成食物
	while(1){
		if(food.need == 1){
			srand(Get_ADC_RandomSeek());																							//添加随机种子 采用的ADC悬空采样			玩的时候在吃到食物的瞬间不要按任何东西！		
			p_food->x = rand()%128;																												
			p_food->y = rand()%64;
			for(n=0; n<snake.Long; n++){
			if(p_food->x == snake.coordinate.x[n] && p_food->y == snake.coordinate.y[n])	
				break;
			}
			if(n == snake.Long)
			p_food->need = 0;	
		}
		if(p_food->need == 0){																															//有食物就要显示
			xy_draw(p_snake->coordinate.x[0],p_snake->coordinate.y[0],&snake, &food);			//使蛇身重叠
			p_food->need = 2;																															// 为了不重复画
		}
		if(p_food->need == 2){
			if(p_snake->coordinate.x[0] == p_food->x && p_snake->coordinate.y[0] == p_food->y){
				p_food->count++;
				snake_eat_food(&snake, &food);
				p_food->need = 1;
			}
		}
	}	
}

void xxxx(struct Snake *p_snake)
{
	int i;
	
	p_snake->Long = 20;
	for (i = 0; i <20; i++) {
		p_snake->coordinate.x[i] = 64;
		p_snake->coordinate.y[i] = i + 1;
	}
}

int main(void)                          
{
	OLED_Init();
	snake_Init(64,17,&snake, &food);	
	ADC_RandomConfig();
	keypad_IO_Init();
	GPIO_UP();
	TIM2_Init();
	xxxx(&snake);
	play(&snake,&food);
}


#endif

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
  

}
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
