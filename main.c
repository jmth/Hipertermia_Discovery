/**
   @file main.c

   @brief Hyperthermia STM32F4Discovery Code
   @author Jose Manuel Terres joterha@upv.es
   @date July 2018
*/

#include "stm32f4_discovery.h"
#include <stdio.h>
#include "button.h"
#include "delay.h"
#include "USART.h"
#include "ADC.h"
#include "TIMInit.h"
#include "serialcomms.h"
#include "Variables.h"
#include "IntelligentDAC.h"
#include "Motors.h"
#include "ds18b20.h"
#include "ds1804.h"
#include "PeltierTemp.h"

// Temperature sensor addresses
uint8_t tempSensor1[8] = {40,209,52,190,9,0,0,124};
uint8_t tempSensor2[8] = {40,165,87,190,9,0,0,103};

#ifndef DEBUG
	#define DEBUG 1
#endif

uint8_t dob = 0; // Temperature control sampling ratio

// Timer 0.5 s interrupt handling
void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)){
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		halfTimeExpTime(); // Add half second to passed experiment time
		setHalfTimeFlag(1); // Set flag to send info to GUI
		// If time passed is equal to maximum, stop experiment
		if(checkExpTime()) setExperimentOffFlag(1);
	}
}

int main(void)
{
	delayInit(); // Initialize timers and delays
	normalDAC(0); // Initialize DAC and set output to 0
	StopDAC(); // Stop DAC
	init_led(); // Initialize board LED
	init_USART6(); // Initialize USART communication
	initMotors(); // Initialize motors pinout
	ds18b20_Init(GPIOA,GPIO_Pin_15,TIM14); // Initialize temperature sensors
	ds18b20_setResolution(9);
	// Initialize digital potentiometer pinout
	ds1804_init(GPIO_Pin_1,GPIOE,GPIO_Pin_2,GPIOE,GPIO_Pin_0,GPIOE,TIM14);
	temperature_convert(); // Send temperature request to sensors
	USART_puts(USART6, "HT 1064nm!!\r\n"); // Put a "hello world" into USART comm
	TIM2_INT_Init(); // Initialize 0.5 s timer (no start)
	ADCDMA_Init(); // Initialize ADC readings
	zeroMotors(); // Move the motors to zero position
	TIM_Cmd(TIM2,ENABLE); // Enable 0.5 s timer (start)
	while(1){
		// If something received in USART, send ACK and go to parse the array
		if(usart6_flagRead()){
			parsecomms();
			USART_puts(USART6, "ACK\r\n");
			usart6_flagDn();
		}

		// If moving motors is required move then set flag false
		if(readMovingFlag()){
			moveMotors(getAbsX(),getAbsY());
			setMovingFlag(0);
		}
		
		// If stopping experiment, set off laser and reset variables and timers
		if(readExperimentOffFlag()){
			StopDAC();
			setExperimentOnFlag(0);
			setExperimentOffFlag(0);
			led_off();
			resetExpTime();
			//USART_puts(USART6,"1,2,\r\n");
		}
		
		// If required to start experiment, check variables then proceed
		if(readStartLaserFlag()){
			if(readTimeDn()==0){
				//Normally start ADC with continuous power
				normalDAC(DAC_Calculate());
			} else{
				//Start IntelligentDAC
				IntelligentDAC_Init(0,DAC_Calculate(),readTimeDn(),readTimeUp());
			}
			resetExpTime();
			calculateExpTimeFinal2(readMinutes(),readSeconds());
			setStartLaserFlag(0);
			setExperimentOnFlag(1);
		}
		
		// If experiment is On
		if(readExperimentOnFlag()){
			// Nothing to do here
		}
		
		// If 0.5 s passed
		// Check temperatures
		// Apply temperature controls
		// Send variables to GUI
		if(readHalfTimeFlag()){
			int temp1 = (int)(ds18b20_celsius(ds18b20_get_temperature_address(tempSensor1))*100);
			int temp2 = (int)(ds18b20_celsius(ds18b20_get_temperature_address(tempSensor2))*100);
			temperature_convert();
			if(dob>0){
				Control_Temp(temp2);
				dob = 0;
			}
			dob++;
			char str[50];
			sprintf(str,"2,%d,%d,%d,%d,%d,\r\n",ADCread(0),ADCread(1),ADCread(2),temp1,temp2);
			USART_puts(USART6,str);
			setHalfTimeFlag(0);
		}
		// Send information to computer
	}
}
