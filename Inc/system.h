#ifndef __systemuser_H
#define __systemuser_H
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

 void systemTimeHandle(void);
 uint32_t getSystemMSTime(void);

 void enableFastButton(void);
 void disableFastButton(void);
 uint8_t fastButtonIsEnable(void);


#ifdef __cplusplus
}
#endif
#endif /*__systemuser_H */

