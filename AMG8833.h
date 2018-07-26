/**
*@file AMG8833.h
*@author Jose Manuel Terres
*@date 28th February 2018
*@brief Reading the 8x8 pixel thermal camera AMG88xx
*/

#include <stm32f4xx.h>
#include <stm32f4xx_I2C.h>

#define AMG_PIXELS_OFFSET 0x80;



void amgInit(I2C_TypeDef* I2CPort, char direction);
void amgRequestPixels(uint8_t *pixelarray);
uint8_t amgRequestPixelByte(uint8_t pixel);
void amgBytes2Temp(float *temps, uint8_t *pixelarray);
