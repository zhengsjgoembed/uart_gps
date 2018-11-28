#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>   //      gettimeofday
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "serial.h"
#include "uartinf.h"
#include "confile.h"
#include "udp_util.h"

#define APP_VERSION "v3.0"


int main (int argc, char *argv[])
{
    int n_read;
    unsigned char _rx_data[60];
    n_read = uart_init(ROUTE_UART, "/dev/ttyPS1", 57600, 'N');
    if(n_read <=0 ) {
	printf("open /dev/ttyPS1 failed\n");
	return -1;
    }

    while(1) {
		n_read = uart_read(ROUTE_UART, _rx_data);
		if(n_read > 0) {
		
		}

	}
}
