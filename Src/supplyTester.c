#include "supplyTester.h"
#include "system.h"
#include "menu.h"
#include "gpio.h"
#include "main.h"
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
char *getCorrectionResistanceString(void){
	static char correctionResistanceString[5];
	uint8_t num1 = supplyTester.correctionResistance;
	uint8_t num2 = (supplyTester.correctionResistance - num1) * 10;
	sprintf(correctionResistanceString, "%d.%d", num1, num2);
	return correctionResistanceString;
}

static void actualizeOnResistance(void){
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
char *getRelayOn(void){
	static char relayDescription[10] = {0};
	uint8_t integer = 0;
	uint8_t decimal = 0;

	if(supplyTester.relayToSet == none){
		sprintf(relayDescription, "Przerwa");
		return relayDescription;
	} else if(supplyTester.relayToSet == relay1){
		integer = RELAY_1;
		decimal = (RELAY_1 - integer) * 10;
	} else if(supplyTester.relayToSet == relay1and2){
		integer = RELAY_1_2;
		decimal = (RELAY_1_2 - integer) * 10;
	} else if(supplyTester.relayToSet == relay1and2and3){
		integer = RELAY_1_2_3;
		decimal = (RELAY_1_2_3 - integer) * 10;
	} else if(supplyTester.relayToSet == relay1and2and3and4){
		integer = RELAY_1_2_3_4;
		decimal = (RELAY_1_2_3_4 - integer) * 10;
	}
	sprintf(relayDescription, "%d.%dohm", integer, decimal);

	return relayDescription;
}
void increaseRelayToSet(void){
	supplyTester.relayToSet++;
	if(supplyTester.relayToSet == relayOut)
		supplyTester.relayToSet = none;
}
static void activateSetRelay(void){
	if(supplyTester.relayWork == none){
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
	} else if (supplyTester.relayWork == relay1){
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
	} else if (supplyTester.relayWork == relay1and2){
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
	} else if (supplyTester.relayWork == relay1and2and3){
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
	} else if (supplyTester.relayWork == relay1and2and3and4){
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_SET);
	}
}
void acceptSetRelay(void){
	supplyTester.relayWork = supplyTester.relayToSet;
	actualizeOnResistance();
	supplyTester.actualResistance = supplyTester.onResistance + supplyTester.correctionResistance;
	activateSetRelay();
}
void resetSetRelay(void){
	supplyTester.relayToSet = supplyTester.relayWork;
}
uint8_t setRelayAndActualWorkIsEqual(void){
	return (supplyTester.relayToSet == supplyTester.relayWork) ? 1 : 0;
}

char* getTemperatureSensorString(void){
	static char strTemp[10] = {0};
	static float beforeTemperature = 1;
	static uint32_t generateTime = 0;
	float temperature = (float)(adc[4] * (3.3/40.95));

	if (!(beforeTemperature + 0.5 > temperature && beforeTemperature - 0.5 < temperature) && generateTime + 1000 < getSystemMSTime()){
		beforeTemperature = temperature;
		generateTime = getSystemMSTime();
		uint8_t integer = temperature;
		uint8_t decimal = (temperature - integer) * 10;

		if(integer < 10){
			sprintf(strTemp, "  %d.%d", integer, decimal);
		} else if(integer < 100){
			sprintf(strTemp, " %d.%d", integer, decimal);
		} else{
			sprintf(strTemp, "%d.%d", integer, decimal);
		}
	}

	return strTemp;
}
