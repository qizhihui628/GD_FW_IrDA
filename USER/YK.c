#include "stm32f10x.h"
#include "YK.h"

u8  TxBuffer2[TxBufferSize2];
u8  RxBuffer2[RxBufferSize2];
__IO u8 TxCounter2 = 0x00;
__IO u8 RxCounter2 = 0x00;
u8 NumofTX = 0;
u8 NumofRX = 0;


void Serial2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_AFIO, ENABLE);

  /* Enable USARTy Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); 

	//GPIOA2 USART2_Tx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;             //推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //GPIOA3 USART2_Rx
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;       //浮动输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  //设定USART1 中断优先级
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;            //只修改波特率
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  //串口初始化
  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);
  USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
  //使能USART2
  USART_Cmd(USART2, ENABLE);
  
}


u8 Enable_TxRx(void)
{
	u32 Delay_i = 80000;
	u8 tmp_return = 0;
	 USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 	 USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	 do{
	 	
	 	}while((RxCounter2 < NumofRX) && ((--Delay_i )> 0));
	 if(RxBuffer2[0] == YK_SUCCESS)
	 	tmp_return = 0;
	 else
	 	tmp_return = 1;
	 	RxBuffer2[0] = 0x00;
	 return tmp_return;
}

u8 Send_Type(u8 DataH, u8 DataL)
{
	TxCounter2
}






