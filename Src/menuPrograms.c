#include "menuPrograms.h"
#include "HD44780.h"
#include "menu.h"
#include "main.h"
#include "gpio.h"
#include "lcd.h"
#include "relayTest.h"

void startProgram(void){
	setActualView(startMenuView);
}

void ADCProgram(void){
	setActualView(ADCMenuView);
	if(someProgramIsActive()){
		setActualView(ADCProgramSet);
		printCurrentView();
		while(someProgramIsActive()){
			setActualView(ADCProgramWork);
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			printCurrentView();
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
		printCurrentView();
		while(someProgramIsActive()){
			setActualView(relayTestWork);
			relayTestLoop();
			printCurrentView();
			HAL_Delay(100);
		}
		relayTestProgramDeactivate();
	}
}
