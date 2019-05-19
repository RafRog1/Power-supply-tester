#include "menuPrograms.h"
#include "HD44780.h"
#include "menu.h"
#include "main.h"
#include "gpio.h"

void startProgram(void){
	setActualView(startMenuView);
}

void ADCProgram(void){
	setActualView(ADCMenuView);
	if(someProgramIsActive()){
		setActualView(ADCProgramSet);
		while(someProgramIsActive()){
			setActualView(ADCProgramWork);
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			HAL_Delay(100);
		}
	}
}

void generatePWMProgram(void){
	setActualView(generatePWMMenuView);
}

void relayTestProgram(void){
	setActualView(relayTestMenuView);
	if(someProgramIsActive()){
		setActualView(relayTestSet);
		while(someProgramIsActive()){
			setActualView(relayTestWork);
		}
		relayTestProgramDeactivate();
	}
}
