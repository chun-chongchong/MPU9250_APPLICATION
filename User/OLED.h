#ifndef __OLED_H
#define	__OLED_H

#include "stm32f0xx.h" 
#include <stdio.h>


#define SNAKE_Max_Long 50

extern struct Snake snake;
extern struct Food food;

struct Food
{
	int x;																																		//食物横坐标
	int y;																																		//食物纵坐标
	int need;																																	//判断是否要出现食物的变量 0有食物 1需要出现食物
	int count;																																//吃掉的食物个数
};

struct point
{
	int x[SNAKE_Max_Long+1];
	int y[SNAKE_Max_Long+1];
};

struct coor
{
	int x[SNAKE_Max_Long] ;									//可优化成一个结构体；
	int y[SNAKE_Max_Long] ; 
};

struct Snake
{
	struct coor coordinate;
	int Long;
};



/*
struct coordinate coo;

coo.x = 1;
coo.y = 1;

struct coordinate *pcoo;
pcoo->x = 1;
pcoo->y = 1;

（*pcoo).x = 1;
 (*pcoo).y = 1;
*/
void OLED_Init(void);
void OLED_Write_String(unsigned char x,unsigned char y,char String[]);
void OLED_Clear(void);
//void xy_draw(int a , int b, struct Snake *p_snake);
void xy_draw(int a , int b, struct Snake *p_snake, struct Food *p_food);
//void x2y2_draw(struct Snake *p_snake);
//void xy_clean(struct Snake *p_snake);
void OLED_Write_Data_S(unsigned char Data);

#endif 

