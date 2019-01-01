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
    UART_Open(UART0);
    UART_SetCallback(UART0, UART_A0Callback);

    char input[64];
    uint32_t read;

    while(1)
    {
        read = UART_Read(UART0, (uint8_t*)input, 64);

        if(read > 0)
        {
            UART_Write(UART0, (uint8_t*)input, read);
        }
    }
}
