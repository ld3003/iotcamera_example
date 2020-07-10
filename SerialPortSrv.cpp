#include "SerialPortSrv.h"
#include "serialport.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

SerialPortU2::SerialPortU2()
{

    sfd = open_port("/dev/ttyAMA2");
    setup_port(sfd,115200,8,1,1);

}

SerialPortU2::~SerialPortU2()
{
    close_port(sfd);
}

int SerialPortU2::run()
{

    for(;;)
    {
        char read_temp[256];
        int len = serial_read(sfd,read_temp,sizeof(read_temp),2000);
        writeData((unsigned char *)read_temp,len);
    }

}

int SerialPortU2::writeData(unsigned char *data , int len)
{
    return write(sfd,data,len);
}

int SerialPortU2::serial_read(int fd, char *p, int desire_get_len, int time_out_ms)
{
    int nfds;
    int nread = 0;
    char read_temp[256];
    fd_set readfds;
    struct timeval tv;
    tv.tv_sec = time_out_ms / 1000;
    tv.tv_usec = time_out_ms % 1000;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    bzero(read_temp, sizeof(read_temp));
    nfds = select(fd + 1, &readfds, NULL, NULL, &tv);
    if (nfds == 0)
    {
        //printf("timeout!\r\n");
        return 0;
    }
    else
    {
        nread = read(fd, read_temp, desire_get_len); //即使不满desire_get_len，也会返回实际读取到的数据量
        if (nread <= 0)
        {
            printf("read get problem!\r\n");
        }
    }
    memcpy(p, read_temp, nread);
    return nread;
}