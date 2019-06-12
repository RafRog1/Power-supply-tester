#ifndef SUPPLY_TESTER_H_
#define SUPPLY_TESTER_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define DEFAULT_CORRECTION_RESISTANCE 1.0

#define RELAY_1 80.0
#define RELAY_1_2 26.7
#define RELAY_1_2_3 11.4
#define RELAY_1_2_3_4 5.3

enum actualRelayWork{
	none,
	relay1, //80ohm
	relay1and2, //26,7ohm
	relay1and2and3, //11,4ohm
	relay1and2and3and4, //5,3ohm

	relayOut
};

struct supplyTester_Struct{
	float correctionResistance;
	float onResistance;
	float actualResistance;

	enum actualRelayWork relayWork;
	enum actualRelayWork relayToSet;
};

void supplyTesterInitStruct(void);
void increaseCorrectionResistance(void);
char* getCorrectionResistanceString(void);
void increaseRelayToSet(void);
void acceptSetRelay(void);
void resetSetRelay(void);
uint8_t setRelayAndActualWorkIsEqual(void);
char *getRelayOn(void);
char* getTemperatureSensorString(void);

#ifdef __cplusplus
}
#endif
#endif /*SUPPLY_TESTER_H_ */

