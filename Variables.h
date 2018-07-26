
#include <stm32f4xx.h>

static uint8_t minutes;
void setMinutes(uint8_t val);
uint8_t readMinutes(void);

static uint8_t seconds;
void setSeconds(uint8_t val);
uint8_t readSeconds(void);

static uint16_t ExpTimePassed2=0;
static uint16_t ExpTimeFinal2;
void calculateExpTimeFinal2(uint8_t minutes, uint8_t seconds);
void resetExpTime(void);
void halfTimeExpTime(void);
uint8_t checkExpTime(void);

static uint8_t power;
void setPower(uint8_t val);
uint8_t readPower(void);

static uint8_t alarm;
void setAlarm(uint8_t val);
uint8_t readAlarm(void);

static uint16_t timeup;
void setTimeUp(uint16_t val);
uint16_t readTimeUp(void);

static uint16_t timedn;
void setTimeDn(uint16_t val);
uint16_t readTimeDn(void);

static uint8_t motorNewPositionX, motorNewPositionY;
void setMotorNewPosition(uint8_t valX, uint8_t valY);
uint8_t readMotorNewPositionX(void);
uint8_t readMotorNewPositionY(void);

static uint8_t movingFlag=0;
void setMovingFlag(uint8_t val);
uint8_t readMovingFlag(void);

static uint8_t startLaserFlag = 0;
void setStartLaserFlag(uint8_t val);
uint8_t readStartLaserFlag(void);

static uint8_t experimentOnFlag = 0;
void setExperimentOnFlag(uint8_t val);
uint8_t readExperimentOnFlag(void);

static uint8_t experimentOffFlag = 0;
void setExperimentOffFlag(uint8_t val);
uint8_t readExperimentOffFlag(void);

uint16_t DAC_Calculate(void);

static uint8_t HalfTimeFlag=0;
uint8_t readHalfTimeFlag(void);
void setHalfTimeFlag(uint8_t val);
