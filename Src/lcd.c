#include "lcd.h"
#include "HD44780.h"

void LCD_double(uint32_t analogRead){
	double x = (3.3 * analogRead)/4095;
	uint8_t p;
	if(x > 1){
		p = x;
		LCD_WriteData(p + '0');
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
	}
}
