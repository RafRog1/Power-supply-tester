//-------------------------------------------------------------------------------------------------

#include "HD44780.h"
#include "main.h"
#include "i2c.h"

//static uint8_t byte = 0;
static uint8_t byte = 8;

#define I2C_Pointer &hi2c1
#define I2C_adress (0x27 << 1)
#define ReceiveTimeMS 50

#define RS 1
#define RW (1 << 1)
#define E (1 << 2)
#define D4 (1 << 4)
#define D5 (1 << 5)
#define D6 (1 << 6)
#define D7 (1 << 7)
//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiaj¹ca pó³bajt na magistralê danych
//
//-------------------------------------------------------------------------------------------------
void _LCD_OutNibble(unsigned char nibbleToWrite){
	if(nibbleToWrite & 0x01){
		byte |= D4;
	}
	else{
		byte &= ~D4;
	}

	if(nibbleToWrite & 0x02){
		byte |= D5;
	}
	else{
		byte &= ~D5;
	}

	if(nibbleToWrite & 0x04){
		byte |= D6;
	}
	else{
		byte &= ~D6;
	}

	if(nibbleToWrite & 0x08){
		byte |= D7;
	}
	else{
		byte &= ~D7;
	}

	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyœwietacza (bez rozró¿nienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void _LCD_Write(unsigned char dataToWrite){
	byte |= E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	_LCD_OutNibble(dataToWrite >> 4);
	byte &= ~E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	HAL_Delay(1);
	byte |= E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	_LCD_OutNibble(dataToWrite);
	byte &= ~E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	HAL_Delay(5);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite){
	byte &= ~RS;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	_LCD_Write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamiêci wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite){
	byte |= RS;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	_LCD_Write(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyœwietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteText(char * text){
	while(*text)
	LCD_WriteData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wspó³rzêdnych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(unsigned char y, unsigned char x){
	LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wyœwietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void){
	LCD_WriteCommand(HD44780_CLEAR);
	HAL_Delay(2);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrócenia pocz¹tkowych wspó³rzêdnych wyœwietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void){
	LCD_WriteCommand(HD44780_HOME);
	HAL_Delay(2);
}
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void){
	unsigned char i;
	byte &= ~RS;
	byte &= ~E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);

	for(i = 0; i < 3; i++) // trzykrotne powtórzenie bloku instrukcji
	{
		byte |= E;
		HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
		_LCD_OutNibble(0x03); // tryb 8-bitowy
		byte &= ~E;
		HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
		HAL_Delay(5); // czekaj 5ms
	}

	byte |= E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);
	_LCD_OutNibble(0x02); // tryb 4-bitowy
	byte &= ~E;
	HAL_I2C_Master_Transmit(I2C_Pointer, I2C_adress, &byte, 1, ReceiveTimeMS);

	HAL_Delay(1); // czekaj 1ms
	LCD_WriteCommand(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT); // interfejs 4-bity, 2-linie, znak 5x7
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF); // wy³¹czenie wyswietlacza
	LCD_WriteCommand(HD44780_CLEAR); // czyszczenie zawartosæi pamieci DDRAM
	HAL_Delay(2);
	LCD_WriteCommand(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);// inkrementaja adresu i przesuwanie kursora
	LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK); // w³¹cz LCD, bez kursora i mrugania
}

//-------------------------------------------------------------------------------------------------
//
// Koniec pliku HD44780.c
//
//-------------------------------------------------------------------------------------------------
