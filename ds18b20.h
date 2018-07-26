	
#include <stm32f4xx.h>
	
static uint16_t	ds18b20_pin;
static GPIO_TypeDef*	ds18b20_gpio;
static TIM_TypeDef*	ds18b20_tim;

//Search variables
static uint8_t LastDiscrepancy = 0;
static uint8_t LastDeviceFlag = 0;
static uint8_t LastFamilyDiscrepancy = 0;
static unsigned char ROM_NO[8];

void ds18b20_Init(GPIO_TypeDef* GPIOx, uint16_t pin, TIM_TypeDef* tim);
void temperature_convert(void);
uint8_t ds18b20_reset(void);
void ds18b20_write_byte(uint8_t cdat);
uint16_t get_temperature(void);
uint8_t ds18b20_read_byte(void);
float ds18b20_celsius(uint16_t byteval);
uint8_t ds18b20_read_bit(void);
void ds18b20_write_bit(uint8_t bit);
void ds18b20_select(uint8_t rom[8]);
void ds18b20_reset_search(void);
uint8_t ds18b20_search(uint8_t *newAddr,uint8_t search_mode);
uint16_t ds18b20_get_temperature_address(uint8_t* deviceAddress);
void ds18b20_setResolution(uint8_t resolution);
