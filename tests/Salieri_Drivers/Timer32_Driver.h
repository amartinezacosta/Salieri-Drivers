#ifndef TIMER32_DRIVER_H_
#define TIMER32_DRIVER_H_

#include "ti/devices/msp432p4xx/driverlib/driverlib.h"

void Timer32_Open(void);
void Timer32_Start(uint32_t count);
void Timer32_Stop(void);
void Timer32_SetCallback(void(*callback)(void));

#endif /* TIMER32_DRIVER_H_ */
