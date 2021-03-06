#include "lcd.h"
#include "HD44780.h"
#include "menu.h"
#include "main.h"
#include "relayTest.h"
#include "supplyTester.h"
#include <string.h>

static char relayOn[10] = {0};
static char temp[10] = {0};
static char current[10] = {0};

void printCurrentView(void){
	static LCDView printedView = repeatThisView;

	if (printedView != getActualView()){
		switch(getActualView()){
		case startMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Wybierz program");
			LCD_GoTo(1, 7);
			LCD_WriteText("OK");
			LCD_GoTo(1, 13);
			LCD_WriteText("-->");

			printedView = startMenuView;
			break;
		case ADCMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Pomiar ADC");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--");
			LCD_GoTo(1, 7);
			LCD_WriteText("OK");
			LCD_GoTo(1, 13);
			LCD_WriteText("-->");

			printedView = ADCMenuView;
			break;
		case ADCProgramSet:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("A1:");
			LCD_GoTo(0, 8);
			LCD_WriteText("A2:");
			LCD_GoTo(1, 0);
			LCD_WriteText("A3:");
			LCD_GoTo(1, 8);
			LCD_WriteText("A4:");

			printedView = ADCProgramSet;
			break;
		case ADCProgramWork:
			LCD_GoTo(0, 3);
			LCD_double(adc[0]);
			LCD_GoTo(0, 11);
			LCD_double(adc[1]);
			LCD_GoTo(1, 3);
			LCD_double(adc[2]);
			LCD_GoTo(1, 11);
			LCD_double(adc[3]);

			printedView = repeatThisView;
			break;
		case generatePWMMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Generowanie PWM");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--");
			LCD_GoTo(1, 7);
			LCD_WriteText("OK");
			//LCD_GoTo(1, 13);
			//LCD_WriteText("-->");

			printedView = generatePWMMenuView;
			break;
		case relayTestMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Test wy. przek.");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--");
			LCD_GoTo(1, 7);
			LCD_WriteText("OK");
			LCD_GoTo(1, 13);
			LCD_WriteText("-->");

			printedView = relayTestMenuView;
			break;
		case relayTestSet:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("P1: 0");
			LCD_GoTo(0, 6);
			LCD_WriteText("P2: 0");
			LCD_GoTo(1, 0);
			LCD_WriteText("P3: 0");
			LCD_GoTo(1, 6);
			LCD_WriteText("P4: 0");

			printedView = relayTestSet;
			break;
		case relayTestWork:
			if(relayTestViewIsChanged()){
				LCD_GoTo(0, 4);
				LCD_WriteText(checkRelayStatus(1));
				LCD_GoTo(0, 10);
				LCD_WriteText(checkRelayStatus(2));
				LCD_GoTo(1, 4);
				LCD_WriteText(checkRelayStatus(3));
				LCD_GoTo(1, 10);
				LCD_WriteText(checkRelayStatus(4));
			}

			printedView = repeatThisView;
			break;
		case servisMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Serwis");
			LCD_GoTo(1, 0);
			LCD_WriteText("RKorekta:");
			LCD_GoTo(1, 10);
			LCD_WriteText(getCorrectionResistanceString());

			printedView = servisMenuView;
			break;
		case servisNumVisible:
			LCD_GoTo(1, 10);
			LCD_WriteText(getCorrectionResistanceString());

			printedView = servisNumVisible;
			break;
		case servisNumNotVisible:
			LCD_GoTo(1, 10);
			LCD_WriteText("   ");

			printedView = servisNumNotVisible;
			break;
		case workMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText(" Test zasilacza");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--");
			LCD_GoTo(1, 7);
			LCD_WriteText("OK");
			LCD_GoTo(1, 13);
			LCD_WriteText("-->");

			printedView = workMenuView;
			break;
		case workProgramInfo:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("P. P. zwieksza");
			LCD_GoTo(1, 0);
			LCD_WriteText("Sr.P. zatwierdza");

			printedView = workProgramInfo;
			break;
		case workingProgramFirstView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText(getRelayOn());
			LCD_GoTo(0, 10);
			LCD_WriteText(getTemperatureSensorString());
			LCD_GoTo(0, 15);
			LCD_WriteText("C");
			LCD_GoTo(1, 0);
			LCD_WriteText(getCurrentString());

			printedView = workingProgramFirstView;
			break;
		case workingProgramView:
			if(!strstr(getRelayOn(), relayOn)){
				LCD_GoTo(0, 0);
				LCD_WriteText(getRelayOn());
			}
			strcpy(relayOn, getRelayOn());
			if(!strstr(getTemperatureSensorString(), temp)){
				LCD_GoTo(0, 10);
				LCD_WriteText(getTemperatureSensorString());
			}
			strcpy(temp, getTemperatureSensorString());
			if(!strstr(getCurrentString(), current)){
				LCD_GoTo(1, 0);
				LCD_WriteText(getCurrentString());
			}
			strcpy(current, getCurrentString());

			printedView = repeatThisView;
			break;
		case workingProgramBlinkView1:
			LCD_GoTo(0, 0);
			LCD_WriteText("        ");

			printedView = workingProgramBlinkView1;
			break;
		case workingProgramBlinkView2:
			LCD_GoTo(0, 0);
			LCD_WriteText(getRelayOnBlink());

			printedView = workingProgramBlinkView2;
			break;
		default:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("!!  Blad  !!");
			LCD_GoTo(1, 0);
			LCD_WriteText("!! Zresetuj !!");

			break;
		}
	}
}

void LCD_double(uint32_t analogRead){
	double x = (3.3 * analogRead)/4095;
	uint8_t p;
	if(x > 1){
		p = x;
		LCD_WriteData(p + '0');
	} else {
		LCD_WriteData('0');
	}

	p = x * 10;
	if (p%10 > 0){
		LCD_WriteData(',');
		p = p%10;
		LCD_WriteData(p + '0');
	}
	else{
		LCD_WriteData(',');
		LCD_WriteData('0');
	}

	p = x * 100;
	if (p%10 > 0){
		p = p%10;
		LCD_WriteData(p + '0');
	} else {
		LCD_WriteData('0');
	}
}
