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
//		DATA_printf(TX_DATA, ax);//ת��X�����ݵ�����
//		Send_data('A','X',TX_DATA);			 //����X����
//		DATA_printf(TX_DATA, ay);//ת��Y�����ݵ�����
//		Send_data('A','Y',TX_DATA);			 //����Y����
//		DATA_printf(TX_DATA, az);//ת��Z�����ݵ�����
//		Send_data('A','Z',TX_DATA);			 //����Z����
//	 
//		DATA_printf(TX_DATA, gx);//ת��X�����ݵ�����
//		Send_data('G','X',TX_DATA);			 //����X����
//		DATA_printf(TX_DATA, gy);//ת��Y�����ݵ�����
//		Send_data('G','Y',TX_DATA);			 //����Y����
//		DATA_printf(TX_DATA, gz);//ת��Z�����ݵ�����
//		Send_data('G','Z',TX_DATA);			 //����Z����
	
////		READ_MPU9250_MAG();	      //�ų�
////		DATA_printf(TX_DATA, mpu_value.Mag[0]);//ת��X�����ݵ�����
////		Send_data('M','X');			 //����X����
////		DATA_printf(TX_DATA, mpu_value.Mag[1]);//ת��Y�����ݵ�����
////		Send_data('M','Y');			 //����Y����
////		DATA_printf(TX_DATA, mpu_value.Mag[2]);//ת��Z�����ݵ�����
////		Send_data('M','Z');			 //����Z����
//		/*
//		DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
//		Send_data('T');			 //�����¶�����
//		*/
		Usart_SendByte(USART1,0X0D);	 //����
		Usart_SendByte(USART1,0X0A);	 //�س�
//		delay_ms(250);				 //��ʱ  ������ ��

    if (flag) {
			if (!I2C_Read(mpu_addr, user_addr, 1, &tmp))
				flag = 0;
		}

	}
}
