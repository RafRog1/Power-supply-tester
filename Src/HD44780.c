//-------------------------------------------------------------------------------------------------

#include "HD44780.h"
#include "main.h"
//-------------------------------------------------------------------------------------------------
//
// Funkcja wystawiaj¹ca pó³bajt na magistralê danych
//
//-------------------------------------------------------------------------------------------------
void _LCD_OutNibble(unsigned char nibbleToWrite)
{
if(nibbleToWrite & 0x01)
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, SET);
else
	HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, RESET);

if(nibbleToWrite & 0x02)
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, SET);
else
	HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, RESET);

if(nibbleToWrite & 0x04)
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, SET);
else
	HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, RESET);

if(nibbleToWrite & 0x08)
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, SET);
else
	HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, RESET);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu bajtu do wyœwietacza (bez rozró¿nienia instrukcja/dane).
//
//-------------------------------------------------------------------------------------------------
void _LCD_Write(unsigned char dataToWrite)
{
HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, SET);
_LCD_OutNibble(dataToWrite >> 4);
HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, RESET);
HAL_Delay(1);
HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, SET);
_LCD_OutNibble(dataToWrite);
HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, RESET);
HAL_Delay(50);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu rozkazu do wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteCommand(unsigned char commandToWrite)
{
HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, RESET);
_LCD_Write(commandToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja zapisu danych do pamiêci wyœwietlacza
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteData(unsigned char dataToWrite)
{
HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, SET);
_LCD_Write(dataToWrite);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja wyœwietlenia napisu na wyswietlaczu.
//
//-------------------------------------------------------------------------------------------------
void LCD_WriteText(char * text)
{
while(*text)
  LCD_WriteData(*text++);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja ustawienia wspó³rzêdnych ekranowych
//
//-------------------------------------------------------------------------------------------------
void LCD_GoTo(unsigned char x, unsigned char y)
{
LCD_WriteCommand(HD44780_DDRAM_SET | (x + (0x40 * y)));
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja czyszczenia ekranu wyœwietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Clear(void)
{
LCD_WriteCommand(HD44780_CLEAR);
HAL_Delay(2);
}
//-------------------------------------------------------------------------------------------------
//
// Funkcja przywrócenia pocz¹tkowych wspó³rzêdnych wyœwietlacza.
//
//-------------------------------------------------------------------------------------------------
void LCD_Home(void)
{
LCD_WriteCommand(HD44780_HOME);
HAL_Delay(2);
}
//-------------------------------------------------------------------------------------------------
//
// Procedura inicjalizacji kontrolera HD44780.
//
//-------------------------------------------------------------------------------------------------
void LCD_Initalize(void)
{
unsigned char i;
HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, RESET); // wyzerowanie linii RS
HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, RESET);  // wyzerowanie linii E

for(i = 0; i < 3; i++) // trzykrotne powtórzenie bloku instrukcji
  {
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, SET); //  E = 1
  _LCD_OutNibble(0x03); // tryb 8-bitowy
  HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, RESET); // E = 0
  HAL_Delay(5); // czekaj 5ms
  }

HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, SET); // E = 1
_LCD_OutNibble(0x02); // tryb 4-bitowy
HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, RESET); // E = 0

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
