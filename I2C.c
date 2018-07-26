/**
*@file I2C.c
*@author Jose Manuel Terres
*@date 28th February 2018
*@brief I2C port functions
*/

#include <stm32f4xx_i2c.h>

/**
*@brief Initializes I2C1 on pins PB6 (SCL) and PB7 (SDA)
*/
void init_I2C1(void){
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); //SDA
	
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_ClockSpeed = 400000;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStruct);
	
	I2C_Cmd(I2C1, ENABLE);
	
}

/**
*@brief Generates a Start sequence to a selected slave address
*@param I2Cx number of I2C peripheral
*@param address is the slave address
*@param direction is I2C_Direction_Transmitter or I2C_Direction_Receiver
*/
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction){
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2Cx, ENABLE);
	
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2Cx, address, direction);
	
	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

/**
*@brief Sends one byte to the I2C bus
*@param I2Cx number of I2C port
*@param data byte
*/
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data){
	I2C_SendData(I2Cx, data);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/**
*@brief Read one byte from the slave device and acknowledge (request another byte)
*@param I2Cx number of I2C port
*/
uint8_t I2C_read_ack(I2C_TypeDef* I2Cx){
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

/**
*@brief Read one byte from the slave device and not acknowledge (generate stop)
*@param I2Cx number of I2C port
*/
uint8_t I2C_read_nack(I2C_TypeDef* I2Cx){
	I2C_AcknowledgeConfig(I2Cx,DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

/**
*@brief Generate a stop, release the bus
*@param I2Cx number of I2C port
*/
void I2C_stop(I2C_TypeDef* I2Cx){
	I2C_GenerateSTOP(I2Cx, ENABLE);
}
