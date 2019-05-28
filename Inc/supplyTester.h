#ifndef SUPPLY_TESTER_H_
#define SUPPLY_TESTER_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

#define DEFAULT_CORRECTION_RESISTANCE 1.0

struct supplyTester_Struct{
	float correctionResistance;
};

void supplyTesterInitStruct(void);
void increaseCorrectionResistance(void);
char* getCorrectionResistanceString(void);

#ifdef __cplusplus
}
#endif
#endif /*SUPPLY_TESTER_H_ */

