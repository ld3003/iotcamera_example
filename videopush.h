#ifndef vidoe_push_h__
#define vidoe_push_h__

#include "videopush.h"
#include "ts/Ts.h"
#include "ts/FileIo.h"
#include "ts/Mux.h"
#include "avenc.h"
#include "rtp/rtp.h"

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

class videopush
{
public:
    videopush();
    ~videopush();
    
    int start_rtp();
    int stop_rtp();

private:
    rtp *mrtp;
    struct TSPKG_SESSION *chn0_ts_session;
    struct TSPKG_SESSION *chn1_ts_session;
    
};

#endif
