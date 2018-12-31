#include "ADC_Driver.h"

uint16_t adc0;
uint16_t adc1;

void ADC0_Callback(uint16_t sample)
{
    adc0 = sample;
}

void ADC1_Callback(uint16_t sample)
{
    adc1 = sample;
}


void main(void)
{
    MAP_WDT_A_holdTimer();

    ADC_Open(ADC0);
    ADC_SetCallback(ADC0, ADC0_Callback);

    ADC_Open(ADC1);
    ADC_SetCallback(ADC1, ADC1_Callback);

    while(1)
    {
        ADC_Read(ADC0);
        __delay_cycles(12000000);

        ADC_Read(ADC1);
        __delay_cycles(12000000);
    }
}
