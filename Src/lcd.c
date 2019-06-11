#include "lcd.h"
#include "HD44780.h"
#include "menu.h"
#include "main.h"
#include "relayTest.h"
#include "supplyTester.h"

void printCurrentView(void){
	static LCDView printedView = repeatThisView;

	if (printedView != getActualView()){
		switch(getActualView()){
		case startMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Wybierz program");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--  !OK!  -->");

			printedView = startMenuView;
			break;
		case ADCMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Pomiar ADC");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--   OK   -->");

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
			LCD_WriteText("<--   OK   -->");

			printedView = generatePWMMenuView;
			break;
		case relayTestMenuView:
			LCD_Clear();
			LCD_GoTo(0, 0);
			LCD_WriteText("Test wy. przek.");
			LCD_GoTo(1, 0);
			LCD_WriteText("<--   OK   -->");

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
			LCD_WriteText("<--   OK   -->");

			printedView = workMenuView;
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
