#include "gpio.h"

void LED_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void Wheels_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;            		//定义结构体GPIO_InitStructure
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 	// 使能PB端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	  //PB7 PB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;     	//推挽，增大电流输出能力  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO口速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);          //GBIOB初始化  	
}

void Bule_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStrue;  
    USART_InitTypeDef USART_InitStrue;  
    NVIC_InitTypeDef NVIC_InitStrue;  
      
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIO端口使能  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//串口端口使能  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_10;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOB,&GPIO_InitStrue);  
      
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;  
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_11;  
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;  
    GPIO_Init(GPIOB,&GPIO_InitStrue);  
      
    USART_InitStrue.USART_BaudRate=9600;  
    USART_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;  
    USART_InitStrue.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;  
    USART_InitStrue.USART_Parity=USART_Parity_No;  
    USART_InitStrue.USART_StopBits=USART_StopBits_1;  
    USART_InitStrue.USART_WordLength=USART_WordLength_8b;  
      
    USART_Init(USART3,&USART_InitStrue);
      
    USART_Cmd(USART3,ENABLE);					//使能串口2  
      
    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//开启接收中断  
      
    NVIC_InitStrue.NVIC_IRQChannel=USART3_IRQn;  
    NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE;  
    NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=0;  
    NVIC_InitStrue.NVIC_IRQChannelSubPriority=1;  
    NVIC_Init(&NVIC_InitStrue);  
}
