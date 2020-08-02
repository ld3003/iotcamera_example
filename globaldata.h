#ifndef __globaldata_h__
#define __globaldata_h__

#include "networkmgr.h"
#include "ts/Mux.h"
#include "rtsp/rtsp.h"
#include "rtp/rtp.h"
#include "CRtmpPush.h"
#include "SerialPortSrv.h"
#include "irchannel.h"
#include "rtsp_server.h"
#include "wifimgr.h"
extern struct TSPKG_SESSION ts_session;
extern rtp *mrtp;
extern CRtmpPush rtmp;
extern NetWorkMgr *nmgr;
extern WIFIMGR *wmgr;
extern SerialPortU2 *spu2;
extern IRCHANNEL *irchn;
extern RTSPServer *rtspSrv;

#endif