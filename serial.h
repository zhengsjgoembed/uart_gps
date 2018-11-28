#ifndef __SERIAL_H__
#define __SERIAL_H__

int open_dev(char *Dev);
void close_dev(int fd);
void set_Speed(int fd, int speed);
int set_Parity(int fd, int databits, int stopbits, int parity);

#endif
