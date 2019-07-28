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
	supplyTester.status = 0;
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
static double calculateResistanceOfResostors(uint8_t res4, uint8_t res3, uint8_t res2, uint8_t res1){
	uint8_t divider = 0;

	if(res4)
		divider++;
	if(res3)
		divider++;
	if(res2)
		divider++;
	if(res1)
		divider++;

	if (divider == 0)
		return 0;

	return ((res4 * RESISTOR_4) + (res3 * RESISTOR_3) + (res2 * RESISTOR_2) + (res1 * RESISTOR_1)) / divider;
}
static void actualizeOnResistance(void){
	if(supplyTester.relayWork == none)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 0, 0, 0);
	else if(supplyTester.relayWork == relay1)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 0, 0, 1);
	else if(supplyTester.relayWork == relay1and2)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 0, 1, 1);
	else if(supplyTester.relayWork == relay1and3)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 1, 0, 1);
	else if(supplyTester.relayWork == relay1and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 0, 0, 1);
	else if(supplyTester.relayWork == relay1and2and3)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 1, 1, 1);
	else if(supplyTester.relayWork == relay1and2and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 0, 1, 1);
	else if(supplyTester.relayWork == relay1and3and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 1, 0, 1);
	else if(supplyTester.relayWork == relay2)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 0, 1, 0);
	else if(supplyTester.relayWork == relay2and3)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 1, 1, 0);
	else if(supplyTester.relayWork == relay2and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 0, 1, 0);
	else if(supplyTester.relayWork == relay2and3and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 1, 1, 0);
	else if(supplyTester.relayWork == relay3)
		supplyTester.onResistance = calculateResistanceOfResostors(0, 1, 0, 0);
	else if(supplyTester.relayWork == relay3and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 1, 0, 0);
	else if(supplyTester.relayWork == relay4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 0, 0, 0);
	else if(supplyTester.relayWork == relay1and2and3and4)
		supplyTester.onResistance = calculateResistanceOfResostors(1, 1, 1, 1);
}
char *getRelayOnBlink(void){
	static char relayDescription[10] = {0};
	uint8_t integer = 0;
	uint8_t decimal = 0;

	if(supplyTester.relayToSet == none){
		sprintf(relayDescription, "Przerwa");
		return relayDescription;
	}
	else if(supplyTester.relayToSet == relay1){
		integer = calculateResistanceOfResostors(0, 0, 0, 1);
		decimal = (calculateResistanceOfResostors(0, 0, 0, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and2){
		integer = calculateResistanceOfResostors(0, 0, 1, 1);
		decimal = (calculateResistanceOfResostors(0, 0, 1, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and3){
		integer = calculateResistanceOfResostors(0, 1, 0, 1);
		decimal = (calculateResistanceOfResostors(0, 1, 0, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and4){
		integer = calculateResistanceOfResostors(1, 0, 0, 1);
		decimal = (calculateResistanceOfResostors(1, 0, 0, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and2and3){
		integer = calculateResistanceOfResostors(0, 1, 1, 1);
		decimal = (calculateResistanceOfResostors(0, 1, 1, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and2and4){
		integer = calculateResistanceOfResostors(1, 0, 1, 1);
		decimal = (calculateResistanceOfResostors(1, 0, 1, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and3and4){
		integer = calculateResistanceOfResostors(1, 1, 0, 1);
		decimal = (calculateResistanceOfResostors(1, 1, 0, 1) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay2){
		integer = calculateResistanceOfResostors(0, 0, 1, 0);
		decimal = (calculateResistanceOfResostors(0, 0, 1, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay2and3){
		integer = calculateResistanceOfResostors(0, 1, 1, 0);
		decimal = (calculateResistanceOfResostors(0, 1, 1, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay2and4){
		integer = calculateResistanceOfResostors(1, 0, 1, 0);
		decimal = (calculateResistanceOfResostors(1, 0, 1, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay2and3and4){
		integer = calculateResistanceOfResostors(1, 1, 1, 0);
		decimal = (calculateResistanceOfResostors(1, 1, 1, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay3){
		integer = calculateResistanceOfResostors(0, 1, 0, 0);
		decimal = (calculateResistanceOfResostors(0, 1, 0, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay3and4){
		integer = calculateResistanceOfResostors(1, 1, 0, 0);
		decimal = (calculateResistanceOfResostors(1, 1, 0, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay4){
		integer = calculateResistanceOfResostors(1, 0, 0, 0);
		decimal = (calculateResistanceOfResostors(1, 0, 0, 0) - integer) * 10;
	}
	else if(supplyTester.relayToSet == relay1and2and3and4){
		integer = calculateResistanceOfResostors(1, 1, 1, 1);
		decimal = (calculateResistanceOfResostors(1, 1, 1, 1) - integer) * 10;
	}

	sprintf(relayDescription, "%d.%dohm", integer, decimal);
	return relayDescription;
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
	supplyTester.status |= onResistanceBlinkMode;
	if(supplyTester.relayToSet == relayOut)
		supplyTester.relayToSet = none;
}
static void setSwitch(uint8_t sw4, uint8_t sw3, uint8_t sw2, uint8_t sw1){
	if(sw4)
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);

	if(sw3)
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);

	if(sw2)
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);

	if(sw1)
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
}
static void activateSetRelay(void){
	if(supplyTester.relayWork == none){
		setSwitch(0, 0, 0, 0);
	}
	else if(supplyTester.relayWork == relay1){
		setSwitch(0, 0, 0, 1);
	}
	else if(supplyTester.relayWork == relay1and2){
		setSwitch(0, 0, 1, 1);
	}
	else if(supplyTester.relayWork == relay1and3){
		setSwitch(0, 1, 0, 1);
	}
	else if(supplyTester.relayWork == relay1and4){
		setSwitch(1, 0, 0, 1);
	}
	else if(supplyTester.relayWork == relay1and2and3){
		setSwitch(0, 1, 1, 1);
	}
	else if(supplyTester.relayWork == relay1and2and4){
		setSwitch(1, 0, 1, 1);
	}
	else if(supplyTester.relayWork == relay1and3and4){
		setSwitch(1, 1, 0, 1);
	}
	else if(supplyTester.relayWork == relay2){
		setSwitch(0, 0, 1, 0);
	}
	else if(supplyTester.relayWork == relay2and3){
		setSwitch(0, 1, 1, 0);
	}
	else if(supplyTester.relayWork == relay2and4){
		setSwitch(1, 0, 1, 0);
	}
	else if(supplyTester.relayWork == relay2and3and4){
		setSwitch(1, 1, 1, 0);
	}
	else if(supplyTester.relayWork == relay3){
		setSwitch(0, 1, 0, 0);
	}
	else if(supplyTester.relayWork == relay3and4){
		setSwitch(1, 1, 0, 0);
	}
	else if(supplyTester.relayWork == relay4){
		setSwitch(1, 0, 0, 0);
	}
	else if(supplyTester.relayWork == relay1and2and3and4){
		setSwitch(1, 1, 1, 1);
	}
}
char *getCurrentString(void){
	static char currentString[10] = {0};
	static uint32_t generateTime = 0;
	static float beforCurrent = (float)0xFF;
	float current;

	if(supplyTester.relayWork == none)
		current = 0;
	else
		current = (float)((adc[0] * 3.3/409.5)/supplyTester.actualResistance);

	if (!(beforCurrent + 0.05 > current && beforCurrent - 0.05 < current) && generateTime + 1000 < getSystemMSTime()){
		beforCurrent = current;
		uint8_t integer = current;
		uint8_t decimal = (current - integer) * 100;
		generateTime = getSystemMSTime();

		if(decimal < 10)
			sprintf(currentString, "%d.0%d[A]", integer, decimal);
		else
			sprintf(currentString, "%d.%d[A]", integer, decimal);
	}
	return currentString;
}
void disableRelays(void){
	HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
}
void acceptSetRelay(void){
	supplyTester.relayWork = supplyTester.relayToSet;
	actualizeOnResistance();
	supplyTester.actualResistance = supplyTester.onResistance + supplyTester.correctionResistance;
	activateSetRelay();
	supplyTester.status &= ~onResistanceBlinkMode;
}
void resetSetRelay(void){
	supplyTester.relayToSet = supplyTester.relayWork;
}
uint8_t isBlinkMode(void){
	return (supplyTester.status & onResistanceBlinkMode) ? 1 : 0;
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
