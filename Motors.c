
#include "stm32f4xx.h"
#include <stdio.h>
#include "Motors.h"
#include "delay.h"
#include "Variables.h"
#include "USART.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
uint16_t zeroX = 50, zeroY = 50;
#define dist  1800
uint32_t positionsX[13] = {0, dist*1, dist*2, dist*3, dist*4, dist*5, dist*6, dist*7, dist*8, dist*9, dist*10, dist*11, dist*11};
uint32_t positionsY[9] = {0, dist*1, dist*2, dist*3, dist*4, dist*5, dist*6, dist*7};
//uint16_t xMotorArray[4] = {0x0400,0x0800,0x1000,0x2000}; // PORTB 13,12,11,10 0xC3FF to clear
uint16_t xMotorArray[4] = {0x0C00,0x1800,0x3000,0x2400}; // PORTB 13,12,11,10 0xC3FF to clear
//uint16_t yMotorArray[4] = {0x0004,0x0008,0x0010,0x0020}; // PORTD 5,4,3,2 0xFFC3 to clear
uint16_t yMotorArray[4] = {0x000C,0x0018,0x0030,0x0024}; // PORTD 5,4,3,2 0xFFC3 to clear

int8_t xi = 0, yi = 0;
uint8_t speed = 5;
uint8_t zero = 0;
uint32_t posActualX = 0, posActualY = 0;
uint32_t posObjetivoX = 0, posObjetivoY = 0;

// Initialize motors pinout
void initMotors(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// Get motors to zero position, wait for optocouplers to change states
void zeroMotors(void){
	while((!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))||(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1))){
		if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0)){
			GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
			GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
			xi--;
			if(xi<0) xi = 3;
			delayMs(speed);
		}
		if(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)){
			GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
			GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
			yi++;
			if(yi>3) yi = 0;
			delayMs(speed);
		}
	}
	GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
	GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
	posActualX = 0; posActualY = 0;
}

int32_t getAbsX(void){
	posObjetivoX = positionsX[readMotorNewPositionX()]+zeroX;
	if(posActualX>0) return posObjetivoX-posActualX;
	else return posObjetivoX-posActualX;
}
int32_t getAbsY(void){
	posObjetivoY = positionsY[readMotorNewPositionY()]+zeroY;
	if(posActualY>0) return posObjetivoY-posActualY;
	else return posObjetivoY-posActualY;
}

// Move the motors to the requested position
void moveMotors(int32_t absX, int32_t absY){
	// In case the 2 values are positive
	if((absX!=0)||(absY!=0)){
	if((absX>=0)&&(absY>=0)){
		for(uint32_t i = 0; i<=MIN(absX,absY); i++){
			GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
			GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
			xi++;
			if(xi>3) xi = 0;
			GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
			GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
			yi++;
			if(yi>3) yi = 0;
			delayMs(speed);
		}
		if(absX>absY){
				for(uint32_t i = 0; i<=(MAX(absX,absY)-MIN(absX,absY)); i++){
				GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
				GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
				xi++;
				if(xi>3) xi = 0;
				delayMs(speed);
			}
		}
		if(absY>absX){
			for(uint32_t i = 0; i<=(MAX(absX,absY)-MIN(absX,absY)); i++){
				GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
				GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
				yi++;
				if(yi>3) yi = 0;
				delayMs(speed);
			}
		}
	}
	// In case absX is negative and absY positive
	else if((absX<=0)&&(absY>=0)){
		for(uint32_t i = 0; i<=MIN(-absX,absY); i++){
			GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
			GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
			xi--;
			if(xi<0) xi = 3;
			GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
			GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
			yi++;
			if(yi>3) yi = 0;
			delayMs(speed);
		}
		if(-absX>absY){
				for(uint32_t i = 0; i<=(MAX(-absX,absY)-MIN(-absX,absY)); i++){
				GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
				GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
				xi--;
				if(xi<0) xi = 3;
				delayMs(speed);
			}
		}
		if(absY>-absX){
			for(uint32_t i = 0; i<=(MAX(-absX,absY)-MIN(-absX,absY)); i++){
				GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
				GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
				yi++;
				if(yi>3) yi = 0;
				delayMs(speed);
			}
		}
	}
	// In case absX is positive and absY negative
	else if((absX>=0)&&(absY<=0)){
		for(uint32_t i = 0; i<=MIN(absX,-absY); i++){
			GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
			GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
			xi++;
			if(xi>3) xi = 0;
			GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
			GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
			yi--;
			if(yi<0) yi = 3;
			delayMs(speed);
		}
		if(absX>-absY){
				for(uint32_t i = 0; i<=(MAX(absX,-absY)-MIN(absX,-absY)); i++){
				GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
				GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
				xi++;
				if(xi>3) xi = 0;
				delayMs(speed);
			}
		}
		if(-absY>absX){
			for(uint32_t i = 0; i<=(MAX(absX,-absY)-MIN(absX,-absY)); i++){
				GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
				GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
				yi--;
				if(yi<0) yi = 3;
				delayMs(speed);
			}
		}
	}
	// In case the two are negative
	// In case the 2 values are positive
	else if((absX<=0)&&(absY<=0)){
		for(uint32_t i = 0; i<=MIN(-absX,-absY); i++){
			GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
			GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
			xi--;
			if(xi<0) xi = 3;
			GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
			GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
			yi--;
			if(yi<0) yi = 3;
			delayMs(speed);
		}
		if(-absX>-absY){
				for(uint32_t i = 0; i<=(MAX(-absX,-absY)-MIN(-absX,-absY)); i++){
				GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
				GPIOB->ODR = (GPIOB->ODR | xMotorArray[xi]);
				xi--;
				if(xi<0) xi = 3;
				delayMs(speed);
			}
		}
		if(-absY>-absX){
			for(uint32_t i = 0; i<=(MAX(-absX,-absY)-MIN(-absX,-absY)); i++){
				GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
				GPIOD->ODR = (GPIOD->ODR | yMotorArray[yi]);
				yi--;
				if(yi<0) yi = 3;
				delayMs(speed);
			}
		}
	}
	}
	posActualX += absX;
	posActualY += absY;
	GPIOB->ODR = (GPIOB->ODR & 0xC3FF);
	GPIOD->ODR = (GPIOD->ODR & 0xFFC3);
}
