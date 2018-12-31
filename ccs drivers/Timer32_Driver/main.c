#include "Timer32_Driver.h"

void Timer32_Callback(void)
{
    Timer32_Start(4800000);
}

void main(void)
{
    Timer32_Open();
    Timer32_SetCallback(Timer32_Callback);

    //Callback after 1 second
    Timer32_Start(4800000);

    while(1)
    {
    }
}
