/**
*@file ds18b20.c
*@author Jose Manuel Terres
*@date 28th February 2018
*@brief Read Maxim DS18B20 temperature sensors for STM32F4xx family
*@note Needs delay.c library and one timer
*/

#include "stm32f4xx.h" // Change this to match your device family
#include "ds18b20.h"
#include "delay.h"

/**@function ds18b20_Init
*@brief Initialize pin, port and timer for OneWire communication
*@param GPIO_TypeDef* GPIOx sensor port (A,B,C)
*@param uint16_t pin sensor pin
*@param TIM_TypeDef* tim timer to generate delays (1,2,3,4,5,6,7,8,12,13,14)
*@note You can add new GPIOx ports or new Timers if required by simply adding more else if statements. Code optimized for STM32F4xx series.
*@return None
*/
void ds18b20_Init(GPIO_TypeDef* GPIOx, uint16_t pin, TIM_TypeDef* tim){
	uint16_t temp;
	ds18b20_pin = pin;
	ds18b20_gpio = GPIOx;
	ds18b20_tim = tim;
	
	// Initialize GPIO clock
	// NOTE: family-dependant
	if(GPIOx == GPIOA){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	} else if(GPIOx == GPIOB){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	} else if(GPIOx == GPIOC){
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	}
	
	// Initialize timer clock
	// NOTE: family-dependant
	if(tim == TIM1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	} else if(tim == TIM2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	} else if (tim == TIM3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	} else if (tim == TIM4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	} else if (tim == TIM5){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	} else if (tim == TIM6){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	} else if (tim == TIM7){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	} else if (tim == TIM8){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	} else if (tim == TIM12){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	} else if (tim == TIM13){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	} else if (tim == TIM14){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	}
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(tim,&TIM_TimeBaseInitStruct);
	TIM_Cmd(tim,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
	do{
		temperature_convert();
		delayMs(800);
		temp = get_temperature();
	} while(temp==85);
	temperature_convert();	
}

/**@function temperature_convert
*@brief Tells all the sensors to start converting temperature
*@param None
*@return None
*/
void temperature_convert(void){
	ds18b20_reset();
	ds18b20_write_byte(0xCC);
	ds18b20_write_byte(0x44);
}

/**@function ds18b20_reset
*@brief Reset the bus to start communication
*@param None
*@return uint8_t Flag {1 if there are sensors, 0 if not}
*/
uint8_t ds18b20_reset(void){
	uint8_t Flag;
	GPIO_ResetBits(ds18b20_gpio,ds18b20_pin);
	microdelay(480, ds18b20_tim);
	GPIO_SetBits(ds18b20_gpio, ds18b20_pin);
	microdelay(60,ds18b20_tim);
	if(!GPIO_ReadInputDataBit(ds18b20_gpio, ds18b20_pin)) Flag = 1;
	else Flag = 0;
	while(!GPIO_ReadInputDataBit(ds18b20_gpio, ds18b20_pin));
	return Flag;
}

/**@function ds18b20_write_byte
*@brief Transmit one byte to the bus
*@param uint8_t cdat byte data
*@return None
*/
void ds18b20_write_byte(uint8_t cdat){
	uint8_t i, one_bit;
	//uint32_t t;
	for(i=0;i<8;i++){
		one_bit = cdat & 0x01;
		cdat = cdat >> 1;
		GPIO_ResetBits(ds18b20_gpio,ds18b20_pin);
		if(one_bit){
			microdelay(2,ds18b20_tim);
			GPIO_SetBits(ds18b20_gpio,ds18b20_pin);
			microdelay(58,ds18b20_tim);
		}
		else{
			microdelay(60,ds18b20_tim);
		}
		GPIO_SetBits(ds18b20_gpio,ds18b20_pin);
		microdelay(2,ds18b20_tim);
	}
}

/**@function get_temperature
*@brief Read temperature data from the sensors memory
*@param None
*@return uint16_t temperature data in 16-bit format
*/
uint16_t get_temperature(void){
	uint16_t a,b;
	uint16_t val;
	ds18b20_reset();
	ds18b20_write_byte(0xCC);
	ds18b20_write_byte(0xBE);
	a = (uint16_t)ds18b20_read_byte();
	b = (uint16_t)ds18b20_read_byte();
	val = (uint16_t)(b << 8 | a);
	ds18b20_reset();
	return val;
}

/**@function ds18b20_read_byte
*@brief Read one byte from the data bus
*@param None
*@return uint8_t data byte
*/
uint8_t ds18b20_read_byte(void){
	uint8_t i;
	uint8_t cdat;
	uint8_t value=0;
	for(i=0;i<8;i++){
		GPIO_ResetBits(ds18b20_gpio,ds18b20_pin);
		microdelay(2,ds18b20_tim);
		GPIO_SetBits(ds18b20_gpio,ds18b20_pin);
		microdelay(15,ds18b20_tim);
		cdat = GPIO_ReadInputDataBit(ds18b20_gpio,ds18b20_pin);
		if(cdat==0) while(!GPIO_ReadInputDataBit(ds18b20_gpio,ds18b20_pin));
		else microdelay(30,ds18b20_tim);
		value = (uint16_t)(cdat << 7)|(value >> 1);
	}
	return value;
}

/**@function ds18b20_celsius
*@brief Transform sensor temperature data to celsius
*@param uint16_t temperature data in 16-bit format
*@return float temperature in celsius
*/
float ds18b20_celsius(uint16_t byteval){
	uint16_t inv = (byteval & 0xF800);
	if(inv){
		byteval = (byteval^0xffff)+1;
	}
	if(inv){
		float val = byteval/16.0;
		return -val;
	}
	else{
	float val = byteval/16.0;
	return val;
	}
}

uint8_t ds18b20_read_bit(void){
	uint8_t r;
	GPIO_ResetBits(ds18b20_gpio, ds18b20_pin);
	microdelay(2, ds18b20_tim);
	GPIO_SetBits(ds18b20_gpio, ds18b20_pin);
	microdelay(15, ds18b20_tim);
	r = GPIO_ReadInputDataBit(ds18b20_gpio, ds18b20_pin);
	if(r==0) while(!GPIO_ReadInputDataBit(ds18b20_gpio, ds18b20_pin));
	else microdelay(30,ds18b20_tim);
	return r;
}

void ds18b20_write_bit(uint8_t bit){
	GPIO_ResetBits(ds18b20_gpio,ds18b20_pin);
	if(bit){
		microdelay(2,ds18b20_tim);
		GPIO_SetBits(ds18b20_gpio,ds18b20_pin);
		microdelay(58,ds18b20_tim);
	}
	else{
		microdelay(60,ds18b20_tim);
	}
	GPIO_SetBits(ds18b20_gpio,ds18b20_pin);
	microdelay(2,ds18b20_tim);
}

void ds18b20_select(uint8_t rom[8]){
	uint8_t i;
	ds18b20_write_byte(0x55);
	for (i=0; i<8; i++) ds18b20_write_byte(rom[i]);
}

void ds18b20_reset_search(void){
	LastDiscrepancy = 0;
	LastDeviceFlag = 0;
	LastFamilyDiscrepancy = 0;
	uint8_t i = 0;
	for(i = 7;;i--){
		ROM_NO[i] = 0;
		if(i==0) break;
	}
}

uint8_t ds18b20_search(uint8_t *newAddr,uint8_t search_mode){
	uint8_t id_bit_number;
	uint8_t last_zero, rom_byte_number, search_result;
	uint8_t id_bit, cmp_id_bit;
	
	unsigned char rom_byte_mask, search_direction;
	
	id_bit_number = 1;
	last_zero = 0;
	rom_byte_number = 0;
	rom_byte_mask = 1;
	search_result = 0;
	
	// If the last call was not the last
	if(!LastDeviceFlag){
		if(!ds18b20_reset()){ // 1-Wire reset
			// Reset the search
			LastDiscrepancy = 0;
			LastDeviceFlag = 0;
			LastFamilyDiscrepancy = 0;
			return 0;
		}
		
		// issue search command
		if(search_mode == 1){
			ds18b20_write_byte(0xF0); // Normal search
		} else {
			ds18b20_write_byte(0xEC);
		}
		
		// Loop to do the search
		do {
			// read a bit and its complement
			id_bit = ds18b20_read_bit();
			cmp_id_bit = ds18b20_read_bit();
			
			// check for no devices on 1-wire
			if ((id_bit == 1) && (cmp_id_bit == 1)) break;
			else{
				// All devices coupled have 0 or 1
				if(id_bit != cmp_id_bit)
					search_direction = id_bit; // bit write value for search
				else{
					// if this discrepancy if before the last discrepancy
					// on a previous next then pick the same as the last time
					if(id_bit_number < LastDiscrepancy) search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
					else search_direction = (id_bit_number == LastDiscrepancy); // if equal to lst pick 1, if not then pick 0
					
					// if 0 was picked then record its position in LastZero
					if(search_direction == 0){
						last_zero = id_bit_number;
						// Check for last discrepancy in family
						if(last_zero < 9) LastFamilyDiscrepancy = last_zero;
					}
				}
				
				// set or clear the bit in the ROM byte rom_byte_number
				// with mask rom_byte_mask
				if(search_direction == 1) ROM_NO[rom_byte_number] |= rom_byte_mask;
				else ROM_NO[rom_byte_number] &= ~rom_byte_mask;
				
				//serial number search direction write bit
				ds18b20_write_bit(search_direction);
				
				//increment the bytee counter id_bit_number
				// and shift the mask rom_byte_mask
				id_bit_number++;
				rom_byte_mask <<= 1;
				
				//if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
				if(rom_byte_mask == 0){
					rom_byte_number++;
					rom_byte_mask = 1;
				}
			}
		}	while(rom_byte_number < 8); //loop until through all ROM bytes 0-7
		
		//if the search was successful then
		if(!(id_bit_number < 65)){
			// search successful so set LastDiscrepancy, LastDeviceFlag, search_result
			LastDiscrepancy = last_zero;
			
			// check for last device
			if(LastDiscrepancy == 0) LastDeviceFlag = 1;
			
			search_result = 1;
		}
	}
	
	// if no device found then reset counters so next search will be like the first
	if(!search_result || !ROM_NO[0]){
		LastDiscrepancy = 0;
		LastDeviceFlag = 1;
		LastFamilyDiscrepancy = 0;
		search_result = 0;
	} else {
		for(uint8_t i = 0; i < 8; i++) newAddr[i] = ROM_NO[i];
	}
	
	return search_result;
}

uint16_t ds18b20_get_temperature_address(uint8_t* deviceAddress){
	uint16_t val;
	uint8_t a,b;

	ds18b20_reset();
	ds18b20_select(deviceAddress);
	//ds18b20_write_byte(0x44);
	//delayMs(800);
	ds18b20_write_byte(0xBE);
	a = (uint16_t)ds18b20_read_byte();
	b = (uint16_t)ds18b20_read_byte();
	val = (uint16_t)(b << 8 | a);
	ds18b20_reset();
	return val;
}

void ds18b20_setResolution(uint8_t resolution){
	ds18b20_reset();
	ds18b20_write_byte(0x4E); // Request write to configuration eeprom
	//dummy values for alarm settings
	ds18b20_write_byte(0);
	ds18b20_write_byte(100);
	//now set the resolution
	switch(resolution){
		case 9:
			ds18b20_write_byte(0x1F);
		break;
		case 10:
			ds18b20_write_byte(0x3F);
		break;
		case 11:
			ds18b20_write_byte(0x5F);
		break;
		case 12:
			ds18b20_write_byte(0x7F);
		break;
		default:
			ds18b20_write_byte(0x7F);
		break;
	}
	ds18b20_reset();
}
