#include "OLED.h"
#include "string.h"


#define Board 1







#define OLED_SCL_1    GPIO_SetBits(GPIOC,GPIO_Pin_13)	   //D0
#define OLED_SCL_0    GPIO_ResetBits(GPIOC,GPIO_Pin_13)	

#define OLED_SDA_1    GPIO_SetBits(GPIOC,GPIO_Pin_14)	   //D1
#define OLED_SDA_0    GPIO_ResetBits(GPIOC,GPIO_Pin_14)	

#define OLED_RST_1    GPIO_SetBits(GPIOC,GPIO_Pin_15)	
#define OLED_RST_0    GPIO_ResetBits(GPIOC,GPIO_Pin_15)

#define OLED_DC_1     GPIO_SetBits(GPIOF,GPIO_Pin_0)
#define OLED_DC_0     GPIO_ResetBits(GPIOF,GPIO_Pin_0)



#define Dot_Size 3                  // ??????


signed int Mid_Line;





/**********************************************************/
/******************     ASCII???     *******************/
/**********************************************************/




 const char Change_To_Bit[8]=
{
  0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01
};


void OLED_IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE); //   RCC_APB2Periph_GPIOA   GPIOA的时钟（！）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽模式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
}



const char Font_6x8[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};


/**********************************************************/
/********************     OLED??     ********************/
/**********************************************************/

#define X_WIDTH 128
#define Y_WIDTH 64

//-----------------------OLED???-----------------------//

void OLED_Write_Data(unsigned char Data)
{
  unsigned char Temp_i=8;
  OLED_DC_1;;
  OLED_SCL_0;;
  
  while(Temp_i--)
  {
    if(Data&0x80){OLED_SDA_1;}
    else{OLED_SDA_0;}
    OLED_SCL_1; 
    //asm("nop");;
    OLED_SCL_0;;    
    Data<<=1;    
  }
}

//-----------------------OLED???-----------------------//

void OLED_Write_Command(unsigned char Cmd)
{
	unsigned char Temp_i=8;
	
	OLED_DC_0;;
  OLED_SCL_0;;                   
  while(Temp_i--)
  {
    if(Cmd&0x80){OLED_SDA_1;}
    else{OLED_SDA_0;;}
    OLED_SCL_1;;
    //asm("nop");;
    OLED_SCL_0;;    
    Cmd<<=1;;   
  } 	
}





//----------------------??????----------------------//

void OLED_Set_Pos(unsigned char x,unsigned char y)
{ 
  OLED_Write_Command(0xb0+y);
  OLED_Write_Command(((x&0xf0)>>4)|0x10);
  OLED_Write_Command(x&0x0f); 
} 

//----------------------??????----------------------//

void OLED_Fill(unsigned char Bmp_data)
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		OLED_Write_Command(0xb0+y);
		OLED_Write_Command(0x01);
		OLED_Write_Command(0x10);
		for(x=0;x<X_WIDTH;x++)
			OLED_Write_Data(Bmp_data);
	}
}    

//------------------------OLED??------------------------//

void OLED_Clear(void)
// ????0
{
  OLED_Fill(0x00);
}

//-----------------------OLED???-----------------------//

void OLED_Init(void)        
{
  unsigned int Temp_Count;
    OLED_IO_Init();
    
	OLED_SCL_1;
	OLED_RST_0;
	for(Temp_Count=0;Temp_Count<50000;Temp_Count++);

	OLED_RST_1;
	//??????????????????,???RC????
  OLED_Write_Command(0xAE|0x00);			                // Display Off (0x00/0x01)
  OLED_Write_Command(0xD5);OLED_Write_Command(0x80);	// Set Clock as 100 Frames/Sec
  OLED_Write_Command(0xA8);OLED_Write_Command(0x3F);	// 1/64 Duty (0x0F~0x3F)
  OLED_Write_Command(0xD3);OLED_Write_Command(0x00);	// Shift Mapping RAM Counter (0x00~0x3F)
  OLED_Write_Command(0x40|0x00);			                // Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_Write_Command(0x8D);OLED_Write_Command(0x10|0x04);	    // Enable Embedded DC/DC Converter (0x00/0x04)
  OLED_Write_Command(0x20);OLED_Write_Command(0x02);		      // Set Page Addressing Mode (0x00/0x01/0x02)
  #ifdef Anti_OLED
    OLED_Write_Command(0xA0|0x00);                      // Set SEG/Column Mapping     0x00???? 0x01??
    OLED_Write_Command(0xC0|0x00);			                // Set COM/Row Scan Direction 0x00???? 0x08??
  #else
    OLED_Write_Command(0xA0|0x01);                      // Set SEG/Column Mapping     0x00???? 0x01??
    OLED_Write_Command(0xC0|0x08);			                // Set COM/Row Scan Direction 0x00???? 0x08??
  #endif
  OLED_Write_Command(0xDA);OLED_Write_Command(0x02|0x10);	    // Set Sequential Configuration (0x00/0x10)
  OLED_Write_Command(0x81);OLED_Write_Command(0xCF);	  // Set SEG Output Current
  OLED_Write_Command(0xD9);OLED_Write_Command(0xF1);	        // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_Write_Command(0xDB);OLED_Write_Command(0x40);			    // Set VCOM Deselect Level
  OLED_Write_Command(0xA4|0x00);		                  // Disable Entire Display On (0x00/0x01)
  OLED_Write_Command(0xA6|0x00);		                  // Disable Inverse Display On (0x00/0x01)  
  OLED_Write_Command(0xAE|0x01);		                  // Display On (0x00/0x01)
  OLED_Clear();                                       // ????
	OLED_Set_Pos(0,0); 
} 



/**********************************************************/
/********************     ????     ********************/
/**********************************************************/

//--------------------OLED??????--------------------//     

typedef unsigned char u8;

void OLED_Write_String(unsigned char x,unsigned char y,char String[])
{
  u8 Temp_Char=0,Temp_i=0,Temp_j=0;      
  while (String[Temp_j]!='\0')											 //输入的字符不为0循环
  {    
    Temp_Char =String[Temp_j]-32;										 //大小写？
    if(y>126){y=0;x++;}                              //保证总能找到位置输入？
    OLED_Set_Pos(y,x);    													 //屏幕位置
  	for(Temp_i=0;Temp_i<6;Temp_i++)     						 //循环6次
  	  OLED_Write_Data(Font_6x8[Temp_Char][Temp_i]);  
  	y+=6;
  	Temp_j++;
  }
}



//--------------------OLED?????----------------------//      

void OLED_Write_Char(unsigned char x,unsigned char y,signed char Asc) 
{
    char Temp_String[2];
	y=y*6;        
	Temp_String[0]=Asc;	
	Temp_String[1]='\0';			                  //???????
	OLED_Write_String(x,y,Temp_String);	        //????
}

//--------------------?????----------------------//

void OLED_draw(unsigned int data[])
{      
  
  unsigned char i;
  unsigned int tmp;
  unsigned char Cross;
  
  //CCD_Paint_Ratio = (ccd_max)/52 +1;
  for(Cross = 0; Cross < 8;Cross++)
  {
     OLED_Set_Pos(0,7-Cross);                            //确定位置
    for(i = 0;i <127; i++)                               //i确定列
    {
      tmp = data[i]; 
      if(tmp>>3 == Cross)																 //确定data所在的象限
      {
        OLED_Write_Data(Change_To_Bit[tmp&0x07]); 
      }

      else 
      {
        OLED_Write_Data(0x00);
      }
    }
  }
}






//void xy_draw(int a , int b, struct Snake *p_snake)
//{
//	p_snake->x[1] = p_snake->x[0];
//	p_snake->y[1] = p_snake->y[0];
//	p_snake->x[0] = a;
//	p_snake->y[0] = b;
//	OLED_Fill(0x00);                                             //在刷新之时先擦除所有，（感觉8好，后面再改）
//	int j,Cross;
////	char i = 0;
//  for(Cross = 0; Cross < 8;Cross++)
//  {
//     OLED_Set_Pos(p_snake->x[0] , 7-Cross);                          //确定位置
//      if(p_snake->y[0] >> 3 == Cross)																 //确定data所在的象限
//      {
//				j = p_snake->y[0] % 8;
////				i = Change_To_Bit[j];																 //为什么无法赋值？
//				OLED_Write_Data(Change_To_Bit[j]);                     //通过覆盖，所以当7->0时，无法覆盖7 
//      }
//  }
//}




//void xy_draw(int a , int b, struct Snake *p_snake, struct Food *p_food)
//{	
//	int j,k,Cross,n;		
//	char draw_snake[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};					//画蛇身（不让食物影响正常的蛇身）
//	char i[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};									//连着食物一起画
//	
//	OLED_Fill(0x00);                                             //在刷新之时先擦除所有，（感觉8好，后面再改）
//	
//	
//	for(n=p_snake->Long-1; n>=0; n--)																			//保存前一项的值
//	{
//		if(n!=0)
//		{	
//			p_snake->coordinate.x[n] = p_snake->coordinate.x[n-1];
//			p_snake->coordinate.y[n] = p_snake->coordinate.y[n-1];
//		}
//		else 
//		{	
//			p_snake->coordinate.x[n] = a;
//		  p_snake->coordinate.y[n] = b;
//		}
//	}

//	k = p_food->y % 8;
//	for(n=0; n<p_snake->Long; n++)																								//跨单元会出问题
//	{ 
//		for(Cross = 0; Cross < 8;Cross++)
//		{
//			if(p_food->x == p_snake->coordinate.x[n])																	//食物也在这行    不等就进不了循环 画不了蛇
//			{
//				OLED_Set_Pos(p_snake->coordinate.x[n] , 7-Cross);                          //确定位置
//				if(p_snake->coordinate.y[n] >> 3 == Cross)																   //确定data所在的象限
//				{
//					j = p_snake->coordinate.y[n] % 8;
//					if(p_snake->coordinate.x[n] == p_snake->coordinate.x[n+1] && (n+1)!=p_snake->Long)		//如果前一点和后一点在同一行上
//					{
//						draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);	
//						if(p_food->y >>3 == Cross)																				//同时在同一象限
//						{
//							i[Cross] = (draw_snake[Cross] | Change_To_Bit[k]);
//							OLED_Write_Data(i[Cross]);
//						}	else {
//							OLED_Write_Data(draw_snake[Cross]);																	//通过覆盖，所以当7->0时，无法覆盖7
//						}
//					} else if((n+1)==p_snake->Long) {
//						if(p_snake->coordinate.x[n] == p_snake->coordinate.x[n-1])
//						{
//							draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);
//							if(p_food->y >>3 == Cross)																				//同时在同一象限
//							{
//								i[Cross] = (draw_snake[Cross] | Change_To_Bit[k]);
//								OLED_Write_Data(i[Cross]);
//							}
//							else OLED_Write_Data(draw_snake[Cross]);
//						}
//						else	OLED_Write_Data(Change_To_Bit[j]);																//只需draw_snake[Cross]不变就行
//					} else	{
//						OLED_Write_Data(Change_To_Bit[j]);
//					}
//				}
//			}	
//			else 
//			{
//				OLED_Set_Pos(p_snake->coordinate.x[n] , 7-Cross);                          //确定位置
//				if(p_snake->coordinate.y[n] >> 3 == Cross)																   //确定data所在的象限
//				{
//					j = p_snake->coordinate.y[n] % 8;
//					if(p_snake->coordinate.x[n] == p_snake->coordinate.x[n+1] && (n+1)!=p_snake->Long)
//					{
//						draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);	
//						OLED_Write_Data(draw_snake[Cross]);																	//通过覆盖，所以当7->0时，无法覆盖7
//					}			
//					else if((n+1)==p_snake->Long)
//					{
//						if(p_snake->coordinate.x[n] == p_snake->coordinate.x[n-1])
//						{
//							draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);
//							OLED_Write_Data(draw_snake[Cross]);
//						}
//						else	OLED_Write_Data(Change_To_Bit[j]);																//只需draw_snake[Cross]不变就行
//					}
//					else	OLED_Write_Data(Change_To_Bit[j]);
//				}	
//				
//				OLED_Set_Pos(p_food->x , 7-Cross); 
//				if(p_food->y >>3 == Cross)
//				{
//					i[Cross] = (draw_snake[Cross] | Change_To_Bit[k]);
//					OLED_Write_Data(i[Cross]);
//				}
//			}
//		}			
//	}

#define SNAKE_POS_DRAW
#ifdef SNAKE_POS_DRAW
void xy_draw(int a , int b, struct Snake *p_snake, struct Food *p_food)								//二维数组似乎不行？
{	
	int j,Cross,n,t,compare_value;																																	
	char draw_snake[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	struct point draw_point_coordinate;
	
	memset(&draw_point_coordinate, 0, sizeof(struct point));
	
	OLED_Fill(0x00);                                             //在刷新之时先擦除所有，（感觉8好，后面再改）

	
	
	for(n=p_snake->Long-1; n>=0; n--)																			//保存前一项的值
	{
		if(n!=0){
			p_snake->coordinate.x[n] = p_snake->coordinate.x[n-1];
			p_snake->coordinate.y[n] = p_snake->coordinate.y[n-1];
		}else {	
			p_snake->coordinate.x[n] = a;
		  p_snake->coordinate.y[n] = b;
		}
	}
	// copy 一份数组
	for(n=0; n<=p_snake->Long; n++){															//把食物存在【p_snake->Long】的位置 
		if(n != p_snake->Long){
			draw_point_coordinate.x[n] = p_snake->coordinate.x[n];
			draw_point_coordinate.y[n] = p_snake->coordinate.y[n];
		}else{
			draw_point_coordinate.x[n] = p_food->x;
			draw_point_coordinate.y[n] = p_food->y;
		}
	}
	
	
	for(n=0; n<p_snake->Long; n++){																				//序给实际要画的数组 
		for(j=0; j<p_snake->Long-n; j++){
			if(draw_point_coordinate.x[j] > draw_point_coordinate.x[j+1]){
				t = draw_point_coordinate.x[j];
				draw_point_coordinate.x[j] = draw_point_coordinate.x[j+1];
				draw_point_coordinate.x[j+1] = t;
				t = draw_point_coordinate.y[j];
				draw_point_coordinate.y[j] = draw_point_coordinate.y[j+1];
				draw_point_coordinate.y[j+1] = t;
			}
		}
	}
	


	for(n=0; n<=p_snake->Long; n++){
		for(Cross = 0; Cross < 8;Cross++){
			if(draw_point_coordinate.x[n] != compare_value){
				draw_snake[Cross] = 0x00;
			}
			OLED_Set_Pos(draw_point_coordinate.x[n], 7-Cross);
			if(draw_point_coordinate.y[n] >> 3 ==Cross){	
				j = draw_point_coordinate.y[n] % 8;
				draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);
				OLED_Write_Data(draw_snake[Cross]);
			}
		}
		compare_value = draw_point_coordinate.x[n];
	}

#else
//void xy_draw(int a , int b, struct Snake *p_snake, struct Food *p_food)								//二维数组似乎不行？
//{	
//	int j,Cross,n;	
//	char draw_snake[128][8];
//	
//	OLED_Fill(0x00);                                             //在刷新之时先擦除所有，（感觉8好，后面再改）
//	
//	for(n=0; n<128; n++)																				//draw_snake_init
//	{
//		for(j=0; j<8; j++)
//		{
//			draw_snake[n][j] = 0x00;
//		}
//	}
//	
//	for(n=p_snake->Long-1; n>=0; n--)																			//保存前一项的值;同时进行排序给另外的数组？？
//	{
//		if(n!=0)
//		{	
//			p_snake->coordinate.x[n] = p_snake->coordinate.x[n-1];
//			p_snake->coordinate.y[n] = p_snake->coordinate.y[n-1];
//		}
//		else 
//		{	
//			p_snake->coordinate.x[n] = a;
//		  p_snake->coordinate.y[n] = b;
//		}
//	}
//
//	for(n=0; n<p_snake->Long; n++)
//	{
//		for(Cross=0; Cross<8; Cross++)
//		{
//			if(p_snake->coordinate.y[n] >> 3 == Cross)
//			{
//				j = p_snake->coordinate.y[n] % 8;
//				draw_snake[p_snake->coordinate.x[n]][Cross] = (Change_To_Bit[j] | draw_snake[p_snake->coordinate.x[n]][Cross]);	
//			}
//		}
//	}
//	
//	for(n=0; n<128; n++)
//	{	
//		for(Cross = 0; Cross < 8;Cross++)
//		{				
//			OLED_Set_Pos(n,7-Cross);                            //确定位置
//      OLED_Write_Data(draw_snake[n][Cross]); 
//		}
//  }



//	for(n=0; n<p_snake->Long; n++)																								//跨单元会出问题
//	{ 
//		for(Cross = 0; Cross < 8;Cross++)
//		{
//			OLED_Set_Pos(p_snake->coordinate.x[n] , 7-Cross);                          //确定位置
//			if(p_snake->coordinate.y[n] >> 3 == Cross)															   //确定data所在的象限
//			{
//				j = p_snake->coordinate.y[n] % 8;
//				if(p_snake->coordinate.x[n] == p_snake->coordinate.x[n+1] && (n+1)!=p_snake->Long)		//如果前一点和后一点在同一行上
//				{
//					draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);	
//					OLED_Write_Data(draw_snake[Cross]);																	//通过覆盖，所以当7->0时，无法覆盖7
//				} else if((n+1)==p_snake->Long) {
//					if(p_snake->coordinate.x[n] == p_snake->coordinate.x[n-1])
//					{
//						draw_snake[Cross] = (Change_To_Bit[j] | draw_snake[Cross]);
//						OLED_Write_Data(draw_snake[Cross]);
//					} else	{
//						OLED_Write_Data(Change_To_Bit[j]);
//					}
//				}
//			}
//		}			
//	}
//	for(n=0; n<8; n++)
//	{
//		i[n] = draw_snake[n];
//	}
//	
//	for(n=0; n<p_snake->Long; n++)
//	{
//		for(Cross = 0; Cross < 8;Cross++)
//		{
//			if(p_food->y >> 3 ==Cross)
//			{	
//				j = p_food->y % 8;
//				OLED_Set_Pos(p_food->x, 7-Cross);
//				if(p_food->x == p_snake->coordinate.x[n])
//				{
//					i[Cross] = (Change_To_Bit[j] | i[Cross]);
//					OLED_Write_Data(i[Cross]);
//				}
//				else OLED_Write_Data(Change_To_Bit[j]);
//			}
//		}
//	}
#endif


}


//void x2y2_draw(struct Snake *p_snake)
//{
//	int i,Cross1;
//  for(Cross1 = 0; Cross1 < 8;Cross1++)
//  {
//     OLED_Set_Pos(p_snake->coordinate.x[1] , 7-Cross1);                            //确定位置
//      if(p_snake->coordinate.y[1] >> 3 == Cross1)																  //确定data所在的象限 
//      {
//        i = p_snake->coordinate.y[1] % 8;
//        OLED_Write_Data(Change_To_Bit[i]);
//      }
//  }
//}


//void xy_clean(struct Snake *p_snake)
//	{
//	int Cross1;
//  for(Cross1 = 0; Cross1 < 8;Cross1++)
//  {
//     OLED_Set_Pos(p_snake->coordinate.x[1] , 7-Cross1);                            //确定位置
//      if(p_snake->coordinate.y[1] >> 3 == Cross1)																  //确定data所在的象限
//      {
//        OLED_Fill(0x00);
//      }
//  }
//}



