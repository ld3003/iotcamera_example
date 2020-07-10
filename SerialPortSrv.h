#ifndef __SerialPortSrv_h__
#define __SerialPortSrv_h__

#include "CThread.h"
#include "PCM/pcm.h"

class SerialPortU2 :public CThread
{
public:

	SerialPortU2();
	virtual ~SerialPortU2();
    int run();
    int writeData(unsigned char *data , int len);
    

private:

    int serial_read(int fd, char *p, int desire_get_len, int time_out_ms);
	int sfd;



};




#endif
