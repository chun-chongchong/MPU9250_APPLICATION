#include "i2c.h"


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
/*f042的PB10 PB11是I2C1,F072的PB10 PB11是I2C2*/
void I2C2_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct; 
	
	/* Enable  GPIOA clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	
 
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB , &GPIO_InitStruct);
	
  /*!< GPIO configuration */  
  /*!< Configure sEE_I2C pins: SCL */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(GPIOB , &GPIO_InitStruct);
  
  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB , &GPIO_InitStruct);
	
 /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig( GPIOB , GPIO_PinSource10, GPIO_AF_1); 
  /* Connect PXx to I2C_SDA*/
   GPIO_PinAFConfig( GPIOB ,GPIO_PinSource11, GPIO_AF_1); 
}
/**
  * @brief  I2C接口初始化
  *         初始化为I2C1主机，7位地址模式，启用模拟滤波器，停用数字滤波器，
  *         启用应答，I2C传输速度100KHz
  * @param  无
  * @retval 无
  */
void I2C2_Init_Config(void)
{
  I2C_InitTypeDef I2C_InitStructure;
  
 // RCC_I2CCLKConfig(RCC_I2C1CLK_SYSCLK);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x01;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_Timing = 0x30E32E44;
  I2C_Init(I2C2, &I2C_InitStructure);
  
  I2C_Cmd(I2C2, ENABLE);
}

void I2C2_init(void)
{
  I2C2_GPIO_Configuration();
	I2C2_Init_Config();
}

/*******************************************************************************
* Function Name  : I2C_Read
* Description    : Reads a block of data from the Device.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the Device.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the Device.
* Output         : None
* Return         : None
*******************************************************************************/
I2C_Status I2C_Read(uint8_t DevAddr, uint8_t RegAddr, uint8_t NumByteToRead, uint8_t* pBuffer)
{  
	uint32_t timeout = 60;

	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET)
	{
		if((timeout--) == 0)
		{
			return I2C_FAIL;
		}
	}

	I2C_TransferHandling(I2C2, (DevAddr<<1), 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);


	timeout = I2C_TIMEOUT;
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS) == RESET)
	{
		if((timeout--) == 0)
		{
			return I2C_FAIL;
		}
	}

	I2C_SendData(I2C2, RegAddr);

	timeout = I2C_TIMEOUT;	
	while (I2C_GetFlagStatus(I2C2, I2C_FLAG_TC) == RESET)
	{
		if((timeout--) == 0)
		{
			return I2C_FAIL;
		}
	}

	I2C_TransferHandling(I2C2, (DevAddr<<1), NumByteToRead,  I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
	
	while(NumByteToRead)  
	{	
		timeout = I2C_TIMEOUT;
		
		while(I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) == RESET)
		{
			if((timeout--) == 0)
			{
				return I2C_FAIL;
			}
		}

		*pBuffer = I2C_ReceiveData(I2C2);

		pBuffer++; 

		NumByteToRead--;        
	}
	
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF) == RESET)
	{
		if((timeout--) == 0)
		{
			return I2C_FAIL;
		}
	}
	return I2C_OK;
}

/*******************************************************************************
* Function Name  : I2C_Write
* Description    : Writes more than one byte to the Device with a single WRITE
*                  cycle. The number of byte can't exceed the Device page size.
* Input          : - pBuffer : pointer to the buffer containing the data to be 
*                    written to the Device.
*                  - WriteAddr : Device's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the Device.
* Output         : None
* Return         : None
*******************************************************************************/
I2C_Status I2C_Write(uint8_t DevAddr, uint8_t RegAddr, uint8_t NumByteToWrite, uint8_t* pBuffer)
{
	uint32_t timeout = I2C_TIMEOUT;
	//delay_us(10);
	while( I2C_GetFlagStatus( I2C2, I2C_FLAG_BUSY) != RESET) {
		if(( timeout-- ) == 0){
			return I2C_FAIL;
		}
	  //delay_us(10);
	}
	
	
	I2C_TransferHandling(I2C2, (DevAddr<<1), 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
	
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS) == RESET){
		if((timeout--) == 0){
			return I2C_FAIL;
		}
	}
	
	I2C_SendData(I2C2, RegAddr);
  
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_TCR) == RESET){
		if((timeout--) == 0){
			return I2C_FAIL;
		}
	}
	
	I2C_TransferHandling(I2C2, (DevAddr<<1), NumByteToWrite, I2C_AutoEnd_Mode, I2C_No_StartStop);
	
	while(NumByteToWrite--){
		timeout = I2C_TIMEOUT;
		while (I2C_GetFlagStatus(I2C2, I2C_FLAG_TXIS) == RESET){
			if((timeout--) == 0){
				return I2C_FAIL;
			}
		}
		I2C_SendData(I2C2, *pBuffer); 
	
		pBuffer++; 		
	}
	
	timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_STOPF) == RESET){
		if((timeout--) == 0){
			return I2C_FAIL;
		}
	}
	
	
	return I2C_OK;
	
}




//void Send_data(uint8_t MAG,uint8_t axis, int8_t* tx_data)
//{
//	uint8_t i;
//	Usart_SendByte(USART1,MAG);
//  Usart_SendByte(USART1,axis);
//  Usart_SendByte(USART1,':');
//  for(i=0;i<4;i++)Usart_SendByte(USART1,*tx_data);
//  Usart_SendByte(USART1,' ');
//  Usart_SendByte(USART1,' ');
//}
 



//void  USART1_SendData(uint8_t SendData)
//{
//USART_SendData(USART1, SendData);
//delay_us(100);
//}

//void Send_data(uint8_t MAG,uint8_t axis)
// {uint8_t i;
//	USART1_SendData(MAG);
//  USART1_SendData(axis);
//  USART1_SendData(':');
//  for(i=0;i<4;i++)USART1_SendData(TX_DATA[i]);
//  USART1_SendData(' ');
//  USART1_SendData(' ');
// }

void Send_data(uint8_t MAG,uint8_t axis, int8_t *senddata)
 {uint8_t i;
	Usart_SendByte(USART1, MAG);
  Usart_SendByte(USART1, axis);
  Usart_SendByte(USART1, ':');
  for(i=0;i<4;i++){
		Usart_SendByte(USART1, *senddata);
		senddata++;
	}
  Usart_SendByte(USART1, ' ');
  Usart_SendByte(USART1, ' ');
 } 

/**************************实现函数********************************************
*函数原型:		void DATA_printf(uint8_t *s,short temp_data)
*功　　能:	  将读取 MPU6050的读数对应到ASCLL码
*******************************************************************************/
void DATA_printf(int8_t *s,int16_t temp_data)
{
//	int8_t errorvalue;
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
//	if(*s < '0' || *s > '9')
//	{
//		errorvalue = *s - 0x30;
//		Send_data(0x00, 0x00);
//		Send_data(errorvalue, 0x00);
//	}
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
//		if(*s < '0' || *s > '9')
//	{
//		errorvalue = *s - 0x30;
//		Send_data(0x00, 0x00);
//		Send_data(errorvalue,0x00);
//	}
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
//		if(*s < '0' || *s > '9')
//	{
//		errorvalue = *s - 0x30;
//		Send_data(0x00, 0x00);
//		Send_data(errorvalue,0x00);
//	}
}
