#include "bsp_i2c.h"
#include "delay.h"
 
uint32_t I2C_Timeout;
 
struct
{
	uint16_t Accel[3];
	uint16_t Gyro[3];
	uint16_t Mag[3];
}mpu_value;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
 
/*f042��PB10 PB11��I2C1,F072��PB10 PB11��I2C2*/
void I2C2_GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct; 
	
	/* Enable  GPIOA clock */
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	
 
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
  * @brief  I2C�ӿڳ�ʼ��
  *         ��ʼ��ΪI2C1������7λ��ַģʽ������ģ���˲�����ͣ�������˲�����
  *         ����Ӧ��I2C�����ٶ�100KHz
  * @param  ��
  * @retval ��
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

//void GPIO_TIM2_Init(void) 											//1usʱ��
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	
//	TIM_DeInit(TIM2);
//	
//	TIM_TimeBaseStructure.TIM_Period = 1000-1;
//	TIM_TimeBaseStructure.TIM_Prescaler = 49 - 1;
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	
//	
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);	
//	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
//	TIM_Cmd(TIM2, ENABLE);
//	
//}	

//void delay_us( uint32_t ntime)
//{
//	TIM_SetCounter(TIM2, 0);
//	while(TIM_GetCounter(TIM2) < ntime);
//}
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

 

#define MCP4017_adr  0x5e

uint8_t TX_DATA[4];
uint8_t BUF[6];
uint8_t BMP085_Temperature_Data_Buffer[5];
/****************************************************************************************************
�� �� ����read_mcp4561(void)
����˵�������Ĵ���ֵ,��һ���ֽ�
�����βΣ���
*****************************************************************************************************/

void read_mcp4017(void)
{
	uint8_t BMP085_TEMPERATURE_NUMBER = 1;
  if(I2C_Read(mpu_addr, 0, BMP085_TEMPERATURE_NUMBER, BMP085_Temperature_Data_Buffer) == I2C_FAIL)
  {
    
  }  
}

/****************************************************************************************************
�� �� ����write_mcp4561(uint8_t *write_Buffer)
����˵����д�Ĵ���ֵ
�����βΣ�Ҫд�������
*****************************************************************************************************/

void write_mcp4017(uint8_t *write_Buffer)
{
 
	if(I2C_Write(mpu_addr,0,1,write_Buffer)== I2C_FAIL )
	{
		if(I2C_Write(mpu_addr,0,1,write_Buffer)== I2C_FAIL )
		{}			
	}
}

/****************************************************************************************************
�� �� ����MPU9250_Read_Reg(uint8_t reg)
����˵������MPU�Ĵ���ֵ
�����βΣ��Ĵ�����ַ
*****************************************************************************************************/
uint8_t MPU9250_Read_Reg(uint8_t reg)
{
	uint8_t Temperature_Data_Buffer[1];
	I2C_Read(mpu_addr, reg, 1, Temperature_Data_Buffer);
	return Temperature_Data_Buffer[0];
}

/****************************************************************************************************
�� �� ����i2c_Mag_write(void)
����˵����д�����ƼĴ���ֵ
�����βΣ��Ĵ�����ַ��Ҫ�����ֵ
*****************************************************************************************************/
static void i2c_Mag_write(void)
{
 uint8_t write_Data_Buffer[1] = {0x02};
 I2C_Write(mpu_addr, MPU9250_AK8963_ADDR, 1, write_Data_Buffer);
// MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR);//���ô����Ƶ�ַ,mode: write
// delay_us(100);
// MPU9250_Write_Reg(I2C_SLV0_REG ,reg);//set reg addr
// delay_us(100);
// MPU9250_Write_Reg(I2C_SLV0_DO ,value);//send value 
// delay_us(100);//�˴���ΪMPU�ڲ�I2C��ȡ�ٶȽ�������ʱ�ȴ��ڲ�д���
}


/****************************************************************************************************
�� �� ����i2c_Mag_read(uint8_t reg)
����˵�����������ƼĴ���ֵ
�����βΣ��Ĵ�����ַ
*****************************************************************************************************/
static uint8_t i2c_Mag_read(uint8_t reg)
{
	uint8_t Temperature_Data_Buffer[5];
	i2c_Mag_write();
	I2C_Read(MPU9250_AK8963_ADDR, reg, 1, Temperature_Data_Buffer);
// MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //���ô����Ƶ�ַ��mode��read
// delay_us(100);
// MPU9250_Write_Reg(I2C_SLV0_REG ,reg);// set reg addr
// delay_us(100);
// MPU9250_Write_Reg(I2C_SLV0_DO ,0xff);//read
// delay_us(100);//�˴���ΪMPU�ڲ�I2C��ȡ�ٶȽ�����������ʱ�ȴ��ڲ���ȡ���
 return MPU9250_Read_Reg(EXT_SENS_DATA_00);
}


/****************************************************************************************************
�� �� ����READ_MPU9250_ACCEL(void)
����˵������ȡ���ٶ�
�����βΣ�none
*****************************************************************************************************/
void READ_MPU9250_ACCEL(void)//
{ 
   BUF[0]=MPU9250_Read_Reg(ACCEL_XOUT_L); 
   BUF[1]=MPU9250_Read_Reg(ACCEL_XOUT_H);
   mpu_value.Accel[0]= (BUF[1]<<8)|BUF[0];
   mpu_value.Accel[0]/=164;          //��ȡ����X������
   BUF[2]=MPU9250_Read_Reg(ACCEL_YOUT_L);
   BUF[3]=MPU9250_Read_Reg(ACCEL_YOUT_H);
   mpu_value.Accel[1]= (BUF[3]<<8)|BUF[2];
   mpu_value.Accel[1]/=164;          //��ȡ����Y������
   BUF[4]=MPU9250_Read_Reg(ACCEL_ZOUT_L); 
   BUF[5]=MPU9250_Read_Reg(ACCEL_ZOUT_H);
   mpu_value.Accel[2]=  (BUF[5]<<8)|BUF[4];
   mpu_value.Accel[2]/=164;           //��ȡ����Z������  
}

/****************************************************************************************************
�� �� ����READ_MPU9250_GYRO(void)
����˵������ȡ������
�����βΣ�none
*****************************************************************************************************/
void READ_MPU9250_GYRO(void)
{ 
   BUF[0]=MPU9250_Read_Reg(GYRO_XOUT_L); 
   BUF[1]=MPU9250_Read_Reg(GYRO_XOUT_H);
   mpu_value.Gyro[0]= (BUF[1]<<8)|BUF[0];
   mpu_value.Gyro[0]/=32.8;            //��ȡ����X������
   BUF[2]=MPU9250_Read_Reg(GYRO_YOUT_L);
   BUF[3]=MPU9250_Read_Reg(GYRO_YOUT_H);
   mpu_value.Gyro[1]= (BUF[3]<<8)|BUF[2];
   mpu_value.Gyro[1]/=32.8;            //��ȡ����Y������
   BUF[4]=MPU9250_Read_Reg(GYRO_ZOUT_L);
   BUF[5]=MPU9250_Read_Reg(GYRO_ZOUT_H);
   mpu_value.Gyro[2]= (BUF[5]<<8)|BUF[4];
   mpu_value.Gyro[2]/=32.8;             //��ȡ����Z������
 }

 
 /****************************************************************************************************
�� �� ����READ_MPU9250_MAG(void)
����˵������ȡ������
�����βΣ�none
*****************************************************************************************************/
void READ_MPU9250_MAG(void)
{  
 uint8_t x_axis,y_axis,z_axis; 
 
 x_axis=i2c_Mag_read(AK8963_ASAX);// X�������ȵ���ֵ
 y_axis=i2c_Mag_read(AK8963_ASAY);
 z_axis=i2c_Mag_read(AK8963_ASAZ);
 
 if((i2c_Mag_read(AK8963_ST1_REG)&AK8963_ST1_DOR)==0)//data ready
 {
   //��ȡ����X������
   BUF[0]=i2c_Mag_read(MAG_XOUT_L); //Low data 
   if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register & check Magnetic sensor overflow occurred 
   {
    BUF[0]=i2c_Mag_read(MAG_XOUT_L);//reload data
   } 
   BUF[1]=i2c_Mag_read(MAG_XOUT_H); //High data 
   if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
   {
    BUF[1]=i2c_Mag_read(MAG_XOUT_H);
   }
   mpu_value.Mag[0]=((BUF[1]<<8)|BUF[0])*(((x_axis-128)>>8)+1);  //�����Ⱦ��� ��ʽ��/RM-MPU-9250A-00 PDF/ 5.13 
   
  //��ȡ����Y������
   BUF[2]=i2c_Mag_read(MAG_YOUT_L); //Low data 
   if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
   {
    BUF[2]=i2c_Mag_read(MAG_YOUT_L);
   }   
   BUF[3]=i2c_Mag_read(MAG_YOUT_H); //High data 
   if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
   {
    BUF[3]=i2c_Mag_read(MAG_YOUT_H);
   }
    mpu_value.Mag[1]=((BUF[3]<<8)|BUF[2])*(((y_axis-128)>>8)+1); 
   
  //��ȡ����Z������
   BUF[4]=i2c_Mag_read(MAG_ZOUT_L); //Low data 
   if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
   {
    BUF[4]=i2c_Mag_read(MAG_ZOUT_L);
   }  
   BUF[5]=i2c_Mag_read(MAG_ZOUT_H); //High data 
   if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
   {
    BUF[5]=i2c_Mag_read(MAG_ZOUT_H);
   }
    mpu_value.Mag[2]=((BUF[5]<<8)|BUF[4])*(((z_axis-128)>>8)+1); 
 }            
}

//****************************************************************
void Init_MPU9250(void)
{
	uint8_t Temperature_Data_Buffer[5] = {0x00, 0x07, 0x06, 0x08, 0x01};
/*
   Single_Write(GYRO_ADDRESS,PWR_M, 0x80);   //
   Single_Write(GYRO_ADDRESS,SMPL, 0x07);    //
   Single_Write(GYRO_ADDRESS,DLPF, 0x1E);    //��2000��
   Single_Write(GYRO_ADDRESS,INT_C, 0x00 );  //
   Single_Write(GYRO_ADDRESS,PWR_M, 0x00);   //
*/
  I2C_Write(GYRO_ADDRESS,PWR_MGMT_1,1, &Temperature_Data_Buffer[0]);	//�������״̬
	I2C_Write(GYRO_ADDRESS,SMPLRT_DIV,1, &Temperature_Data_Buffer[1]);
	I2C_Write(GYRO_ADDRESS,CONFIG,1, &Temperature_Data_Buffer[2]);
	I2C_Write(GYRO_ADDRESS,GYRO_CONFIG,1, &Temperature_Data_Buffer[3]);
	I2C_Write(GYRO_ADDRESS,ACCEL_CONFIG,1, &Temperature_Data_Buffer[4]);
  //----------------
//	Single_Write(GYRO_ADDRESS,0x6A,0x00);//close Master Mode	

}

//************************************************
void  USART1_SendData(uint8_t SendData)
{
USART_SendData(USART1, SendData);
delay_us(100);
}

//********���ڷ�������***************************************
void Send_data(uint8_t MAG,uint8_t axis)
 {uint8_t i;
	USART1_SendData(MAG);
  USART1_SendData(axis);
  USART1_SendData(':');
  for(i=0;i<4;i++)USART1_SendData(TX_DATA[i]);
  USART1_SendData(' ');
  USART1_SendData(' ');
 }
 
//*****************************************************
void DATA_printf(uint8_t *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
    *++s =temp_data+0x30; 	
}

void TX(void)
{
	READ_MPU9250_ACCEL();  //���ٶ�
	DATA_printf(TX_DATA, mpu_value.Accel[0]);//ת��X�����ݵ�����
	Send_data('A','X');			 //����X����
	DATA_printf(TX_DATA, mpu_value.Accel[1]);//ת��Y�����ݵ�����
	Send_data('A','Y');			 //����Y����
	DATA_printf(TX_DATA, mpu_value.Accel[2]);//ת��Z�����ݵ�����
	Send_data('A','Z');			 //����Z����
	 
	READ_MPU9250_GYRO();      //����
	DATA_printf(TX_DATA, mpu_value.Gyro[0]);//ת��X�����ݵ�����
	Send_data('G','X');			 //����X����
	DATA_printf(TX_DATA, mpu_value.Gyro[1]);//ת��Y�����ݵ�����
	Send_data('G','Y');			 //����Y����
	DATA_printf(TX_DATA, mpu_value.Gyro[2]);//ת��Z�����ݵ�����
	Send_data('G','Z');			 //����Z����
	 
//	READ_MPU9250_MAG();	      //�ų�
//	DATA_printf(TX_DATA, mpu_value.Mag[0]);//ת��X�����ݵ�����
//	Send_data('M','X');			 //����X����
//	DATA_printf(TX_DATA, mpu_value.Mag[1]);//ת��Y�����ݵ�����
//	Send_data('M','Y');			 //����Y����
//	DATA_printf(TX_DATA, mpu_value.Mag[2]);//ת��Z�����ݵ�����
//	Send_data('M','Z');			 //����Z����
	/*
	DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
	Send_data('T');			 //�����¶�����
	*/
	USART1_SendData(0X0D);	 //����
	USART1_SendData(0X0A);	 //�س�
	delay_ms(250);				 //��ʱ  ������ ��
}

