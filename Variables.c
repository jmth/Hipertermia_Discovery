
#include "Variables.h"


#define PiAB 0.159043128F //!< Calibrated value for the area irradiated by laser light

/**
* TIMING VARIABLES
**/
void setMinutes(uint8_t val){
	minutes = val;
}
uint8_t readMinutes(void){
	return minutes;
}
void setSeconds(uint8_t val){
	seconds = val;
}
uint8_t readSeconds(void){
	return seconds;
}
uint8_t readHalfTimeFlag(void){
	return HalfTimeFlag;
}
void setHalfTimeFlag(uint8_t val){
	HalfTimeFlag = val;
}
void calculateExpTimeFinal2(uint8_t minutes, uint8_t seconds){
	ExpTimeFinal2 = ((((uint16_t)minutes)*60)+((uint16_t)seconds))*2;
}
void resetExpTime(){
	ExpTimePassed2 = 0;
}
void halfTimeExpTime(){
	ExpTimePassed2++;
}
uint8_t checkExpTime(void){
	if(ExpTimePassed2 >= ExpTimeFinal2) return 1;
	else return 0;
}
/**
* LASER POWER VARIABLES
**/
void setPower(uint8_t val){
	power = val;
}
uint8_t readPower(void){
	return power;
}
void setTimeUp(uint16_t val){
	timeup = val;
}
uint16_t readTimeUp(void){
	return timeup;
}
void setTimeDn(uint16_t val){
	timedn = val;
}
uint16_t readTimeDn(void){
	return timedn;
}
void setStartLaserFlag(uint8_t val){
	startLaserFlag = val;
}
uint8_t readStartLaserFlag(void){
	return startLaserFlag;
}
void setExperimentOffFlag(uint8_t val){
	experimentOffFlag = val;
}
uint8_t readExperimentOffFlag(void){
	return experimentOffFlag;
}
uint16_t DAC_Calculate(void){
	uint16_t DACValue;
	DACValue = (int)((((readPower()/100.0)*PiAB))*2.7*4096/(3.3*0.6));
	return DACValue;
}
/**
* ALARM VARIABLES
**/
void setAlarm(uint8_t val){
	alarm = val;
}
uint8_t readAlarm(void){
	return alarm;
}

/**
* MOTORS VARIABLES
**/
void setMotorNewPosition(uint8_t valX, uint8_t valY){
	motorNewPositionX = valX;
	motorNewPositionY = valY;
}
uint8_t readMotorNewPositionX(void){
	return motorNewPositionX;
}
uint8_t readMotorNewPositionY(void){
	return motorNewPositionY;
}
void setMovingFlag(uint8_t val){
	movingFlag = val;
}
uint8_t readMovingFlag(void){
	return movingFlag;
}


void setExperimentOnFlag(uint8_t val){
	experimentOnFlag = val;
}
uint8_t readExperimentOnFlag(void){
	return experimentOnFlag;
}



