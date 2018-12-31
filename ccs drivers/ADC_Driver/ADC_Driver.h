#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include "ti/devices/msp432p4xx/driverlib/driverlib.h"

#define ADC_COUNT       4

#define ADC0    0       //P4.5
#define ADC1    1       //P4.7
#define ADC2    2       //P4.6
#define ADC3    3       //P4.0

void ADC_Open(uint32_t adc);
void ADC_Read(uint32_t adc);
void ADC_SetCallback(uint32_t adc, void(*callback)(uint16_t));

#endif /* ADC_DRIVER_H_ */
