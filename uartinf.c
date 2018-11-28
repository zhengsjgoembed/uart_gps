#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
//#define DBG 1
//#include "config.h"
//#include "input_inf.h"
#include "serial.h"
#include "uartinf.h"

#define Dbg printf
#define MAX_CMD_SIZE 59

static int fds[UART_NUM];
#if 1
int uart_read(UART_INTF uart_type, char* str) 
{
	int fd = fds[uart_type];
    fd_set read_fds;
    struct timeval TimeoutVal;
    int ret;	

    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    TimeoutVal.tv_sec  = 0;
    TimeoutVal.tv_usec = 9*1500;

	ret = select(fd+1, &read_fds, NULL, NULL, &TimeoutVal);
	if(ret < 0) {
		perror("[select]");
	} else if (0 == ret) {
		//timeout
	} else {
		if(FD_ISSET(fd, &read_fds)) {
			ret = read(fd, str, MAX_CMD_SIZE);
			//ret = read(fd, str, 4);
			Dbg("----uart receive %d data\n", ret);
		}else
			ret = 0;
	}
	
	return ret;
}

int uart_get(UART_INTF uart_type, char* str) 
{
	int fd = fds[uart_type];
	fd_set read_fds;
	struct timeval TimeoutVal;
	int ret, retry_cnt = 0, size = 0;

retry:
	FD_ZERO(&read_fds);
	FD_SET(fd, &read_fds);
	
	TimeoutVal.tv_sec  = 0;
	TimeoutVal.tv_usec = 100*1000;

	ret = select(fd+1, &read_fds, NULL, NULL, &TimeoutVal);
	if(ret < 0) {
		perror("[select]");
		goto exit;
	} else if (0 == ret) {
		retry_cnt++;
		//printf("++\n");
		if(retry_cnt < 5)
			goto retry;
	} else {
		if(FD_ISSET(fd, &read_fds)) {
			ret = read(fd, str+size, MAX_CMD_SIZE);
			size += ret;

			//if(str[size-4] == 'O' && str[size-3] == 'K'){
			if(str[size-1] == '_'){
				str[size] = '\0';
				Dbg("----uart receive %d data: %s\n", size, str);
				return size;
			}else
				goto retry;
		}
	}
	
exit:
	return ret;
}

/*
int uart_get(UART_INTF uart_type, char* str)
{
        int fd = fds[uart_type];
        fd_set read_fds;
        struct timeval TimeoutVal;
        int ret, retry_cnt = 0, size = 0;

	do {
		//ret = read(fd, str+size, MAX_CMD_SIZE);
		ret = read(fd, str+size, 1);
		printf("++\n");
		if(ret>0)
                	size += ret;
		if(size>3000)
			return 0;
		if(ret <= 0)
			usleep(20000);
	} while(str[size-1] != '_');

	str[size] = '\0';
        Dbg("----uart receive %d data: %s\n", size, str);
	return size;
}*/
#endif
int uart_write(UART_INTF uart_type, char* str)
{
	int fd = fds[uart_type];
	int ret;

	//Dbg(("----uart%d write %s\n", uart_type, str));
	ret = write(fd, str, strlen(str));

	return ret;
}

int uart_init(UART_INTF uart_type, char *device, int speed, int parity)
{
	int fd;

    fd = open_dev(device);
    if (fd > 0) {
        set_Speed(fd, speed);
    } else {
        fprintf(stderr, "Error opening %s: %s\n", device, strerror(errno));
        return -1;
    }

    if (set_Parity(fd,8,1,parity) == 0) {
        fprintf(stderr, "Set Parity Error\n");
        close(fd);
        return -1;
    }

	fds[uart_type] = fd;

	return 1;
}

void uart_exit(UART_INTF uart_type)
{
	int fd = fds[uart_type];

	close_dev(fd);		
}
