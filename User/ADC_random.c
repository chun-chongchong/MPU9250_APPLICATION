#include "ADC_random.h"


uint16_t Get_ADC_RandomSeek(void);
/*初始化ADC*/
void ADC_RandomConfig(void)
{
 ADC_InitTypeDef ADC_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;

 RCC_AHBPeriphClockCmd(ADC_RANDOM_GPIO_CLK, ENABLE);
 RCC_APB2PeriphClockCmd(ADC_RANDOM_CLK, ENABLE);

 GPIO_InitStructure.GPIO_Pin = ADC_RANDOM_PIN;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
 GPIO_Init(ADC_RANDOM_GPIO_PORT, &GPIO_InitStructure);

 ADC_DeInit(ADC_RANDOM);
 ADC_StructInit(&ADC_InitStructure);

 ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
 ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
 ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
 ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
 ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
 ADC_Init(ADC_RANDOM, &ADC_InitStructure); 
 ADC_ChannelConfig(ADC_RANDOM, ADC_RANDOM_CHANNEL , ADC_SampleTime_1_5Cycles);
 ADC_GetCalibrationFactor(ADC_RANDOM);
 ADC_Cmd(ADC_RANDOM, ENABLE); 
 while(!ADC_GetFlagStatus(ADC_RANDOM, ADC_FLAG_ADRDY)); 
}

/*采集4次ADC的值，每次取采集的第四位，拼成16位作为种子*/
uint16_t Get_ADC_RandomSeek(void)
{
 uint8_t Count;
 uint16_t ADC_RandomSeek = 0;
 ADC_StartOfConversion(ADC_RANDOM);
 for(Count = 0; Count < 4; Count++){
 while(ADC_GetFlagStatus(ADC_RANDOM, ADC_FLAG_EOC) == RESET){
 ;
 }
 ADC_RandomSeek <<= 4;
 ADC_RandomSeek += ADC_GetConversionValue(ADC_RANDOM) & 0x000f;
 }
 ADC_StopOfConversion(ADC_RANDOM);
 return ADC_RandomSeek;
}

///*拿种子产生随机数*/
//uint16_t Get_ADC_Random(void)
//{
// srand(Get_ADC_RandomSeek());
// return rand();
//}


