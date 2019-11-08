#ifndef __ADC_RANDOM_H
#define __ADC_RANDOM_H

#include "stm32f0xx_adc.h"

#define ADC_RANDOM_GPIO_CLK RCC_AHBPeriph_GPIOC
#define ADC_RANDOM_CLK RCC_APB2Periph_ADC1
#define ADC_RANDOM_PIN GPIO_Pin_1
#define ADC_RANDOM_GPIO_PORT GPIOC

#define ADC_RANDOM ADC1
#define ADC_RANDOM_CHANNEL ADC_Channel_1

void ADC_RandomConfig(void);
uint16_t Get_ADC_RandomSeek(void);

#endif
