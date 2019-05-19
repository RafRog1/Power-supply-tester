#include "relayTest.h"
#include "system.h"
#include "main.h"
#include "gpio.h"

static uint8_t viewChange = 0;

uint8_t viewIsChanged(void){
	if(viewChange){
		viewChange = 0;
		return 1;
	} else
		return 0;
}

static void changeRelay(void){
	static uint8_t relayNumber = 1;

	switch(relayNumber){
	case 1:
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
		viewChange = 1;
		relayNumber = 2;
		break;
	case 2:
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
		viewChange = 1;
		relayNumber = 3;
		break;
	case 3:
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
		viewChange = 1;
		relayNumber = 4;
		break;
	case 4:
		HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_SET);
		viewChange = 1;
		relayNumber = 1;
		break;
	default:
		relayTestProgramDeactivate();
		viewChange = 1;
		break;
	}
}

char* checkRelayStatus(uint8_t relayNumber){
	const static char* off = "0";
	const static char* on = "1";

	switch(relayNumber){
	case 1:
		if(HAL_GPIO_ReadPin(switch1_GPIO_Port, switch1_Pin) == GPIO_PIN_SET)
			return (char*)on;
		else
			return (char*)off;
		break;
	case 2:
		if(HAL_GPIO_ReadPin(switch2_GPIO_Port, switch2_Pin) == GPIO_PIN_SET)
			return (char*)on;
		else
			return (char*)off;
		break;
	case 3:
		if(HAL_GPIO_ReadPin(switch3_GPIO_Port, switch3_Pin) == GPIO_PIN_SET)
			return (char*)on;
		else
			return (char*)off;
		break;
	case 4:
		if(HAL_GPIO_ReadPin(switch4_GPIO_Port, switch4_Pin) == GPIO_PIN_SET)
			return (char*)on;
		else
			return (char*)off;
		break;
	}
}

void relayTestLoop(void){
	static uint32_t changeRelayTime = 0;
	const static uint16_t msToRelayChange = 1000;

	if(changeRelayTime + msToRelayChange < getSystemMSTime()){
		changeRelay();
		changeRelayTime = getSystemMSTime();
	}
}

void relayTestProgramDeactivate(void){
	HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
}
