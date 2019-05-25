#ifndef __RELAY_TEST_H
#define __RELAY_TEST_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

void relayTestProgramDeactivate(void);
void relayTestLoop(void);
char* checkRelayStatus(uint8_t relayNumber);
uint8_t relayTestViewIsChanged(void);


#ifdef __cplusplus
}
#endif
#endif /*__RELAY_TEST_H */

