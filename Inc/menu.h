#ifndef __menu_H
#define __menu_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define MS_TO_ENABLE_EXTI 500

typedef enum {
	startMenu = 0x00,
	ADCMenu = 0x01,
	generatePWMMenu = 0x02,
	relayTestMenu = 0x03,

	menuMax
} programName;
typedef enum {
	startMenuView,

	ADCMenuView,
	ADCProgramSet,
	ADCProgramWork,

	generatePWMMenuView,

	relayTestMenuView,
	relayTestSet,
	relayTestWork,

	repeatThisView
} LCDView;

struct menuStruct{
	uint8_t enableExtiButton; //1
	programName chosenProgram; //startMenu
	LCDView actualLCDView; //startMenuView
	uint8_t aceptProgram; //0
	uint16_t msDisableExti; //0
};

void menuInit(void);
uint8_t extiButtonIsEnable(void);
uint8_t someProgramIsActive(void);
void disableExtiButton(void);
void leftButtonHandle(void);
void okButtonHandle(void);
void rightButtonHandle(void);
programName getCurrentChosenProgram(void);
void enableExtiButtonAfterTime(uint32_t timeToEnableMS);
LCDView getActualView(void);
void setActualView(LCDView view);


#ifdef __cplusplus
}
#endif
#endif /*__ menu_H */

