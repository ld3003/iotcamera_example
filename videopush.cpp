
#include "videopush.h"

videopush::videopush()
{

    chn0_ts_session = (struct TSPKG_SESSION *)malloc(sizeof(struct TSPKG_SESSION));
    chn1_ts_session = (struct TSPKG_SESSION *)malloc(sizeof(struct TSPKG_SESSION));
    assert(chn0_ts_session);
    assert(chn1_ts_session);
}
videopush::~videopush()
{
    free(chn0_ts_session);
    free(chn1_ts_session);
}

int videopush::start_rtp()
{
    mrtp = new rtp("192.168.1.182",
				   //mrtp = new rtp("192.168.1.182",
				   8900,
				   26000,
				   (double)(1.0 / 90000.0));

	mrtp->setPayloadtype(encode_mp2t);
}

int videopush::stop_rtp()
{
    delete mrtp;
}

//rtp *mrtp;
#if 0

void *vencode_thread(void *arg)
{
    SAMPLE_VENC_H265_H264();
}

void vp()
{
    
    pthread_t venctid;
    pVideo_Audio_Ts_File = OpenFile("./test.ts", "wb");

    init_venc();
    init_videopip();
    //mrtp = new rtp("47.104.166.126",
    mrtp = new rtp("192.168.2.206",
                   8900,
                   26000,
                   (double)(1.0 / 90000.0));

    mrtp->setPayloadtype(encode_mp2t);

    pthread_create(&venctid, NULL, vencode_thread, NULL);

    for (;;)
    {
        static unsigned long inc = 0;
        unsigned char tmpbuf[1024 * 512];
        ssize_t len = read(video_pipe_fd, tmpbuf, sizeof(tmpbuf));
        //mrtp->sendPack(tmpbuf,len,0);
        WriteBuf2TsFile(25, 1, tmpbuf, len, 0);
    }
    
}

#endif