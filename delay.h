/**
*@file delay.h
*@author Yohanes Erwin Setiawan
*@note Modified by Jose Manuel Terres to include some utilities
*@date 28th February 2018
*/

#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f4xx.h"
void delayInit(void);
void delayUs(uint32_t us);
void delayMs(uint32_t ms);
uint32_t usTicksR(void);
void usTicksW(void);
void microdelay(uint16_t us, TIM_TypeDef* TIMR);

#ifdef __cplusplus
}
#endif

#endif
