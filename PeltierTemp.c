
#include "PeltierTemp.h"
#include "ds1804.h"
#include "stdio.h"
#include "USART.h"

float error = 0, tempant = 25, Target = 15.0;

// Here is implemented the peltier control
void Control_Temp(int temp){
	float cels = temp/100.0;
	error = 0.7*(cels-Target)+(error*0.35)-((tempant-cels)*4);
	tempant = cels;
	int8_t change_req = (int8_t)(error*1);
	if(change_req<0){
		ds1804_up(-(change_req));
	} else{
		ds1804_down(change_req);
	}
}
