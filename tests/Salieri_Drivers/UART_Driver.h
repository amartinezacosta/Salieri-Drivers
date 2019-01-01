#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define UART_MODULE_COUNT       4
#define UART_RX_BUFFER_SIZE     128
#define UART_TX_BUFFER_SIZE     128

#define TX_BUFFER_FULL_MSG      0
#define RX_BUFFER_FULL_MSG      1
#define TX_BUFFER_EMPTY_MSG     2
#define RX_BUFFER_EMPTY_MSG     3

#define UART0   0
#define UART1   1
#define UART2   2
#define UART3   3

void UART_Open(uint32_t uart);
void UART_ChangeSettings(uint32_t uart, eUSCI_UART_Config *config);
void UART_SetCallback(uint32_t uart, void(*callback)(uint32_t));
uint32_t UART_Write(uint32_t uart, uint8_t *data, uint32_t size);
uint32_t UART_Read(uint32_t uart, uint8_t *data, uint32_t size);

#endif /* UART_DRIVER_H_ */
