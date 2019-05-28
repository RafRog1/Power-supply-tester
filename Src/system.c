#include "system.h"

uint32_t systemMSTime = 0;
uint8_t fastButtonEnable = 0;
uint8_t fastButtonCounter = 0;

void systemTimeHandle(void){
	systemMSTime++;
}

uint32_t getSystemMSTime(void){
	return systemMSTime;
}

void enableFastButton(void){
	if(fastButtonCounter < 3)
		fastButtonCounter++;

	if(fastButtonCounter > 2)
		fastButtonEnable = 1;
}
void disableFastButton(void){
	fastButtonCounter = 0;
	fastButtonEnable = 0;
}
uint8_t fastButtonIsEnable(void){
	return fastButtonEnable;
}
