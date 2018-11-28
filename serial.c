#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <termios.h>
//#include "config.h"

static int speed_arr[] = {
    B921600, B460800, B230400, B115200, B57600, B38400, B19200,
    B9600, B4800, B2400, B1200, B300,
};

static int name_arr[] = {
    921600, 460800, 230400, 115200, 57600, 38400,  19200,
    9600,  4800,  2400,  1200,  300,
};

int open_dev(char *Dev)
{
    int fd = open(Dev, O_RDWR /*| O_NONBLOCK*/);         //| O_NOCTTY | O_NDELAY
    //int fd = open(Dev, O_RDWR | O_NONBLOCK);         //| O_NOCTTY | O_NDELAY
    //int fd = open(Dev, O_RDWR | O_NOCTTY | O_NDELAY/*| O_NONBLOCK*/);         //| O_NOCTTY | O_NDELAY

    return fd;
}


void close_dev(int fd)
{
	close(fd);
}

void set_Speed(int fd, int speed)
{
    int i;
    struct termios options;
    tcgetattr(fd, &options);

    for(i= 0; i<sizeof(speed_arr)/sizeof(int); i++) {
        if(speed == name_arr[i]) {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&options, speed_arr[i]);
            cfsetospeed(&options, speed_arr[i]);
            if( tcsetattr(fd, TCSANOW, &options) != 0)
                perror("tcsetattr fd1");
            return;
        }
        tcflush(fd,TCIOFLUSH);
    }

    if (i == sizeof(speed_arr)/sizeof(int)) {
        fprintf(stderr,"Sorry, please set the correct baud rate!\n\n");
    }
}

int set_Parity(int fd, int databits, int stopbits, int parity)
{
    struct termios options;

    if(tcgetattr(fd, &options) != 0) {
        perror("Getattr");
        //return(FALSE);
        return 0;
    }
    options.c_cflag &= ~CSIZE ;
    switch(databits) { /*设置数据位数*/
    case 7:
        options.c_cflag |= CS7;
        break;
    case 8:
        options.c_cflag |= CS8;
        break;
    default:
        fprintf(stderr,"Unsupported data size\n");
        //return (FALSE);
        return 0;
    }

    switch(parity) {
    case 'n':
    case 'N':
        options.c_cflag &= ~PARENB;   /* Clear parity enable */
        options.c_iflag &= ~INPCK;     /* Disable parity checking */
        break;
    case 'o':
    case 'O':
		options.c_cflag |= PARENB;	/* Enable parity */
        options.c_cflag |= PARODD;  /* 设置为奇效验*/
        options.c_iflag |= INPCK;	/* Enable parity checking */
        break;
    case 'e':
    case 'E':
        options.c_cflag |= PARENB;    /* Enable parity */
        options.c_cflag &= ~PARODD;   /* 转换为偶效验*/
        options.c_iflag |= INPCK;	  /* Enable parity checking */
        break;
    case 'S':
    case 's':  /*as no parity*/
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported parity\n");
        //return (FALSE);
        return 0;
    }
    /* 设置停止位*/
    switch (stopbits) {
    case 1:
        options.c_cflag &= ~CSTOPB;
        break;
    case 2:
        options.c_cflag |= CSTOPB;
        break;
    default:
        fprintf(stderr,"Unsupported stop bits\n");
        //return (FALSE);
        return 0;
    }
    /* Set input parity option */
    if (parity != 'n' || parity != 'N')
        options.c_iflag |= INPCK;
    //options.c_cc[VTIME] = 150; // 15 seconds
    options.c_cc[VTIME] = 5; // 15 seconds
    options.c_cc[VMIN] = 0;

    options.c_lflag &= ~(ECHO | ICANON);

    tcflush(fd,TCIFLUSH); /* Update the options and do it NOW */
    if (tcsetattr(fd,TCSANOW,&options) != 0) {
        perror("Setattr");
        //return (FALSE);
        return 0;
    }
    //return (TRUE);
    return 1;
}

