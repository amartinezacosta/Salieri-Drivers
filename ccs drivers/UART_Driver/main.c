#include "UART_Driver.h"

void UART_A0Callback(uint32_t msg)
{
    switch(msg)
    {
    case TX_BUFFER_FULL_MSG:
        break;
    case TX_BUFFER_EMPTY_MSG:
        break;
    case RX_BUFFER_FULL_MSG:
        break;
    case RX_BUFFER_EMPTY_MSG:
        break;
    }
}

void main(void)
{
    UART_Open(EUSCI_A0_BASE);
    UART_SetCallback(EUSCI_A0_BASE, UART_A0Callback);

    char input[64];
    uint32_t read;

    while(1)
    {
        read = UART_Read(EUSCI_A0_BASE, (uint8_t*)input, 64);

        if(read > 0)
        {
            UART_Write(EUSCI_A0_BASE, (uint8_t*)input, read);
        }
    }
}
