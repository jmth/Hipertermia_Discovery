/**
*@file USART.c
*@author Jose Manuel Terres
*@date 7th march 2018
*@brief Use USART peripherals
*/

#include <stm32f4xx.h>
#include "USART.h"

/**
*@brief Initialize USART2 in pins PD5 and PD6
*@param baudrate
*/
void init_USART2(uint32_t baudrate){
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2,&USART_InitStruct);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2, ENABLE);
}

/**
*@brief Write a string to the selected UART peripheral
*@param USARTx where x is the number of UART peripheral
*@param s char array or string inside quotes
*/
void USART_puts(USART_TypeDef* USARTx, volatile char *s){
	while(*s){
		while(!(USARTx->SR & 0x00000040));
		USART_SendData(USARTx, *s);
		*s++;
	}
}

/**
*@brief USART2 interruption handler function used to read incoming characters
*/
void USART2_IRQHandler(void){
	if( USART_GetITStatus(USART2, USART_IT_RXNE)){
		static uint8_t cnt = 0;
		char t = USART2->DR;
		
		if((t != '\n') && (cnt < MAX_STRLEN)){
			received_string[cnt] = t;
			cnt++;
		}
		else{
			cnt = 0;
			USART_puts(USART2,received_string);
			for(uint8_t i = 0; i<MAX_STRLEN; i++){
				received_string[i] = '\0';
			}
		}
	}
}


void init_USART6(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6, &USART_InitStruct);
	
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART6, ENABLE);
}

void USART6_IRQHandler(void){
	if( USART_GetITStatus(USART6, USART_IT_RXNE)){
		static uint8_t cnt = 0;
		char t = USART6->DR;
		
		if((t != (char)0xFF) && (cnt < MAX_STRLEN)){
			received_string[cnt] = t;
			cnt++;
		}
		else{
			cnt = 0;
			//USART_puts(USART6,received_string);
			for(uint8_t i = 0; i<MAX_STRLEN; i++){
				CompleteCom[i] = received_string[i];
				received_string[i] = '\0';
			}
			usart6_flag = 1;
		}
	}
}

uint8_t usart6_flagRead(void){
	return usart6_flag;
}

void usart6_flagDn(void){
	usart6_flag = 0;
}

void read_com(char *buf){
	for(uint8_t i = 0; i<MAX_STRLEN; i++){
		buf[i] = CompleteCom[i];
	}
}
