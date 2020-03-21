#ifndef __BSP_I2C_H
#define __BSP_I2C_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


#define mpuaddr 0xD0        //网上例程时使用
#define mpu_addr 0x68       //自己

#define	GYRO_ADDRESS   0x68	
#define ACCEL_ADDRESS  0x68 
#define MPU9250_AK8963_ADDR   0x0C    //MAG_ADDRESS

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)

#define INT_PIN_CFG						0x37
#define INT_ENABLE						0x38
#define USER_CTRL  						0x6A
#define I2C_SLV0_ADDR         0x25
#define I2C_SLV0_REG    			0x26
#define I2C_SLV0_DO    			  0x63
#define EXT_SENS_DATA_00      0x49

#define	ACCEL_XOUT_H					0x3B
#define	ACCEL_XOUT_L					0x3C
#define	ACCEL_YOUT_H					0x3D
#define	ACCEL_YOUT_L					0x3E
#define	ACCEL_ZOUT_H					0x3F
#define	ACCEL_ZOUT_L					0x40

#define	TEMP_OUT_H						0x41
#define	TEMP_OUT_L						0x42

#define	GYRO_XOUT_H						0x43
#define	GYRO_XOUT_L						0x44	
#define	GYRO_YOUT_H						0x45
#define	GYRO_YOUT_L						0x46
#define	GYRO_ZOUT_H						0x47
#define	GYRO_ZOUT_L						0x48

		
#define MAG_XOUT_L						0x03
#define MAG_XOUT_H						0x04
#define MAG_YOUT_L						0x05
#define MAG_YOUT_H						0x06
#define MAG_ZOUT_L						0x07
#define MAG_ZOUT_H						0x08
 
#define AK8963_ASAX 				  0x10
#define AK8963_ASAY   				0x11
#define AK8963_ASAZ  		 		  0x12


#define AK8963_ST1_REG   			0x02
#define AK8963_ST1_DOR   			(i2c_Mag_read(AK8963_ST1_REG)&0x02)>>1
#define AK8963_ST2_REG   			0x09
#define AK8963_ST2_HOFL  			(i2c_Mag_read(AK8963_ST2_REG)&0x08)>>3



/* Exported types ------------------------------------------------------------*/
typedef enum
{
  I2C_OK                                          = 0,
  I2C_FAIL                                        = 1
}I2C_Status;
 
/* Exported constants --------------------------------------------------------*/
#define I2C_TIMING                                0x00210507
#define I2C_TIMEOUT                               0x1000
 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C2_init(void);
//I2C_Status I2C2_Read_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer); 
//I2C_Status I2C2_Write_NBytes(uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer); 
//void GPIO_TIM2_Init(void);
void Init_MPU9250(void);
I2C_Status I2C_Write(uint8_t DevAddr, uint8_t RegAddr, uint8_t NumByteToWrite, uint8_t* pBuffer);
I2C_Status I2C_Read(uint8_t DevAddr, uint8_t RegAddr, uint8_t NumByteToRead, uint8_t* pBuffer);
void read_mcp4017(void);
void write_mcp4017(uint8_t *write_Buffer);
void READ_MPU9250_ACCEL(void);
void READ_MPU9250_GYRO(void);
void READ_MPU9250_MAG(void);
void USART1_SendData(uint8_t SendData);
void Send_data(uint8_t MAG,uint8_t axis);
void DATA_printf(uint8_t *s,short temp_data);
void TX(void);
 
#endif
