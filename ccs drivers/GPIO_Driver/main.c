#include "GPIO_Driver.h"

void Button0_Callback(void)
{
    GPIO_Toggle(GPIO_PORT_P2, GPIO_PIN0);
}


void main(void)
{
    MAP_WDT_A_holdTimer();

    GPIO_Open(GPIO_PORT_P1, GPIO_PIN1, INPUT);
    GPIO_Open(GPIO_PORT_P1, GPIO_PIN4, INPUT);
    GPIO_Open(GPIO_PORT_P2, GPIO_PIN0, OUTPUT);

    GPIO_SetCallback(GPIO_PORT_P1, GPIO_PIN1 | GPIO_PIN4, Button0_Callback);

    MAP_Interrupt_enableMaster();

	while(1)
	{

	}
}


