#include "PWM_Driver.h"

void main(void)
{
    MAP_WDT_A_holdTimer();

    PWM_Open(PWM0);
    PWM_Open(PWM1);
    PWM_Open(PWM2);
    PWM_Open(PWM3);

    PWM_SetDutyCycle(PWM0, 4800);   //10% duty cycle
    PWM_SetDutyCycle(PWM1, 9600);   //20% duty cycle
    PWM_SetDutyCycle(PWM2, 14400);  //30% duty cycle
    PWM_SetDutyCycle(PWM3, 19200);  //40% duty cycle

    while(1)
    {
    }
}
