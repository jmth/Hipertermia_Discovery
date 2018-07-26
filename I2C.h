/**
*@file I2C.h
*@author Jose Manuel Terres
*@date 28th February 2018
*@brief I2C port functions
*/
#include <stm32f4xx_I2C.h>

void init_I2C1(void);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx);
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);
