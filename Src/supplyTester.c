#include "supplyTester.h"
#include "system.h"
#include "menu.h"
#include <stdio.h>

struct supplyTester_Struct supplyTester;

void supplyTesterInitStruct(void){
	supplyTester.correctionResistance = DEFAULT_CORRECTION_RESISTANCE;
	supplyTester.onResistance = 0;
	supplyTester.actualResistance = 0;
	supplyTester.relayWork = none;
	supplyTester.relayToSet = none;
}

void increaseCorrectionResistance(void){
	float increase;

	if(fastButtonIsEnable())
		increase = 0.5;
	else
		increase = 0.1;

	if(supplyTester.correctionResistance == 3.0){
		supplyTester.correctionResistance = 0.1;
		disableFastButton();
		return;
	}
	supplyTester.correctionResistance += increase;
	if(supplyTester.correctionResistance > 3.0){
		supplyTester.correctionResistance = 3.0;
		disableFastButton();
	}
}
char* getCorrectionResistanceString(void){
	static char correctionResistanceString[5];
	uint8_t num1 = supplyTester.correctionResistance;
	uint8_t num2 = (supplyTester.correctionResistance - num1) * 10;
	sprintf(correctionResistanceString, "%d.%d", num1, num2);
	return correctionResistanceString;
}

static void actualizeOnResistance(){
	if(supplyTester.relayWork == none)
		supplyTester.onResistance = 0;
	else if(supplyTester.relayWork == relay1)
		supplyTester.onResistance = RELAY_1;
	else if(supplyTester.relayWork == relay1and2)
		supplyTester.onResistance = RELAY_1_2;
	else if(supplyTester.relayWork == relay1and2and3)
		supplyTester.onResistance = RELAY_1_2_3;
	else if(supplyTester.relayWork == relay1and2and3and4)
		supplyTester.onResistance = RELAY_1_2_3_4;
}
