#include "bsp_usart.h"
#include "stm32f072b_discovery.h"


void delay( uint32_t count )
{
	for(;count!=0;count--);
}

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	DEBUG_USART_GPIO_AHBClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT, GPIO_PinSource9,GPIO_AF_1);
	GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT, GPIO_PinSource10,GPIO_AF_1);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	NVIC_Configuration();
	
	USART_ClearFlag(DEBUG_USARTx, USART_FLAG_TC);
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	USART_Cmd(DEBUG_USARTx, ENABLE);	    
}


void Usart_SendByte( USART_TypeDef * p_USARTx, uint8_t ch)
{
	USART_SendData(p_USARTx,ch);
	while (USART_GetFlagStatus(p_USARTx, USART_FLAG_TC) == RESET);	
}

void Usart_SendString(USART_TypeDef * p_USARTx, char *str)
{
	unsigned int i = 0;
	do
	{
		USART_SendData(p_USARTx,*(str+i));
		while (USART_GetFlagStatus(p_USARTx, USART_FLAG_TXE) == RESET);
		i++;
	}while(*(str + i) != '\0');	
	while (USART_GetFlagStatus(p_USARTx, USART_FLAG_TC) == RESET);
}

void Usart_SendHalfWord(USART_TypeDef * p_USARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	temp_h = (ch & 0xff00) >> 8;
	temp_l = ch & 0x00ff;
	
	USART_SendData(p_USARTx,temp_h);
	while (USART_GetFlagStatus(p_USARTx, USART_FLAG_TXE) == RESET);
	
	USART_SendData(p_USARTx,temp_l);
	while (USART_GetFlagStatus(p_USARTx, USART_FLAG_TXE) == RESET);
}

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);
	
		return (int)USART_ReceiveData(DEBUG_USARTx);
}
