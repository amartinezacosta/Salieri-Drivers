#include "ADC_Driver.h"

uint32_t ADC_MapPin[ADC_COUNT] = {GPIO_PIN5, GPIO_PIN7, GPIO_PIN6, GPIO_PIN0};
uint32_t ADC_MapInput[ADC_COUNT] = {ADC_INPUT_A8, ADC_INPUT_A6, ADC_INPUT_A7, ADC_INPUT_A13};
uint32_t ADC_MapMemory[ADC_COUNT] = {ADC_MEM8, ADC_MEM6, ADC_MEM7, ADC_MEM13};
uint32_t ADC_MapInterrupt[ADC_COUNT] = {ADC_INT8, ADC_INT6, ADC_INT7, ADC_INT13};
void (*ADC_Callbacks[ADC_COUNT])(uint16_t) = {0x00000000};

void ADC_Open(uint32_t adc)
{
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, ADC_MapPin[adc], GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

    MAP_ADC14_enableInterrupt(ADC_MapInterrupt[adc]);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
}

void ADC_Read(uint32_t adc)
{
    MAP_ADC14_disableConversion();
    MAP_ADC14_configureSingleSampleMode(ADC_MapMemory[adc], true);
    MAP_ADC14_configureConversionMemory(ADC_MapMemory[adc], ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_MapInput[adc], false);

    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

void ADC_SetCallback(uint32_t adc, void(*callback)(uint16_t))
{
    ADC_Callbacks[adc] = callback;
}

void ADC14_IRQHandler(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT8 & status)
    {
        if(ADC_Callbacks[0])
        {
            ADC_Callbacks[0](MAP_ADC14_getResult(ADC_MEM8));
        }
    }

    else if(ADC_INT6 & status)
    {
        if(ADC_Callbacks[1])
        {
            ADC_Callbacks[1](MAP_ADC14_getResult(ADC_MEM6));
        }
    }

    else if(ADC_INT7 & status)
    {
        if(ADC_Callbacks[2])
        {
            ADC_Callbacks[2](MAP_ADC14_getResult(ADC_MEM7));
        }
    }

    else if(ADC_INT13 & status)
    {
        if(ADC_Callbacks[3])
        {
            ADC_Callbacks[3](MAP_ADC14_getResult(ADC_MEM13));
        }
    }
}
