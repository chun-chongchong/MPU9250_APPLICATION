#include "main.h"
#include "stm32f0xx.h"
#include "i2c.h"
//#include "soft_i2c.h"
#include "MPU9250.h"  

static int flag=0;
static int user_addr=0;

int main()
{
	int8_t TX_DATA[4];
	int16_t ax, ay, az;	
	int16_t gx, gy, gz;
	char buf[30];
	unsigned char tmp;
////  uint8_t BMP085_write_Data_Buffer =0x20;
//	uint8_t BMP085_Temperature_Data_Buffer[5];
//	
//	I2C2_init();
//	
////  write_mcp4017(&BMP085_write_Data_Buffer);    
//  I2C_Read(mpu_addr, 0x75, 1, BMP085_Temperature_Data_Buffer);
//	I2C_Read(mpu_addr, 0, 1, BMP085_Temperature_Data_Buffer);
//	I2C_Read(0x0c, 0, 1, BMP085_Temperature_Data_Buffer);
	I2C2_init();
//	GPIO_TIM2_Init();
	USART_Config();
	Init_MPU9250();
//	MPU9250_testConnection();
	
	while(1)
	{
//		memset(TX_DATA, 0, sizeof(TX_DATA));
//	
//		MPU9250_getMotion6_test_func(&ax);
//		sprintf(buf, "ax = %d\n", ax);
//		Usart_SendData(USART1, (uint8_t *)buf);
//		delay_ms(250);
		
		MPU9250_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
		sprintf(buf, "ax = %d\n", ax);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "ay = %d\n", ay);
		Usart_SendData(USART1, (uint8_t *)buf);	
		sprintf(buf, "az = %d\n", az);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "gx = %d\n", gx);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "gy = %d\n", gy);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "gz = %d\n", gz);
		Usart_SendData(USART1, (uint8_t *)buf);
		
		delay_ms(250);
		
//		MPU9250_getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
//		DATA_printf(TX_DATA, ax);//转换X轴数据到数组
//		Send_data('A','X',TX_DATA);			 //发送X轴数
//		DATA_printf(TX_DATA, ay);//转换Y轴数据到数组
//		Send_data('A','Y',TX_DATA);			 //发送Y轴数
//		DATA_printf(TX_DATA, az);//转换Z轴数据到数组
//		Send_data('A','Z',TX_DATA);			 //发送Z轴数
//	 
//		DATA_printf(TX_DATA, gx);//转换X轴数据到数组
//		Send_data('G','X',TX_DATA);			 //发送X轴数
//		DATA_printf(TX_DATA, gy);//转换Y轴数据到数组
//		Send_data('G','Y',TX_DATA);			 //发送Y轴数
//		DATA_printf(TX_DATA, gz);//转换Z轴数据到数组
//		Send_data('G','Z',TX_DATA);			 //发送Z轴数
	
////		READ_MPU9250_MAG();	      //磁场
////		DATA_printf(TX_DATA, mpu_value.Mag[0]);//转换X轴数据到数组
////		Send_data('M','X');			 //发送X轴数
////		DATA_printf(TX_DATA, mpu_value.Mag[1]);//转换Y轴数据到数组
////		Send_data('M','Y');			 //发送Y轴数
////		DATA_printf(TX_DATA, mpu_value.Mag[2]);//转换Z轴数据到数组
////		Send_data('M','Z');			 //发送Z轴数
//		/*
//		DATA_printf(TX_DATA,T_T);//转换温度数据到数组
//		Send_data('T');			 //发送温度数据
//		*/
		Usart_SendByte(USART1,0X0D);	 //换行
		Usart_SendByte(USART1,0X0A);	 //回车
//		delay_ms(250);				 //延时  有问题 改

    if (flag) {
			if (!I2C_Read(mpu_addr, user_addr, 1, &tmp))
				flag = 0;
		}

	}
}
