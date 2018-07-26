

#include <stm32f4xx.h>
#include "TIMInit.h"

void TIM2_INT_Init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	// Enable clock for TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// TIM2 initialization for overflow every 500ms
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event (Hz) = 84MHz / ((4199 + 1) * (9999 + 1)) = 2Hz (0.5s)
	TIM_TimeBaseInitStruct.TIM_Prescaler = 4199;
	TIM_TimeBaseInitStruct.TIM_Period = 9999;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 13;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	//TIM_Cmd(TIM2, ENABLE);
}

void DXA_Init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	//TIM1 Initialization tick freq
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event (Hz) = 168MHz / ((2485 + 1) * (79 + 1)) = 880Hz
	TIM_TimeBaseStruct.TIM_Period = 2385;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_Prescaler = 79;
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);
	
	TIM_OCInitTypeDef TIM_OCStruct;
	
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCStruct.TIM_Pulse = 1192;
	TIM_OC1Init(TIM1, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
		
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	
  TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Enable;
  TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;
  TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_1;
  TIM_BDTRInitStruct.TIM_DeadTime = 11;
  TIM_BDTRInitStruct.TIM_Break = TIM_Break_Enable;
  TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
	
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);
	TIM_CCPreloadControl(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	//TIM_Cmd(TIM1, ENABLE);
}

void Calef_Init(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM10);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	
	//TIM10 Initialization tick freq
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Update Event (Hz) = 168MHz / ((2485 + 1) * (79 + 1)) = 880Hz
	TIM_TimeBaseStruct.TIM_Period = 2399;
	TIM_TimeBaseStruct.TIM_Prescaler = 79;
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStruct);
	
	TIM_OCInitTypeDef TIM_OCStruct;
	
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCStruct.TIM_Pulse = 1;
	TIM_OC1Init(TIM10, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);
	
	TIM_CCPreloadControl(TIM10, ENABLE);
	TIM_CtrlPWMOutputs(TIM10, ENABLE);
	
	TIM_Cmd(TIM10,ENABLE);
}
