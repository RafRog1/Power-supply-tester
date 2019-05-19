#include "system.h"

uint32_t systemMSTime = 0;

void systemTimeHandle(void){
	systemMSTime++;
}

uint32_t getSystemMSTime(void){
	return systemMSTime;
}
