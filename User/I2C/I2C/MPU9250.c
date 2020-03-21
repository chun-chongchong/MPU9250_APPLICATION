#include "MPU9250.h"

int16_t  MPU9250_FIFO[6][11];
uint8_t buffer[14];

struct
{
	int16_t Gx_offset;
	int16_t Gy_offset;
	int16_t Gz_offset;
	int16_t Lastax;
	int16_t Lastay;
	int16_t Lastaz;
	int16_t Lastgx;
	int16_t Lastgy;
	int16_t Lastgz;
}mpu9250_value;

/****************************************************************************************************
函 数 名：MPU9250_Read_Reg(uint8_t reg)
函数说明：读MPU寄存器值
输入形参：寄存器地址
*****************************************************************************************************/
uint8_t MPU9250_Read_Reg(uint8_t reg)
{
	uint8_t Temperature_Data_Buffer[1];
	I2C_Read(mpu_addr, reg, 1, Temperature_Data_Buffer);
	return Temperature_Data_Buffer[0];
}

/**************************实现函数********************************************
*函数原型:		uint8_t MPU9250_getDeviceID(void)
*功　　能:	    读取  MPU6050 WHO_AM_I 标识	 将返回 0x68
*******************************************************************************/
uint8_t MPU9250_getDeviceID(void) {

    I2C_Read(mpu_addr, 0x75, 1, buffer);
    return buffer[0];
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_testConnection(void)
*功　　能:	    检测MPU6050 是否已经连接
*******************************************************************************/
void MPU9250_testConnection(void) {
   if(MPU9250_getDeviceID() != 0x71){
		 Usart_SendData(USART1, (uint8_t*)"can't found Device!\n");
	 }else {
		 Usart_SendData(USART1, (uint8_t*)"Application Test Start!\n");
	 }
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setClockSource(uint8_t source)
*功　　能:	    设置  MPU9250 的时钟源
*输入参数:  source
register PWR_MGMT_1,
*[7] H_RESET       1-Reset the internal registers and restores the default settings. Write a 1 to set the reset, the bit will auto clear
*[6] SLEEP         1-The chip is set to sleep mode
*[5] CYCLE         When set, and SLEEP and STANDBY are not set, the chip will cycle between sleep and taking a single sample at a rate determined by LP_ACCEL_ODR register
							     NOTE: When all accelerometer axis are disabled via PWR_MGMT_2 register bits and cycle is enabled, the chip will wake up at the rate determined by the respective registers above, but will not take any samples
*[4] GYRO_STANDBY  1-The gyro drive and pll circuitry are enabled, but the sense paths are disabled. This a low power mode that allows quick enabling of the gyros.
*[3] PD_PTAT       Power down internal PTAT voltage generator and PTAT ADC
*[2] CLKSEL[2:0]   0 / 6-Internal 20MHz oscillator
									 1 - 5-Auto selects the best available clock source - PLL if ready, else use the internal oscillator
*******************************************************************************/
void MPU9250_setClockSource(uint8_t* source){
    I2C_Write(GYRO_ADDRESS, PWR_MGMT_1, 1, source);
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setSamplerateDivision(uint8_t rd){
*功　　能:	    设置  MPU9250 的倍频系数
*输入参数:  rd
*******************************************************************************/
void MPU9250_setSamplerateDivision(uint8_t* rd){
		I2C_Write(GYRO_ADDRESS,SMPLRT_DIV,1, rd);
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setConfig(uint8_t source)
*功　　能:	    设置  MPU9250 的配置
*[6]    FIFO_MODE 		     When set to ‘1’, when the fifo is full, additional writes will not be written to fifo.
									   	     When set to ‘0’, when the fifo is full, additional writes will be written to the fifo, replacing the oldest data.
*[5:3]  EXT_SYNC_SET[2:0]  0-disabled
													 1-TEMP_OUT_L[0]
													 2-7 -  …
*[2:0]  DLPF_CFG[2:0]      
*******************************************************************************/
void MPU9250_setConfig(uint8_t* source) {
    I2C_Write(GYRO_ADDRESS, CONFIG, 1, source);
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setFullScaleGyroRange(uint8_t range){
*功　　能:	    设置  MPU9250 陀螺仪的最大量程
*输入参数:  range
*******************************************************************************/
void MPU9250_setFullScaleGyroRange(uint8_t* range) {
    I2C_Write(GYRO_ADDRESS, GYRO_CONFIG, 1, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setFullScaleAccelRange(uint8_t range)
*功　　能:	    设置  MPU9250 加速度计的最大量程
*******************************************************************************/
void MPU9250_setFullScaleAccelRange(uint8_t* range) {
    I2C_Write(GYRO_ADDRESS, ACCEL_CONFIG, 1, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setI2CMasterModeEnabled(FunctionalState Enabled)
*功　　能:	    设置 MPU9250 是否为AUX I2C线的主机
*******************************************************************************/
void MPU9250_setI2CMasterModeEnabled(FunctionalState NewState) {
	uint8_t* Point;
	uint8_t ret;
	if(NewState == ENABLE){
		ret = MPU9250_Read_Reg(USER_CTRL);
		ret |= (ENABLE << 5);
		*Point = ret; 
		I2C_Write(mpu_addr, USER_CTRL, 1, Point);
	}else{
		ret = MPU9250_Read_Reg(USER_CTRL);
		ret &= (DISABLE << 5);
		*Point = ret; 
		I2C_Write(mpu_addr, USER_CTRL, 1, Point);
	}
}


/**************************实现函数********************************************
*函数原型:		void MPU9250_setI2CBypassEnabled(FunctionalState Enabled)
*功　　能:	    设置 MPU9250 是否为AUX I2C线的主机
*******************************************************************************/
void MPU9250_setI2CBypassEnabled(FunctionalState NewState) {
	uint8_t* Point;
	uint8_t ret;
	if(NewState == ENABLE){
		ret = MPU9250_Read_Reg(INT_PIN_CFG);
		ret |= (ENABLE << 1);
		*Point = ret;
		I2C_Write(mpu_addr, INT_PIN_CFG, 1, Point);
	}else{
		ret = MPU9250_Read_Reg(INT_PIN_CFG);
		ret &= (DISABLE << 1);
		*Point = ret;
		I2C_Write(mpu_addr, INT_PIN_CFG, 1, Point);
	}
}

/**************************实现函数********************************************
*函数原型:		void MPU9250_setInterrupt(FunctionalState NewState)
*功　　能:	   配置MPU9250 的中断模式 和中断电平模式
*******************************************************************************/
void MPU9250_setInterrupt(FunctionalState NewState) {
	uint8_t* Point;
	uint8_t ret;
	if(NewState == ENABLE){
		ret = MPU9250_Read_Reg(INT_PIN_CFG);
		ret &= 0x3f;    //[7:6]置0
		ret |= 0x20;		//0x30[5_4]置1
		*Point = ret;
		I2C_Write(mpu_addr, INT_PIN_CFG, 1, Point);
		
		ret = MPU9250_Read_Reg(INT_ENABLE);
		ret |= 0x01;
		*Point = ret;
		I2C_Write(mpu_addr, INT_ENABLE, 1, Point);
	}
}

/**************************实现函数********************************************
*函数原型:		unsigned char MPU9250_is_DRY(void)
*功　　能:	    检查 MPU6050的中断引脚，测试是否完成转换
返回 1  转换完成
0 数据寄存器还没有更新
*******************************************************************************/
unsigned char MPU9250_is_DRY(void)
{
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)==Bit_SET){
	  return 1;
	 }
	 else return 0;
}

/******************************************************************************
*函数原型: unsigned char MPU9250_RAW_DATA_RDY_INT(void)
*功能: 检查 FIFO数据 是否完成转换
返回 1  转换完成
0 数据寄存器还没有更新
*******************************************************************************/
unsigned char MPU9250_RAW_DATA_RDY_INT(void)
{
		uint8_t ret;
		ret = MPU9250_Read_Reg(INT_STATUS);
		ret &= 0x01;
    if(ret == Bit_SET){
	  return 1;
	 }
	 else return 0;
}

/******************************************************************************
*函数原型: void Init_MPU9250(void)
*功能: 初始化MPU9250
*******************************************************************************/
void Init_MPU9250(void)
{
	int16_t temp[6];
	unsigned char i;
	uint8_t Temperature_Data_Buffer[6] = {0x80, 0x07, 0x06, 0x08, 0x01, 0x00};
	
	memset(MPU9250_FIFO, 0, sizeof(MPU9250_FIFO));
	
	MPU9250_setClockSource(&Temperature_Data_Buffer[0]);
	delay_ms(10);
  MPU9250_setClockSource(&Temperature_Data_Buffer[6]);	//解除休眠状态
	MPU9250_setSamplerateDivision(&Temperature_Data_Buffer[1]);
	MPU9250_setConfig(&Temperature_Data_Buffer[2]);
	MPU9250_setFullScaleGyroRange(&Temperature_Data_Buffer[3]);
	MPU9250_setFullScaleAccelRange(&Temperature_Data_Buffer[4]);
	MPU9250_setI2CMasterModeEnabled(DISABLE);
	MPU9250_setI2CBypassEnabled(DISABLE);
	MPU9250_setInterrupt(ENABLE);
	
	for(i=0;i<10;i++){//更新FIFO数组
	delay_us(50);
	MPU9250_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
	}
																			 
	MPU9250_InitGyro_Offset();
}

/**************************实现函数********************************************
*函数原型:		void  MPU9250_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
*功　　能:	    将新的ADC数据更新到 FIFO数组，进行滤波处理
*******************************************************************************/
void  MPU9250_newValues(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz)
{
uint8_t i ;
uint32_t sum;	

for(i=1;i<10;i++){	//FIFO 操作
MPU9250_FIFO[0][i-1]=MPU9250_FIFO[0][i];
MPU9250_FIFO[1][i-1]=MPU9250_FIFO[1][i];
MPU9250_FIFO[2][i-1]=MPU9250_FIFO[2][i];
MPU9250_FIFO[3][i-1]=MPU9250_FIFO[3][i];
MPU9250_FIFO[4][i-1]=MPU9250_FIFO[4][i];
MPU9250_FIFO[5][i-1]=MPU9250_FIFO[5][i];
}
MPU9250_FIFO[0][9]=ax;//将新的数据放置到 数据的最后面
MPU9250_FIFO[1][9]=ay;
MPU9250_FIFO[2][9]=az;
MPU9250_FIFO[3][9]=gx;
MPU9250_FIFO[4][9]=gy;
MPU9250_FIFO[5][9]=gz;

sum=0;
for(i=0;i<10;i++){	//求当前数组的合，再取平均值
   sum+=MPU9250_FIFO[0][i];
}
MPU9250_FIFO[0][10]=sum/10;

sum=0;
for(i=0;i<10;i++){
   sum+=MPU9250_FIFO[1][i];
}
MPU9250_FIFO[1][10]=sum/10;

sum=0;
for(i=0;i<10;i++){
   sum+=MPU9250_FIFO[2][i];
}
MPU9250_FIFO[2][10]=sum/10;

sum=0;
for(i=0;i<10;i++){
   sum+=MPU9250_FIFO[3][i];
}
MPU9250_FIFO[3][10]=sum/10;

sum=0;
for(i=0;i<10;i++){
   sum+=MPU9250_FIFO[4][i];
}
MPU9250_FIFO[4][10]=sum/10;

sum=0;
for(i=0;i<10;i++){
   sum+=MPU9250_FIFO[5][i];
}
MPU9250_FIFO[5][10]=sum/10;
}

/*******************************************************/
void MPU9250_getMotion6_test_func(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) 
{
	memset(buffer, 0, sizeof(buffer));
	memset(MPU9250_FIFO, 0, sizeof(MPU9250_FIFO));
	
	while(!MPU9250_is_DRY());
	if(MPU9250_RAW_DATA_RDY_INT()){
	//ACCEL_XOUT_H
		I2C_Read(GYRO_ADDRESS, GYRO_XOUT_H, 14, buffer);
	  *gx=((((int16_t)buffer[0]) << 8) | buffer[1]);
		*gy=((((int16_t)buffer[2]) << 8) | buffer[3]);
		*gz=((((int16_t)buffer[4]) << 8) | buffer[5]);
		*ax=((((int16_t)buffer[8]) << 8) | buffer[9]);
		*ay=((((int16_t)buffer[10]) << 8) | buffer[11]);
		*az=((((int16_t)buffer[12]) << 8) | buffer[13]);
	}		
}


/**************************实现函数********************************************
*函数原型:		void MPU9250_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
*功　　能:	    读取 MPU9250的当前测量值
*******************************************************************************/
void MPU9250_getMotion6(int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz) {
  
	memset(buffer, 0, sizeof(buffer));
	
//	if(MPU9250_is_DRY()){
	while(!MPU9250_is_DRY());
	if(MPU9250_RAW_DATA_RDY_INT()){
	I2C_Read(GYRO_ADDRESS, GYRO_XOUT_H, 14, buffer);
    mpu9250_value.Lastgx=(((int16_t)buffer[0]) << 8) | buffer[1];
    mpu9250_value.Lastgy=(((int16_t)buffer[2]) << 8) | buffer[3];
    mpu9250_value.Lastgz=(((int16_t)buffer[4]) << 8) | buffer[5];
	//跳过温度ADC
    mpu9250_value.Lastax=(((int16_t)buffer[8]) << 8) | buffer[9];
    mpu9250_value.Lastay=(((int16_t)buffer[10]) << 8) | buffer[11];
    mpu9250_value.Lastaz=(((int16_t)buffer[12]) << 8) | buffer[13];
	MPU9250_newValues(mpu9250_value.Lastax,mpu9250_value.Lastay,mpu9250_value.Lastaz
		,mpu9250_value.Lastgx,mpu9250_value.Lastgy,mpu9250_value.Lastgz);
	*gx  =MPU9250_FIFO[0][10]-mpu9250_value.Gx_offset;
	*gy  =MPU9250_FIFO[1][10]-mpu9250_value.Gy_offset;
	*gz = MPU9250_FIFO[2][10]-mpu9250_value.Gz_offset;
	*ax  =MPU9250_FIFO[3][10];
	*ay = MPU9250_FIFO[4][10];
	*az = MPU9250_FIFO[5][10];
	} 
//	else {
//	*ax = MPU9250_FIFO[0][10];//=MPU9250_FIFO[0][10];
//	*ay = MPU9250_FIFO[1][10];//=MPU9250_FIFO[1][10];
//	*az = MPU9250_FIFO[2][10];//=MPU9250_FIFO[2][10];
//	*gx = MPU9250_FIFO[3][10]-mpu9250_value.Gx_offset;//=MPU9250_FIFO[3][10];
//	*gy = MPU9250_FIFO[4][10]-mpu9250_value.Gy_offset;//=MPU9250_FIFO[4][10];
//	*gz = MPU9250_FIFO[5][10]-mpu9250_value.Gz_offset;//=MPU9250_FIFO[5][10];
//	}
}

void MPU9250_getlastMotion6(int16_t* ax, int16_t* ay, 
		int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz)
{	
	*gx  =MPU9250_FIFO[0][10]-mpu9250_value.Gx_offset;
	*gy = MPU9250_FIFO[1][10]-mpu9250_value.Gy_offset;
	*gz = MPU9250_FIFO[2][10]-mpu9250_value.Gz_offset;
	*ax  =MPU9250_FIFO[3][10];
	*ay  =MPU9250_FIFO[4][10];
	*az = MPU9250_FIFO[5][10];
}


/**************************实现函数********************************************
*函数原型:		void MPU9250_InitGyro_Offset(void)
*功　　能:	    读取 MPU6050的陀螺仪偏置
此时模块应该被静止放置。以测试静止时的陀螺仪输出
*******************************************************************************/
void MPU9250_InitGyro_Offset(void)
{
	unsigned char i;
	int16_t temp[6];
	int32_t	tempgx=0,tempgy=0,tempgz=0;
	int32_t	tempax=0,tempay=0,tempaz=0;
	mpu9250_value.Gx_offset=0;
	mpu9250_value.Gy_offset=0;
	mpu9250_value.Gz_offset=0;
	for(i=0;i<50;i++){
  		delay_us(100);
  		MPU9250_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
	}
 	for(i=0;i<100;i++){
		delay_us(200);
		MPU9250_getMotion6(&temp[0],&temp[1],&temp[2],&temp[3],&temp[4],&temp[5]);
		tempax+= temp[0];
		tempay+= temp[1];
		tempaz+= temp[2];
		tempgx+= temp[3];
		tempgy+= temp[4];
		tempgz+= temp[5];
	}

	mpu9250_value.Gx_offset=tempgx/100;//MPU9250_FIFO[3][10];
	mpu9250_value.Gy_offset=tempgy/100;//MPU9250_FIFO[4][10];
	mpu9250_value.Gz_offset=tempgz/100;//MPU9250_FIFO[5][10];
	tempax/=100;
	tempay/=100;
	tempaz/=100;
}
