/**
*@file ds1804.h
*@author Jose Manuel Terres
*@date 13 November 2017
*@brief Actuate over ds1804 digital potentiometer - defines
*@note Needs delay.c library and one timer
*/

#include "stm32f4xx.h"

static uint16_t CS;
static uint16_t INC;
static uint16_t UD;

static GPIO_TypeDef* CS_P;
static GPIO_TypeDef* INC_P;
static GPIO_TypeDef* UD_P;

static TIM_TypeDef* ds1804_tim;

void ds1804_init(uint16_t CSpin, GPIO_TypeDef* CS_Port, uint16_t INCpin, GPIO_TypeDef* INC_Port, uint16_t UDpin, GPIO_TypeDef* UD_Port, TIM_TypeDef* ds1804_timer);
void ds1804_zero(void);
void ds1804_save(void);
void ds1804_up(uint8_t times);
void ds1804_down(uint8_t times);
