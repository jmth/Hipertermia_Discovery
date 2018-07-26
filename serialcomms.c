
#include <stm32f4_discovery.h>
#include <stm32f4xx.h>
#include "USART.h"
#include "button.h"
#include "serialcomms.h"
#include <stdio.h>
#include "delay.h"
#include "Variables.h"
#include "IntelligentDAC.h"
#include "Motors.h"

#ifndef DEBUG
	#define DEBUG 1
#endif
char str[25];
int xpos,ypos;

// Parse communications reveived
void parsecomms(void){
	char aux[8];
	read_com(aux);
	
	if(aux[0] == 0x61){
		switch(aux[1]){
			case 0x61:
				// Code to start experiment (recommend flag)
				setStartLaserFlag(1);
				#if DEBUG
					led_on();
					//delayMs(readSeconds()*1000);
					//led_off();
				#endif
			break;
			case 0x63:
				// Code to stop experiment
				//StopDAC();
				//resetExpTime();
				setExperimentOffFlag(1);
				#if DEBUG
					led_off();
				#endif
			break;
			case 0x6D:
				// Read aux[2] for minutes time
				setMinutes(aux[2]);
				setSeconds(aux[3]);
			break;
			case 0x73:
				// Read aux[2] for seconds time
			break;
			case 0x74:
				// PWM cycle time aux[2] and aux[3] 16-bit data integer
				setTimeUp((((uint16_t)aux[2])<<8) | ((uint16_t)aux[3]));
				setTimeDn((((uint16_t)aux[4])<<8) | ((uint16_t)aux[5]));
			break;
			case 0x75:
				// PWM up time aux[2] and aux[3] 16-bit data integer
			break;
			case 0x62:
				// Alarm ON / OFF read aux[2] for data (bool)
				setAlarm(aux[2]);
			break;
			case 0x6C:
				// Read aux[2] for laser power data
				setPower(aux[2]);
			break;
			case 0x78:
				// Read aux[2] for experiment temperature set data
			break;
			case 0x54:
				// Read aux[2] for manual override data (bool)
			break;
			case 0x4C:
				// If manual ON
				// Laser manual ON, read aux[2] for power level data
			break;
			case 0x43:
				// If manual ON
				// Laser manual OFF
			break;
			case 0x58:
				// If manual ON
				// Heater manual ON / OFF read aux[2] for data (bool)
			break;
			case 0x56:
				// If manual ON
				// Fan manual ON / OFF read aux[2] for data (bool)
			break;
			case 0x4D:
				// Moving motors to position X:aux[2] Y:aux[3]
				setMotorNewPosition(aux[2], aux[3]);
				// Flag to check and move
				setMovingFlag(1);
			break;
			case 0x4E:
				if(aux[2]==1){
					moveMotors(1,0);
				} else if(aux[3]==1){
					moveMotors(-1,0);
				} else if(aux[4]==1){
					moveMotors(0,1);
				} else if(aux[5]==1){
					moveMotors(0,-1);
				}
			break;
			
			
			default:
				// ?????? Transmit comm error no identified commands
			break;
		}
	}
}
