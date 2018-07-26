#include <stm32f4xx.h>
#include "intelligentDAC.h"

uint16_t Square12bit[2];
uint32_t DualSquare12bit[2];
uint32_t TimerSetTable[2];

DAC_InitTypeDef	DAC_InitStructure;
DMA_InitTypeDef DMA_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
uint32_t Idx = 0;

// Start the intelligent DAC with given variables
void IntelligentDAC_Init(uint16_t val1, uint16_t val2, uint16_t time1, uint16_t time2){
	Square12bit[1] = val1;
	Square12bit[0] = val2;
	TimerSetTable[1] = (uint16_t)((time1*3.5)-1);
	TimerSetTable[0] = (uint16_t)(((time2-time1)*3.5)-1);
	
	// DMA1 Clock enable (DAC1 is connected to DMA1 channel 5)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	// GPIOA Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// DAC1 Periph clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	// TIM3 Periph clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	// GPIO pin initialization for DAC output
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	// TIM6 Configuration
	// Base clock 84 MHz
	// Update Event (Hz) = timer_clock / ((TIM_Prescaler + 1) * (TIM_Period + 1))
	// Set to initially 1 Hz
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 3499;
	TIM_TimeBaseStructure.TIM_Prescaler = 24000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM5, TIM_TRGOSource_Update);
	
	// DAC Channel1 configuration
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T5_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	
	//DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	
	for(Idx = 0; Idx < 2; Idx++){
		DualSquare12bit[Idx] = (Square12bit[Idx]<<16) + (Square12bit[Idx]);
	}
	
	DMA_DeInit(DMA1_Stream5);
	
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12RD;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&DualSquare12bit;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_MemoryBurst_Single;
	
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);
	
	// Enable DAC Channel
	//DAC_Cmd(DAC_Channel_1,ENABLE);
	DMA_Cmd(DMA1_Stream5, ENABLE);
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
	DAC_DMACmd(DAC_Channel_1, ENABLE);
	
	//DMA configuration for timer
	DMA_DeInit(DMA1_Stream6);
	DMA_InitStructure.DMA_Channel = DMA_Channel_6;
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&TimerSetTable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&TIM5->ARR;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_Init(DMA1_Stream6,&DMA_InitStructure);
	DMA_Cmd(DMA1_Stream6, ENABLE);
	TIM_DMACmd(TIM5, TIM_DMA_Update, ENABLE);

	TIM_Cmd(TIM5, ENABLE);
}

// Start normal DAC, only power required
void normalDAC(uint16_t power){
	// GPIOA Periph clock enable
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	// DAC1 Periph clock enable
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	// GPIO pin initialization for DAC output
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// DAC Channel1 configuration
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
	DAC_SetChannel1Data(DAC_Align_12b_R, power);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
}

// Stop DAC and prepare to any other initialization
void StopDAC(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	TIM_Cmd(TIM3, DISABLE);
	TIM_DMACmd(TIM3,TIM_DMA_Update,DISABLE);
	DMA_Cmd(DMA1_Stream5,DISABLE);
	DMA_Cmd(DMA1_Stream7,DISABLE);
	DAC_DMACmd(DAC_Channel_1, DISABLE);
	DAC_DMACmd(DAC_Channel_2, DISABLE);
	DAC_StructInit(&DAC_InitStructure);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	DAC_SetDualChannelData(DAC_Align_12b_R,0,0);
	DAC_DualSoftwareTriggerCmd(ENABLE);
}
