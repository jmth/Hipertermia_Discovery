/**
*@file ds1804.c
*@author Jose Manuel Terres
*@date 13 November 2017
*@brief Actuate over ds1804 digital potentiometer
*@note Needs delay.c library and one timer
*/

#include "stm32f4xx.h" // Change this to match your device family
#include "ds1804.h" // Go here to define pins
#include "delay.h"

// Potentiometer pinout initialization
void ds1804_init(uint16_t CSpin, GPIO_TypeDef* CS_Port, uint16_t INCpin, GPIO_TypeDef* INC_Port, uint16_t UDpin, GPIO_TypeDef* UD_Port, TIM_TypeDef* tim){
	CS = CSpin;
	CS_P = CS_Port;
	INC = INCpin;
	INC_P = INC_Port;
	UD = UDpin;
	UD_P = UD_Port;
	ds1804_tim = tim;
	// Initialize GPIO Pins
	GPIO_InitTypeDef GPIO_InitStructure;
	// Get the RCC Periph identification
	// For  CS Pin
	if(CS_P==GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(CS_P==GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(CS_P==GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(CS_P==GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(CS_P==GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		
	// For INC Pin
	if(INC_P==GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(INC_P==GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(INC_P==GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(INC_P==GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(INC_P==GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	// For UD Pin
	if(UD_P==GPIOA) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	else if(UD_P==GPIOB) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	else if(UD_P==GPIOC) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	else if(UD_P==GPIOD) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	else if(UD_P==GPIOE) RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	// Set GPIO Configuration
	// For CS Pin
	GPIO_InitStructure.GPIO_Pin = CS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(CS_P,&GPIO_InitStructure);
	// For INC Pin
	GPIO_InitStructure.GPIO_Pin = INC;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(INC_P,&GPIO_InitStructure);
	// For UD Pin
	GPIO_InitStructure.GPIO_Pin = UD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(UD_P,&GPIO_InitStructure);
	
	// Set pins to disable state
	GPIO_SetBits(CS_P,CS); // Set CS high
	GPIO_SetBits(INC_P,INC); // Set INC high
	
		if(tim == TIM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	} else if(tim == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} else if (tim == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	} else if (tim == TIM4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	} else if (tim == TIM5){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	} else if (tim == TIM6){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	} else if (tim == TIM7){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	} else if (tim == TIM8){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	} else if (tim == TIM12){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	} else if (tim == TIM13){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	} else if (tim == TIM14){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	}
	
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(tim,&TIM_TimeBaseInitStruct);
	TIM_Cmd(tim,ENABLE);
	
	GPIO_ResetBits(CS_P,CS); // Set CS low
	GPIO_SetBits(UD_P,UD); // Set UD high - up
	uint8_t i;
	for(i=0;i<105;i++){
		GPIO_SetBits(INC_P,INC);
		microdelay(2,ds1804_tim);
		GPIO_ResetBits(INC_P,INC);
	}
	GPIO_SetBits(CS_P,CS);
}

// Set potentiometer to zero
void ds1804_zero(){
	// Set the trim to the down extreme position
	GPIO_ResetBits(CS_P,CS); // Set CS low
	GPIO_SetBits(UD_P,UD); // Set UD high - up
	uint8_t i;
	for(i=0;i<105;i++){
		GPIO_SetBits(INC_P,INC);
		microdelay(2,ds1804_tim);
		GPIO_ResetBits(INC_P,INC);
	}
	GPIO_SetBits(CS_P,CS);
}

// Save actual position for next power restart
void ds1804_save(){
	// Set the memory to autostart in the present position
	GPIO_SetBits(INC_P,INC);
	GPIO_ResetBits(CS_P,CS);
	microdelay(2,ds1804_tim);
	GPIO_SetBits(CS_P,CS);
}

// Set potentiometer one step up
void ds1804_up(uint8_t times){
	GPIO_SetBits(UD_P,UD);
	GPIO_ResetBits(CS_P,CS);
	uint8_t i;
	for(i=times;i>0;i--){
		GPIO_ResetBits(INC_P,INC);
		microdelay(2,ds1804_tim);
		GPIO_SetBits(INC_P,INC);
		microdelay(2,ds1804_tim);
	}
	GPIO_SetBits(CS_P,CS);
}

// Set potentiometer one step down
void ds1804_down(uint8_t times){
	GPIO_ResetBits(UD_P,UD);
	GPIO_ResetBits(CS_P,CS);
	uint8_t i;
	for(i=times;i>0;i--){
		GPIO_ResetBits(INC_P,INC);
		microdelay(2,ds1804_tim);
		GPIO_SetBits(INC_P,INC);
		microdelay(2,ds1804_tim);
	}
	GPIO_SetBits(CS_P,CS);
}
