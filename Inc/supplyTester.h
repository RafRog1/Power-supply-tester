#ifndef SUPPLY_TESTER_H_
#define SUPPLY_TESTER_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define DEFAULT_CORRECTION_RESISTANCE 1.0

#define RESISTOR_1 (10.0)
#define RESISTOR_2 (20.0)
#define RESISTOR_3 (40.0)
#define RESISTOR_4 (80.0)

enum actualRelayWork{
	none,

	relay1,
	relay1and2,
	relay1and3,
	relay1and4,
	relay1and2and3,
	relay1and2and4,
	relay1and3and4,
	relay2,
	relay2and3,
	relay2and4,
	relay2and3and4,
	relay3,
	relay3and4,
	relay4,
	relay1and2and3and4,

	relayOut
};

enum supplyTesterStatus{
	onResistanceBlinkMode = 0x01,
};

struct supplyTester_Struct{
	float correctionResistance;
	float onResistance;
	float actualResistance;

	enum actualRelayWork relayWork;
	enum actualRelayWork relayToSet;

	uint8_t status;
};

void supplyTesterInitStruct(void);
void increaseCorrectionResistance(void);
char* getCorrectionResistanceString(void);
void increaseRelayToSet(void);
void acceptSetRelay(void);
void resetSetRelay(void);
uint8_t isBlinkMode(void);
char *getRelayOnBlink(void);
char *getRelayOn(void);
char *getTemperatureSensorString(void);
void disableRelays(void);
char *getCurrentString(void);

#ifdef __cplusplus
}
#endif
#endif /*SUPPLY_TESTER_H_ */

