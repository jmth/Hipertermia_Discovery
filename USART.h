/**
*@file USART.h
*@author Jose Manuel Terres
*@date 7th march 2018
*@brief Use USART peripherals
*/

#include <stm32f4xx.h>

#define MAX_STRLEN 12

static volatile char received_string[MAX_STRLEN+1];
static char CompleteCom[MAX_STRLEN+1];
static uint8_t usart6_flag = 0;

void init_USART2(uint32_t baudrate);
void USART_puts(USART_TypeDef* USARTx, volatile char *s);
void init_USART6(void);
uint8_t usart6_flagRead(void);
void usart6_flagDn(void);
void read_com(char *buf);
