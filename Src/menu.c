#include "menu.h"
#include "main.h"
#include "gpio.h"
#include "supplyTester.h"
#include "system.h"

struct menuStruct menu;

void menuInit(void){
	menu.enableExtiButton = 1;
	menu.chosenProgram = startMenu;
	menu.actualLCDView = startMenuView;
	menu.aceptProgram = 0;
	menu.msDisableExti = 0;

}

LCDView getActualView(void){
	return menu.actualLCDView;
}
void setActualView(LCDView view){
	menu.actualLCDView = view;
}

uint8_t extiButtonIsEnable(void){
	return (menu.enableExtiButton > 0) ? 1 : 0;
}
static void disableExtiButton(void){
	menu.enableExtiButton = 0;
}
static void enableExtiButton(void){
	menu.enableExtiButton = 1;
}
void enableExtiButtonAfterTime(uint32_t timeToEnableMS){
	if(!extiButtonIsEnable()){
		menu.msDisableExti++;
		if(menu.msDisableExti >= timeToEnableMS){
			menu.msDisableExti = 0;
			if(HAL_GPIO_ReadPin(P1_GPIO_Port, P1_Pin) == GPIO_PIN_RESET){
				leftButtonHandle();
				enableFastButton();
				disableExtiButton();
			} else if(HAL_GPIO_ReadPin(P2_GPIO_Port, P2_Pin) == GPIO_PIN_RESET){
				okButtonHandle();
				enableFastButton();
				disableExtiButton();
			} else if(HAL_GPIO_ReadPin(P3_GPIO_Port, P3_Pin) == GPIO_PIN_RESET){
				rightButtonHandle();
				enableFastButton();
				disableExtiButton();
			} else{
				disableFastButton();
				enableExtiButton();
			}
		}
	}
}

uint8_t someProgramIsActive(void){
	return menu.aceptProgram;
}
static void deactivateProgram(void){
	menu.aceptProgram = 0;
}
static void activateProgram(void){
	menu.aceptProgram = 1;
}

programName getCurrentChosenProgram(void){
	return menu.chosenProgram;
}
static void increaseChosenProgram(void){
	if (getCurrentChosenProgram() < menuMax - 1)
		menu.chosenProgram++;
}
static void decreaseChosenProgram(void){
	if (getCurrentChosenProgram() > startMenu)
		menu.chosenProgram--;
}

void leftButtonHandle(void){
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	disableExtiButton();
	if (someProgramIsActive()){
		if(getCurrentChosenProgram() == workMenu && isBlinkMode())
			resetSetRelay();
		else
			deactivateProgram();
	} else{
		decreaseChosenProgram();
	}
}
void rightButtonHandle(void){
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	disableExtiButton();
	if(!someProgramIsActive()){
		increaseChosenProgram();
	} else if(getCurrentChosenProgram() == serviceMenu){
		increaseCorrectionResistance();
	} else if(getCurrentChosenProgram() == workMenu){
		increaseRelayToSet();
	}
}
void okButtonHandle(void){
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	disableExtiButton();
	if(getCurrentChosenProgram() != startMenu){
		if(getCurrentChosenProgram() == workMenu && someProgramIsActive())
			acceptSetRelay();
		else
			activateProgram();
	}
}
