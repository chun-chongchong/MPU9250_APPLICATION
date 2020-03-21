#ifndef __MPU9250_H
#define __MPU9250_H

#include "i2c.h"
#include "bsp_usart.h"
#include "delay.h"
#include "string.h"

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
#define INT_STATUS						0x3A
#define USER_CTRL  						0x6A
#define I2C_SLV0_ADDR         0x25
#define I2C_SLV0_REG    			0x26
#define I2C_SLV0_DO    			  0x63
#define EXT_SENS_DATA_00      0x49

#define	GYRO_XOUT_H					0x3B			//已改 ACCEL和GYRO的数据寄存器 datasheet给反了
#define	GYRO_XOUT_L					0x3C
#define	GYRO_YOUT_H					0x3D
#define	GYRO_YOUT_L					0x3E
#define	GYRO_ZOUT_H					0x3F
#define	GYRO_ZOUT_L					0x40

#define	TEMP_OUT_H						0x41
#define	TEMP_OUT_L						0x42

#define	ACCEL_XOUT_H						0x43
#define	ACCEL_XOUT_L						0x44	
#define	ACCEL_YOUT_H						0x45
#define	ACCEL_YOUT_L						0x46
#define	ACCEL_ZOUT_H						0x47
#define	ACCEL_ZOUT_L						0x48

		
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

void MPU9250_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
void MPU9250_getlastMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz);
void MPU9250_InitGyro_Offset(void);
uint8_t MPU9250_getDeviceID(void);
void MPU9250_testConnection(void);
void MPU9250_getMotion6_test_func(int16_t *ax);
 

#endif

