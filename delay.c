/**
*@file delay.c
*@author Yohanes Erwin Setiawan
*@note Modified by Jose Manuel Terres to include some utilities
*@date 28 February 2018
*/
#include "delay.h"

static __IO uint32_t usTicks;

// SysTick_Handler function will be called every 1 us
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if(usTicksR() != 0){
		usTicksW();
	}
}


/**@brief Set SysTick timer to cause interruption every 1 us
*@warning Do not change divider value
*@note May need changes if device is not compatible or clock is set wrong
*@note Possible nonsense warning here
*/
void delayInit(){
	SysTick_Config(SystemCoreClock/1000);
}

/**
*@brief Load usTicks and wait, causing a delay
*@note Using this method will not affect other timers
*@param us Microseconds
*/
void delayUs(uint32_t us){
	usTicks = us;
	while(usTicks);
}

/**@brief Tell DelayUs to load usTicks
*@param ms Milliseconds
*/
void delayMs(uint32_t ms){
	/*while(ms--){
		delayUs(1000);
	}*/
	usTicks = ms;
	while(usTicks);
}

/**
*@brief Read the usTicks value from anywhere
*/
uint32_t usTicksR(){
	return usTicks;
}

/**
*@brief Substract 1 from usTicks from anywhere
*/
void usTicksW(){
	usTicks--;
}

/**
*@brief Use another timer to cause a delay, very precise
*@param us Microseconds
*@param TIMR Timer number
*@note TIMR has to be initialized separately
*/
void microdelay(uint16_t us, TIM_TypeDef* TIMR){
	uint16_t val = us*(SystemCoreClock/2000000);
	TIMR->CNT = 0x0000;
	while(TIMR->CNT<val);
}
