#include "UART_Driver.h"

void (*UART_Callbacks[UART_MODULE_COUNT])(uint32_t msg) = {0x00000000};

uint8_t UART_RXBuffer[UART_MODULE_COUNT][UART_RX_BUFFER_SIZE];
uint8_t UART_TXBuffer[UART_MODULE_COUNT][UART_TX_BUFFER_SIZE];

uint32_t UART_RXWriteIndex[UART_MODULE_COUNT] = {0};
uint32_t UART_RXReadIndex[UART_MODULE_COUNT] = {0};

uint32_t UART_TXWriteIndex[UART_MODULE_COUNT] = {0};
uint32_t UART_TXReadIndex[UART_MODULE_COUNT] = {0};

#define ADVANCE_TX_WRITE_INDEX(B)       UART_TXWriteIndex[B] = (UART_TXWriteIndex[B] + 1) % UART_TX_BUFFER_SIZE
#define ADVANCE_TX_READ_INDEX(B)        UART_TXReadIndex[B] = (UART_TXReadIndex[B] + 1) % UART_TX_BUFFER_SIZE
#define ADVANCE_RX_WRITE_INDEX(B)       UART_RXWriteIndex[B] = (UART_RXWriteIndex[B] + 1) % UART_RX_BUFFER_SIZE
#define ADVANCE_RX_READ_INDEX(B)        UART_RXReadIndex[B] = (UART_RXReadIndex[B] + 1) % UART_RX_BUFFER_SIZE
#define UART_TX_FULL(B)                 (UART_TXWriteIndex[B] + 1) % UART_TX_BUFFER_SIZE == UART_TXReadIndex[B] ? true : false
#define UART_TX_EMPTY(B)                UART_TXReadIndex[B] == UART_TXWriteIndex[B] ? true : false
#define UART_RX_FULL(B)                 (UART_RXWriteIndex[B] + 1) % UART_RX_BUFFER_SIZE == UART_RXReadIndex[B] ? true : false
#define UART_RX_EMPTY(B)                UART_RXReadIndex[B] == UART_RXWriteIndex[B] ? true : false

/*TODO: make this a local instead of a global?*/
/*Default baud rate is 115200 @ 48MHz*/
eUSCI_UART_Config UARTConfigs[UART_MODULE_COUNT] =
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     26,
     0,
     111,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};


uint32_t UART_ModuleMap(uint32_t uart)
{
    switch(uart)
    {
    case EUSCI_A0_BASE:
        return 0;
    case EUSCI_A1_BASE:
        return 1;
    case EUSCI_A2_BASE:
        return 2;
    case EUSCI_A3_BASE:
        return 3;
    default:
        return 0;
    }
}

uint32_t UART_ModuleInterruptMap(uint32_t uart)
{
    switch(uart)
    {
    case EUSCI_A0_BASE:
        return INT_EUSCIA0;
    case EUSCI_A1_BASE:
        return INT_EUSCIA1;
    case EUSCI_A2_BASE:
        return INT_EUSCIA2;
    case EUSCI_A3_BASE:
        return INT_EUSCIA3;
    default:
        return 0;
    }
}

uint32_t UART_ModuleMapPort(uint32_t uart)
{
    switch(uart)
    {
    case EUSCI_A0_BASE:
        return GPIO_PORT_P1;
    case EUSCI_A1_BASE:
        return GPIO_PORT_P2;
    case EUSCI_A2_BASE:
        return GPIO_PORT_P3;
    case EUSCI_A3_BASE:
        return GPIO_PORT_P9;
    default:
        return 0;
    }
}

uint8_t UART_ModuleMapPins(uint32_t uart)
{
    switch(uart)
    {
    case EUSCI_A0_BASE:
        return GPIO_PIN2 | GPIO_PIN3;
    case EUSCI_A1_BASE:
        return GPIO_PIN2 | GPIO_PIN3;
    case EUSCI_A2_BASE:
        return GPIO_PIN2 | GPIO_PIN3;
    case EUSCI_A3_BASE:
        return GPIO_PIN7 | GPIO_PIN6;
    default:
        return 0;
    }
}

void UART_Open(uint32_t uart)
{
    uint32_t index = UART_ModuleMap(uart);

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(UART_ModuleMapPort(uart), UART_ModuleMapPins(uart), GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_UART_initModule(uart, &UARTConfigs[index]);
    MAP_UART_enableModule(uart);
    MAP_UART_enableInterrupt(uart, EUSCI_A_UART_RECEIVE_INTERRUPT);
    MAP_Interrupt_enableInterrupt(UART_ModuleInterruptMap(uart));
}

void UART_ChangeSettings(uint32_t uart, eUSCI_UART_Config *config)
{
     uint32_t index = UART_ModuleMap(uart);
    UARTConfigs[index] = *config;

    MAP_UART_disableModule(uart);
    MAP_UART_initModule(uart, &UARTConfigs[index]);
    MAP_UART_enableModule(uart);
}

void UART_SetCallback(uint32_t uart, void(*callback)(uint32_t))
{
    uint32_t index = UART_ModuleMap(uart);

    UART_Callbacks[index] = callback;
}

uint32_t UART_Write(uint32_t uart, uint8_t *data, uint32_t size)
{
    uint32_t i;
    uint32_t module = UART_ModuleMap(uart);

    if(size > UART_TX_BUFFER_SIZE)
    {
        size = UART_TX_BUFFER_SIZE;
    }

    for(i = 0; i < size; i++)
    {
        if(UART_TX_FULL(module))
        {
            break;
        }

        UART_TXBuffer[module][UART_TXWriteIndex[module]] = data[i];
        ADVANCE_TX_WRITE_INDEX(module);
    }

    MAP_UART_enableInterrupt(uart, EUSCI_A_UART_TRANSMIT_INTERRUPT);

    return i;
}

uint32_t UART_Read(uint32_t uart, uint8_t *data, uint32_t size)
{
    uint32_t i;
    uint32_t module = UART_ModuleMap(uart);

    if(size > UART_RX_BUFFER_SIZE)
    {
        size = UART_RX_BUFFER_SIZE;
    }

    for(i = 0; i < size; i++)
    {
        if(UART_RX_EMPTY(module))
        {
            break;
        }

        data[i] = UART_RXBuffer[module][UART_RXReadIndex[module]];
        ADVANCE_RX_READ_INDEX(module);
    }

    return i;
}

void EUSCIA0_IRQHandler(void)
{
    uint8_t c;
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    uint32_t rxWriteIndex = UART_RXWriteIndex[0];
    uint32_t txReadIndex = UART_TXReadIndex[0];

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        c = MAP_UART_receiveData(EUSCI_A0_BASE);

        if(UART_RX_FULL(0))
        {
            if(UART_Callbacks[0])
            {
                UART_Callbacks[0](RX_BUFFER_FULL_MSG);
            }
        }
        else
        {
            UART_RXBuffer[0][rxWriteIndex] = c;
            ADVANCE_RX_WRITE_INDEX(0);
        }
    }

    if(status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
    {
        if(UART_TX_EMPTY(0))
        {
            MAP_UART_disableInterrupt( EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
            if(UART_Callbacks[0])
            {
                UART_Callbacks[0](TX_BUFFER_EMPTY_MSG);
            }
        }
        else
        {
            MAP_UART_transmitData(EUSCI_A0_BASE, UART_TXBuffer[0][txReadIndex]);
            ADVANCE_TX_READ_INDEX(0);
        }
    }
}

void EUSCIA1_IRQHandler(void)
{
    uint8_t c;
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A1_BASE);
    uint32_t rxWriteIndex = UART_RXWriteIndex[1];
    uint32_t txReadIndex = UART_TXReadIndex[1];

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        c = MAP_UART_receiveData(EUSCI_A1_BASE);

        if(UART_RX_FULL(1))
        {
            if(UART_Callbacks[1])
            {
                UART_Callbacks[1](RX_BUFFER_FULL_MSG);
            }
        }
        else
        {
            UART_RXBuffer[1][rxWriteIndex] = c;
            ADVANCE_RX_WRITE_INDEX(1);
        }
    }

    if(status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
    {
        if(UART_TX_EMPTY(1))
        {
            MAP_UART_disableInterrupt( EUSCI_A1_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
            if(UART_Callbacks[1])
            {
                UART_Callbacks[1](TX_BUFFER_EMPTY_MSG);
            }
        }
        else
        {
            MAP_UART_transmitData(EUSCI_A0_BASE, UART_TXBuffer[1][txReadIndex]);
            ADVANCE_TX_READ_INDEX(1);
        }
    }

}

void EUSCIA2_IRQHandler(void)
{
    uint8_t c;
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    uint32_t rxWriteIndex = UART_RXWriteIndex[2];
    uint32_t txReadIndex = UART_TXReadIndex[2];

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        c = MAP_UART_receiveData(EUSCI_A2_BASE);

        if(UART_RX_FULL(2))
        {
            if(UART_Callbacks[2])
            {
                UART_Callbacks[2](RX_BUFFER_FULL_MSG);
            }
        }
        else
        {
            UART_RXBuffer[2][rxWriteIndex] = c;
            ADVANCE_RX_WRITE_INDEX(2);
        }
    }

    if(status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
    {
        if(UART_TX_EMPTY(2))
        {
            MAP_UART_disableInterrupt( EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
            if(UART_Callbacks[2])
            {
                UART_Callbacks[2](TX_BUFFER_EMPTY_MSG);
            }
        }
        else
        {
            MAP_UART_transmitData(EUSCI_A2_BASE, UART_TXBuffer[2][txReadIndex]);
            ADVANCE_TX_READ_INDEX(2);
        }
    }

}

void EUSCIA3_IRQHandler(void)
{
    uint8_t c;
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A3_BASE);
    uint32_t rxWriteIndex = UART_RXWriteIndex[3];
    uint32_t txReadIndex = UART_TXReadIndex[3];

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        c = MAP_UART_receiveData(EUSCI_A3_BASE);

        if(UART_RX_FULL(3))
        {
            if(UART_Callbacks[3])
            {
                UART_Callbacks[3](RX_BUFFER_FULL_MSG);
            }
        }
        else
        {
            UART_RXBuffer[3][rxWriteIndex] = c;
            ADVANCE_RX_WRITE_INDEX(3);
        }
    }

    if(status & EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)
    {
        if(UART_TX_EMPTY(3))
        {
            MAP_UART_disableInterrupt( EUSCI_A3_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT);
            if(UART_Callbacks[3])
            {
                UART_Callbacks[3](TX_BUFFER_EMPTY_MSG);
            }
        }
        else
        {
            MAP_UART_transmitData(EUSCI_A0_BASE, UART_TXBuffer[0][txReadIndex]);
            ADVANCE_TX_READ_INDEX(3);
        }
    }

}


