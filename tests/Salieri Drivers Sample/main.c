#include "UART_Driver.h"
#include "ADC_Driver.h"
#include "Timer32_Driver.h"
#include "PWM_Driver.h"
#include "ADC_Driver.h"
#include "GPIO_Driver.h"

void main(void)
{
    UART_Open(EUSCI_A0_BASE);
    GPIO_Open(GPIO_PORT_P4, GPIO_PIN1|GPIO_PIN3|GPIO_PIN2|GPIO_PIN4, true);
    Timer32_Open();
    ADC_Open(ADC0);
    PWM_Open(PWM0);
    PWM_Open(PWM1);
    PWM_Open(PWM2);
    PWM_Open(PWM3);

    while(1)
    {

    }
}
