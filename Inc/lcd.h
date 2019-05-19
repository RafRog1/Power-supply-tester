#ifndef __lcd_H
#define __lcd_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

void LCD_double(uint32_t analogRead);
void printCurrentView(void);

#ifdef __cplusplus
}
#endif
#endif /*__lcd_H */

