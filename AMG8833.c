
/**
*@file AMG8833.c
*@author Jose Manuel Terres
*@date 28th February 2018
*@brief Reading the 8x8 pixel thermal camera AMG88xx
*/

#include "AMG8833.h"
#include "I2C.h"
#include <stm32f4xx_I2C.h>
#include <stdio.h>


I2C_TypeDef* amgSelectedPort;
char amgSlaveDirection;

// For debug purposes prints communications - will cause delay
#ifndef DEBUG
	#define DEBUG 1
#endif

/**
*@brief Sets where the AMG sensor is connected and its direction
*@param I2CPort
*@param direction
*@warning I2C Port has to be initialized separately (use I2C.h lib)
*/
void amgInit(I2C_TypeDef* I2CPort, char direction){
	amgSelectedPort = I2CPort;
	amgSlaveDirection = direction;
	if(DEBUG){
		printf("Set AMG88xx at 0x%x\r\n",amgSlaveDirection);
	}		
}

/**
*@brief Requests all the pixels
*@return 128 byte array
*/
void amgRequestPixels(uint8_t *pixelarray){
	for(uint8_t i=0;i<=127;i++){
		pixelarray[i] = amgRequestPixelByte(i);
	}
}

/**
*@brief Requests one pixel byte from the AMG88xx registers
*@warning Pixel 0 starts on register 0x80
*@param pixel
*@return One byte
*/
uint8_t amgRequestPixelByte(uint8_t pixel){
	I2C_start(amgSelectedPort, amgSlaveDirection<<1, I2C_Direction_Transmitter);
	uint8_t point = 0x80+pixel;
	if(DEBUG){
		printf("Req AMG88xx pixel 0x%x\r\n",point);
		for(uint32_t del=0; del<1000000; del++);
	}
	I2C_write(amgSelectedPort, point);
	I2C_stop(amgSelectedPort);
	I2C_start(amgSelectedPort, amgSlaveDirection<<1, I2C_Direction_Receiver);
	uint8_t recv = I2C_read_nack(amgSelectedPort);
	if(DEBUG){
		printf("Rcv AMG88xx 0x%x\r\n",recv);
		for(uint32_t del=0; del<1000000; del++);
	}
	return recv;
}

/**
*@brief Converts the 128-byte array of pixels into float temperatures
*@param temps 64 float array
*@param pixelarray 128 byte array
*@return 64 float values of temperature
*/
void amgBytes2Temp(float *temps, uint8_t *pixelarray){
	uint8_t j = 0;
	for(uint8_t i = 0 ; i<=127; i=i+2){
		uint16_t aux = (((uint16_t)pixelarray[i+1])<<8)+(uint16_t)pixelarray[i];
		uint16_t absVal = (aux & 0x7FF);
		temps[j] = (aux & 0x8000) ? 0.0 - (0.25*(double)absVal) : 0.25*(double)absVal;
		if(DEBUG){
			printf("Pixel temp %.2f\r\n",temps[j]);
			for(uint32_t del=0; del<1000000; del++);
		}
		j++;
	}
}
