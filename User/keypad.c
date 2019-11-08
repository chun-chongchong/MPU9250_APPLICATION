#include "keypad.h"
#include "OLED.h"



void delay_ms(u16 nms)
{
 u32 temp;
 SysTick->LOAD = 9000*nms;
 SysTick->VAL=0X00;//清空计数器
 SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源
 do
 {
  temp=SysTick->CTRL;//读取当前倒计数值
 }while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达
    SysTick->CTRL=0x00; //关闭计数器
    SysTick->VAL =0X00; //清空计数器
}

void keypad_IO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(KEY_GPIO_OP_CLK, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_OP_PIN ;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(KEY_GPIO_OP, &GPIO_InitStructure);  

	RCC_AHBPeriphClockCmd(KEY_GPIO_IP_CLK, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = KEY_GPIO_IP_PIN ; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(KEY_GPIO_IP, &GPIO_InitStructure);
} 



void key_text(int Value)
{
	switch(Value)
	{
		case 0:{
							OLED_Clear();
							OLED_Write_String(0,0,"zero");
						}break;
		case 1:{
							OLED_Clear();
							OLED_Write_String(0,0,"1");
					 }break;
		case 2:{
							OLED_Clear();
							OLED_Write_String(0,0,"2");
					 }break;
		case 3:{
							OLED_Clear();
							OLED_Write_String(0,0,"3");
					 }break;				
		case 4:{
							OLED_Clear();
							OLED_Write_String(0,0,"4");
					 }break;
		case 5:{
							OLED_Clear();
							OLED_Write_String(0,0,"5");
					 }break;
		case 6:{
							OLED_Clear();
							OLED_Write_String(0,0,"6");
					 }break;
		case 7:{
							OLED_Clear();
							OLED_Write_String(0,0,"7");
					 }break;
		case 8:{
							OLED_Clear();
							OLED_Write_String(0,0,"8");
					 }break;
		case 9:{
							OLED_Clear();
							OLED_Write_String(0,0,"9");
				 	  }break;
		case 10:{
							OLED_Clear();
							OLED_Write_String(0,0,"A");
					  }break;
		case 11:{
							OLED_Clear();
							OLED_Write_String(0,0,"B");
					  }break;
		case 12:{
							OLED_Clear();
							OLED_Write_String(0,0,"C");
					  }break;
		case 13:{
							OLED_Clear();
							OLED_Write_String(0,0,"D");
					  }break;
		case 14:{
							OLED_Clear();
							OLED_Write_String(0,0,"*");
					  }break;
		case 15:{
							OLED_Clear();
							OLED_Write_String(0,0,"#");
					  }break;
		default:{
							OLED_Clear();
							OLED_Write_String(0,0,"Not pressed");
					  };
	}
}

void GPIO_UP(void)
{
	GPIO_Write(KEY_GPIO_OP, ((KEY_GPIO_OP->ODR & 0xffc6) | 0x0039));
}

int valuekeypad(void)
{
	int KeyValue = 16;
//	GPIO_Write(KEY_GPIO_OP, ((KEY_GPIO_OP->ODR & 0xffc6) | 0x0039));		//PC0~3高电平
//	if((KEY_GPIO_IP->IDR & 0x0033) == 0x0000)						//查看是否有按键按下
//	{
//		return KeyValue;
//	}
//	else
//	{
//		delay_ms(50);								//消抖
//		if((KEY_GPIO_IP->IDR & 0x0033) == 0x0000)					//查看是否是因为抖动产生的
//		{
//			return KeyValue;
//		}
//	}	
	
	GPIO_Write(KEY_GPIO_OP, ((KEY_GPIO_OP->ODR & 0xffc6) | 0x0001 ));		//PC0~3输出为0001
	switch(KEY_GPIO_IP->IDR & 0x0033)  							//对应的 PA0 - PA3 的值进行判断，输出不同的按键值
	{
		case 0x0001:KeyValue = 1;
		break;
 
		case 0x0002:KeyValue = 2;
		break;
 
		case 0x0010:KeyValue = 3;
		break;
 
		case 0x0020:KeyValue = 10;
		break;
	}
	
	GPIO_Write(KEY_GPIO_OP, ((KEY_GPIO_OP->ODR & 0xffc6) | 0x0008 ));		//PC0~3输出为0010
	switch(KEY_GPIO_IP->IDR & 0x0033)   							//对应的 PA0 - PA3 的值进行判断，输出不同的按键值
	{
		case 0x0001:KeyValue = 4;
		break;
 
		case 0x0002:KeyValue = 5;
		break;
 
		case 0x0010:KeyValue = 6;
		break;
 
		case 0x0020:KeyValue = 11;
		break;
	}
	
	GPIO_Write(KEY_GPIO_OP, ((KEY_GPIO_OP->ODR & 0xffc6) | 0x0010 ));		//PC0~3输出为0100
	switch(KEY_GPIO_IP->IDR & 0x0033)   							//对应的 PA0 - PA3 的值进行判断，输出不同的按键值
	{
		case 0x0001:KeyValue = 7;
		break;
 
		case 0x0002:KeyValue = 8;
		break;
 
		case 0x0010:KeyValue = 9;
		break;
 
		case 0x0020:KeyValue = 12;
		break;
	}
	
	GPIO_Write(KEY_GPIO_OP, ((KEY_GPIO_OP->ODR & 0xffc6) | 0x0020 ));		//PC0~3输出为1000
	switch(KEY_GPIO_IP->IDR & 0x0033)   							//对应的 PA0 - PA3 的值进行判断，输出不同的按键值
	{
		case 0x0001:KeyValue = 14;
		break;
 
		case 0x0002:KeyValue = 0;
		break;
 
		case 0x0010:KeyValue = 15;
		break;
 
		case 0x0020:KeyValue = 13;
		break;
	}
	
	GPIO_UP();
	return KeyValue;
}


void key_diraction(int Value)
{
	switch(Value)
	{
		case 6: key_up();break;
		case 8: key_left();break;
		case 9: key_down();break;
		case 12: key_right();break;	 
	}		
}

void key_up(void)
{
	if(snake.coordinate.y[0]+1 != snake.coordinate.y[1])
	{	
		xy_draw(snake.coordinate.x[0], snake.coordinate.y[0]+1, &snake, &food);
		
//		x2y2_draw(&snake);                               //TIM_GetCounter(TIM_TypeDef* TIMx)
		TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	}
	else
	{
		OLED_Write_String(64,32,"N");                     //改？
	}
}

void key_left(void)
{
	if(snake.coordinate.x[0]-1 != snake.coordinate.x[1])
	{	
		xy_draw(snake.coordinate.x[0]-1,snake.coordinate.y[0],&snake, &food);					//xy_draw(snake.Long,snake.x[0]-1,snake.y[0],&snake);
//		x2y2_draw(&snake);
		TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	}
	else
	{
		OLED_Write_String(64,32,"N"); 
	}
}

void key_down(void)
{
	if(snake.coordinate.y[0]-1 != snake.coordinate.y[1])
	{
		xy_draw(snake.coordinate.x[0],snake.coordinate.y[0]-1,&snake, &food);
//		x2y2_draw(&snake);
		TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	}
	else
	{
		OLED_Write_String(64,32,"N"); 
	}
}

void key_right(void)
{
	if(snake.coordinate.x[0]+1 != snake.coordinate.x[1])
	{

		xy_draw(snake.coordinate.x[0]+1,snake.coordinate.y[0],&snake, &food);
//		x2y2_draw(&snake);
		TIM_GenerateEvent(TIM2, TIM_EventSource_Update);
	}
	else
	{
		OLED_Write_String(64,32,"N"); 
	}
}

