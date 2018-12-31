#include "Timer32_Driver.h"

void (*Timer32Callback)(void) = 0x00000000;

void Timer32_Open(void)
{
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
}

void Timer32_Start(uint32_t count)
{
    MAP_Timer32_setCount(TIMER32_0_BASE, count);
    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, true);
}

void Timer32_Stop(void)
{
    MAP_Timer32_haltTimer(TIMER32_0_BASE);
}

void Timer32_SetCallback(void(*callback)(void))
{
    Timer32Callback = callback;
}

void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);
    if(Timer32Callback)
    {
        Timer32Callback();
    }
}

