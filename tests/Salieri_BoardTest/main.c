#include "UART_Driver.h"
#include "ADC_Driver.h"
#include "Timer32_Driver.h"
#include "PWM_Driver.h"
#include "ADC_Driver.h"
#include "GPIO_Driver.h"
#include "MSPIO.h"

void Timer32_Callback(void)
{
    GPIO_Write(GPIO_PORT_P3, GPIO_PIN5|GPIO_PIN7|GPIO_PIN0|GPIO_PIN6, HIGH);
    GPIO_Write(GPIO_PORT_P4, GPIO_PIN1|GPIO_PIN3|GPIO_PIN2|GPIO_PIN4, LOW);
    GPIO_Write(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5|GPIO_PIN0|GPIO_PIN2, HIGH);
    __delay_cycles(50);
    GPIO_Write(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5|GPIO_PIN0|GPIO_PIN2, LOW);

    ADC_Read(ADC0);

    Timer32_Start(4800000);
}


void ADC0_Callback(uint16_t sample)
{
    //Print ADC result, change PWM accordingly
    MSPrintf(EUSCI_A0_BASE, "ADC0 Value = %i\r\n", sample);
    PWM_SetDutyCycle(PWM0, sample);
}


void GPIO_Pin6Callback(void)
{
    //limit switch hit
    MSPrintf(EUSCI_A0_BASE, "GPIO_PIN6 hit\r\n");
}

void GPIO_Pin7Callback(void)
{
    //limit switch hit
    MSPrintf(EUSCI_A0_BASE, "GPIO_PIN7 hit\r\n");
}

void GPIO_Pin1Callback(void)
{
    //limit switch hit
    MSPrintf(EUSCI_A0_BASE, "GPIO_PIN1 hit\r\n");
}


void main(void)
{
    UART_Open(EUSCI_A0_BASE);

    //Thermistor
    ADC_Open(ADC0);
    ADC_SetCallback(ADC0, ADC0_Callback);

    //Stepper Motors
    GPIO_Open(GPIO_PORT_P4, GPIO_PIN1|GPIO_PIN3|GPIO_PIN2|GPIO_PIN4, true);
    GPIO_Open(GPIO_PORT_P5, GPIO_PIN4|GPIO_PIN5|GPIO_PIN0|GPIO_PIN2, true);
    GPIO_Open(GPIO_PORT_P3, GPIO_PIN5|GPIO_PIN7|GPIO_PIN0|GPIO_PIN6, true);

    //Limit Switches
    GPIO_Open(GPIO_PORT_P6, GPIO_PIN6|GPIO_PIN7|GPIO_PIN1, false);
    GPIO_SetCallback(GPIO_PORT_P6, GPIO_PIN6, GPIO_Pin6Callback);
    GPIO_SetCallback(GPIO_PORT_P6, GPIO_PIN7, GPIO_Pin7Callback);
    GPIO_SetCallback(GPIO_PORT_P6, GPIO_PIN1, GPIO_Pin1Callback);

    //Heater Cartridge, Heated bed and extra stuff
    PWM_Open(PWM0);
    PWM_Open(PWM1);
    PWM_Open(PWM2);
    PWM_Open(PWM3);

    //Timer for stepper motor
    Timer32_Open();
    Timer32_SetCallback(Timer32_Callback);


    MSPrintf(EUSCI_A0_BASE, "Salieri Drivers V0.1.0\r\nDesigned to test the Salieri board V0.2.0.\r\nStarting Salieri Board Test.\r\n\r\n\r\n");
    Timer32_Start(144000000);

	while(1)
	{
	}
}
