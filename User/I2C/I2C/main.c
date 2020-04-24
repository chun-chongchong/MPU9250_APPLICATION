#include "main.h"
#include "stm32f0xx.h"
#include "i2c.h"
//#include "soft_i2c.h"
#include "MPU9250.h"  

static int flag=0;
static int user_addr=0;

int main()
{
	int16_t ax, ay, az;	
	int16_t gx, gy, gz;
	char buf[30];
	unsigned char tmp;
	
	I2C2_init();
//	GPIO_TIM2_Init();
	USART_Config();
	Init_MPU9250();
//	MPU9250_testConnection();
	
	while(1)
	{
//		MPU9250_getMotion6_test_func(&ax);
//		sprintf(buf, "ax = %d\n", ax);
//		Usart_SendData(USART1, (uint8_t *)buf);
		

		MPU9250_getMotion6_test_func(&ax,&ay,&az,&gx,&gy,&gz);
		sprintf(buf, "ax = %.3f\n", (float)ax/32768*4);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "ay = %.3f\n", (float)ay/32768*4);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "az = %.3f\n", (float)az/32768*4);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "gx = %.3f\n", (float)gx/32768*1000);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "gy = %.3f\n", (float)gy/32768*1000);
		Usart_SendData(USART1, (uint8_t *)buf);
		sprintf(buf, "gz = %.3f\n", (float)gz/32768*1000);
		Usart_SendData(USART1, (uint8_t *)buf);
		
		Usart_SendByte(USART1,0X0D);	 //换行
		Usart_SendByte(USART1,0X0A);	 //回车
		delay_ms(250);				 //延时  有问题 改

    if (flag) {
			if (!I2C_Read(mpu_addr, user_addr, 1, &tmp))
				flag = 0;
		}
		/* VERSION V1.0 */
	}
}
