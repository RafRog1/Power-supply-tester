#include "menuPrograms.h"
#include "HD44780.h"
#include "menu.h"
#include "main.h"
#include "gpio.h"
#include "lcd.h"
#include "relayTest.h"
#include "system.h"
#include "supplyTester.h"

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

void servisProgram(void){
	uint8_t numVisible = 1;
	setActualView(servisMenuView);
	if(someProgramIsActive()){
		while(someProgramIsActive()){
			if(numVisible){
				setActualView(servisNumVisible);
				printCurrentView();
				numVisible = 0;
				HAL_Delay(400);
			} else {
				setActualView(servisNumNotVisible);
				printCurrentView();
				numVisible = 1;
				HAL_Delay(200);
			}
		}
	}
}

static void blinkingView(void){
	static uint32_t timeToBlinkLCD = 0;
	if(timeToBlinkLCD + 500 < getSystemMSTime()){
		timeToBlinkLCD = getSystemMSTime();
		if(getActualView() == workingProgramBlinkView2)
			setActualView(workingProgramBlinkView1);
		else
			setActualView(workingProgramBlinkView2);
	}
}
static void normalView(uint8_t firstPrint){
	if(firstPrint)
		setActualView(workingProgramFirstView);
	else
		setActualView(workingProgramView);
}
void workProgram(void){
	static uint8_t shouldPrintInfo = 1;
	static uint8_t firstPrintMenuView = 1;
	if(shouldPrintInfo)
		setActualView(workMenuView);
	if(someProgramIsActive()){
		if(shouldPrintInfo){
			setActualView(workProgramInfo);
			setHelpTime(getSystemMSTime());
			shouldPrintInfo = 0;
		}
		else if(getHelpTime() + timeInMsToPrintInfo < getSystemMSTime()){
			if(setRelayAndActualWorkIsEqual()){
				normalView(firstPrintMenuView);
				firstPrintMenuView = 0;
			} else{
				blinkingView();
			}

		}
	} else {
		shouldPrintInfo = 1;
		firstPrintMenuView = 1;
		disableRelays();
	}
}
