#ifndef __I2C_H
#define __I2C_H

#include "stm32f0xx.h"
#include "bsp_usart.h"
#include "delay.h"
#include "string.h"

#define mpuaddr 0xD0        //网上例程时使用
#define mpu_addr 0x68       //自己

#define	GYRO_ADDRESS   0x68	
#define ACCEL_ADDRESS  0x68 
#define MPU9250_AK8963_ADDR   0x0C    //MAG_ADDRESS


/* Exported types ------------------------------------------------------------*/
typedef enum
{
  I2C_OK                                          = 0,
  I2C_FAIL                                        = 1
}I2C_Status;
 
/* Exported constants --------------------------------------------------------*/
#define I2C_TIMING                                0x00210507
#define I2C_TIMEOUT                               0x1000
 

void I2C2_init(void);
void Init_MPU9250(void);
I2C_Status I2C_Read(uint8_t DevAddr, uint8_t RegAddr, uint8_t NumByteToRead, uint8_t* pBuffer);
I2C_Status I2C_Write(uint8_t DevAddr, uint8_t RegAddr, uint8_t NumByteToWrite, uint8_t* pBuffer);
//void Send_data(uint8_t MAG,uint8_t axis);
void Send_data(uint8_t MAG,uint8_t axis,int8_t* senddata);
void DATA_printf(int8_t *s,int16_t temp_data);


#endif
