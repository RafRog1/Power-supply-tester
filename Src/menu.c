#include "menu.h"
#include "gpio.h"

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
	if(menu.enableExtiButton > 0){
		return 1;
	} else{
		return 0;
	}
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
			enableExtiButton();
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
	}
}
void okButtonHandle(void){
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
	disableExtiButton();
	if(getCurrentChosenProgram() != startMenu){
		activateProgram();
	}
}
