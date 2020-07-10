#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "mqtt/mqtt.h"
#include "JSON/cJSON.h"
#include "PCM/pcm.h"
#include "avenc.h"
#include "ts/Mux.h"
#include "rtsp/rtsp.h"
#include "rtp/rtp.h"
#include "CRtmpPush.h"
//#include "audiodec.h"
#include "MQTTProcess.h"
#include "Common.h"
#include "globaldata.h"

#include "config.h"

#include "hi_include/mpi_venc.h"
#include "hi_include/hi_comm_video.h"
#include "hi_include/mpi_vb.h"
#include "hi_include/mpi_sys.h"
#include "hi_include/hi_math.h"
#include "hi_include/mpi_venc.h"
#include "hi_include/mpi_vpss.h"
#include "hi3516ev200_gpio.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>

#include "rtsp_server.h"

static int writetsdata(unsigned char *data, int len)
{
	//printf("send pack\n");
	mrtp->sendPack((unsigned char *)data, len);
}

static int rtmp_write_a(unsigned char *buf, int len, int pts)
{
	//getBandwidth(len);
	//rtmp.writeAACRawData(buf, len, pts);
	return 0;
}
static int rtmp_write_v(unsigned char *buf, int len, int pts)
{
	RUN_TEST;
	getBandwidth(len);
	rtmp.writeH264RawData(buf, len, pts);
	return 0;
}
static int rtpts_write_a(unsigned char *buf, int len, int pts)
{
	//RUN_TEST;
	//AUDIO_SAMPLE_RATE_24000
	//rtmp.writeAACRawData(buf,len,pts);
	getBandwidth(len);
	WriteBuf2TsFile(&ts_session, 2400, pts, 0, buf, len, 0);
	return 0;
}

static int rtpts_write_v(unsigned char *buf, int len, int pts)
{
	//printf("################\n");
	//RUN_TEST;
	getBandwidth(len);
	WriteBuf2TsFile(&ts_session, 25, pts, 1, buf, len, 0);
	return 0;
}

static int all_write_v(unsigned char *buf, int len, int pts)
{
	return 0;
}
static int all_write_a(unsigned char *buf, int len, int pts)
{
	return 0;
}

static int rtsp_write_v(unsigned char *buf, int len, int pts)
{
	rtspSrv->rtspsendFrame(buf,len);
	return 0;
}
static int rtsp_write_a(unsigned char *buf, int len, int pts)
{
	return 0;
}

HI_S32 GetFrameBlkInfo(VIDEO_FRAME_INFO_S *stVideoFrameInfo);

int main(int argc, char **argv)
{

	int ret;
	struct MEDIA_WRITE mwrtp, mwrtmp, mwrtsp;
	//创建异步操作
	create_consumer();

	//读取配置文件
	readcfg();

	//启动串口2
	spu2 = new SerialPortU2();
	spu2->start();

	//启动网络管理
	nmgr = new NetWorkMgr();
	//nmgr->start();

	//创建ir通道
	irchn = new IRCHANNEL();

	//初始化TS会话
	memset(&ts_session, 0x0, sizeof(ts_session));
	ts_session.writets = writetsdata;

	//启动MQTT
	printf("start mqtt\n");
	set_mqtt_clientid(DEVID);
	set_p2psignal_callback(mqttArrived_Fun);

	//初始化音视频编码
	initavenc();

#if 0
	mwrtmp.write_a = rtmp_write_a;
	mwrtmp.write_v = rtmp_write_v;
	mwrtmp.chn = 1;
	register_mw(&mwrtmp);
#endif

#if 0
	mwrtp.write_a = rtpts_write_a;
	mwrtp.write_v = rtpts_write_v;
	mwrtp.chn = 1;
	register_mw(&mwrtp);
#endif

#if 1
	mwrtsp.write_a = rtsp_write_a;
	mwrtsp.write_v = rtsp_write_v;
	mwrtsp.chn = 2;
	register_mw(&mwrtsp);
#endif

	//启动rtmp
	rtmp.setUrl("rtmp://easy-iot.cc:1935/live/livestream");
	//rtmp.start();

	RUN_TEST;
	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

#if 0
	for (;;)
	{
		rtmp.startPush();
		av_init();
		av_start();
		sleep(5);
		av_ctrl(0);
		rtmp.stopPush();
		sleep(5);
	}

	for (;;)
	{
		rtmp.startPush();
		sleep(5);
		rtmp.stopPush();
		sleep(5);
	}

#endif

	//创建RTP会话
	mrtp = new rtp("47.104.166.126",
				   8900,
				   44100,
				   (double)(1.0 / 90000.0));

	mrtp->setPayloadtype(encode_mp2t);

	pause_write(1);
	av_start();
	irchn->start();

	rtspSrv = new RTSPServer();
	rtspSrv->start();

	pause_write(0);

#if 1

	while (1)
	{
#if 0
		mwrtmp.write_a = rtmp_write_a;
		mwrtmp.write_v = rtmp_write_v;
		mwrtmp.chn = 1;
		//register_mw(&mwrtmp);
		rtmp.startPush();
		pause_write(0);
		sleep(20);
		pause_write(1);
		rtmp.stopPush();

#endif

		mwrtmp.write_a = rtmp_write_a;
		mwrtmp.write_v = rtmp_write_v;
		mwrtmp.chn = 2;
		//register_mw(&mwrtmp);
		rtmp.startPush();
		pause_write(0);
		sleep(100000);
		pause_write(1);
		rtmp.stopPush();
	}
#endif

#if 0
	while (1)
	{
		rtmp.startPush();
		av_start();
		sleep(5);
		av_ctrl(0);
		rtmp.stopPush();
		sleep(5);


		mwrtmp.write_a = rtmp_write_a;
		mwrtmp.write_v = rtmp_write_v;
		mwrtmp.chn = 2;
		register_mw(&mwrtmp);


		rtmp.startPush();
		av_start();
		sleep(5);
		av_ctrl(0);
		rtmp.stopPush();
		sleep(5);
	}
#endif

#if 0
	while(1)
	{
		cv::VideoCapture capture(0);

        while (true){

#if 1
                cv::Mat cvframe;
				cv::Mat yuv420;
				cv::Mat yuv420_resize;
                capture >> cvframe;
                cvframe = cvframe(cv::Rect(0,cvframe.size().height/2,cvframe.size().width,cvframe.size().height/2));

				printf("width%d heigh%d type %d\n",cvframe.size().width,cvframe.size().height/2,cvframe.type());

				
				cv::cvtColor(cvframe, yuv420, CV_BGR2YUV);  
				resize(yuv420, yuv420_resize, cv::Size(320,240));

				SEND_TO_CHN2(yuv420_resize.data,0);

#endif

			

        }
	}
#endif

	//sleep(10000);

	for (;;)
	{
		mrtp->readloop();
	}

	return 0;
}
