#ifndef __UARTINF_H__
#define __UARTINF_H__

typedef enum _UART_INTF
{
	CFG_UART,
	OSD_UART,
	ROUTE_UART,
	UART_NUM
}UART_INTF;

int uart_read(UART_INTF uart_type, char* str);
int uart_get(UART_INTF uart_type, char* str);
int uart_write(UART_INTF uart_type, char* str);
int uart_init(UART_INTF uart_type, char *device, int speed, int parity);
void uart_exit(UART_INTF uart_type);

#endif
