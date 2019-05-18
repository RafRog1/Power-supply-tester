#ifndef __menu_H
#define __menu_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define MS_TO_ENABLE_EXTI 750

typedef enum {
	startMenu = 0x00,
	ADCMenu = 0x01,
	generatePWMMenu = 0x02,
	relayTestMenu = 0x03,

	menuMax
} menuName;

struct menuStruct{
	uint8_t enableExtiButton; //1
	menuName chosenProgram; //0
	uint8_t aceptProgram; //0
	uint16_t msDisableExti; //0
};

extern struct menuStruct menu;

void menuInit(void);
uint8_t extiButtonIsEnable(void);
uint8_t someProgramIsActive(void);
void disableExtiButton(void);
void leftButtonHandle(void);
void okButtonHandle(void);
void rightButtonHandle(void);
menuName getCurrentChosenProgram(void);
void enableExtiButtonAfterTime(uint32_t timeToEnableMS);


#ifdef __cplusplus
}
#endif
#endif /*__ menu_H */

