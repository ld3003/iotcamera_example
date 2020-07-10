#ifndef __IRCHN__
#define __IRCHN__

#include "CThread.h"

class IRCHANNEL : public CThread
{
public:
    IRCHANNEL(){};
    virtual ~IRCHANNEL(){};

    int run();

private:
    pthread_mutex_t irchn_mutex;
    char bExit; // 0 start 1 :stop 2 :exit
    unsigned char yuvbuf[320 * 240 * 4];
};

#endif