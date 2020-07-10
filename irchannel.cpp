#include "irchannel.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>

#include "thermalUsb.h"
#include "v4l2dev.h"

#include "Common.h"
#include "avenc.h"

int IRCHANNEL::run()
{

    int w, h;
    cv::Mat frame, frame2, frame3;
    thermalUsb *tusb;
    uint8_t *frame_ptr;
    int frame_len;
    uvc_frame_t frameUvc;
    uint8_t *rgb565_ptr;
    void *p;

    v4l2dev_t camera = {
        -1,
    };
    unsigned int opt_camera_format = V4L2_PIX_FMT_NV12;

    w = 256;
    h = 384;

    v4l2_open(&camera, 0);
    v4l2_enum_fmt(&camera);
    v4l2_query_cap(&camera);
    v4l2_s_input(&camera, 0);
    v4l2_s_fmt(&camera, w, h, opt_camera_format);
    v4l2_enum_fmt22(&camera);
    v4l2_reqbufs(&camera, 4);
    v4l2_stream(&camera, 1);
    v4l2_g_fmt(&camera);

    tusb = new thermalUsb();

    tusb->uvcOpen(w, h);
    tusb->start(0, 0);

    while (1)
    {

        struct timeval rt1, rt2;
        time_consuming_start(&rt1, &rt2);

        v4l2_read_frame(&camera, (void **)(&frame_ptr), &frame_len);

        

        struct timeval t1, t2;
        time_consuming_start(&t1, &t2);
        if (frame_len <= 0)
        {
            continue;
        }
        else
        {
            frameUvc.data = (void *)frame_ptr;
            frameUvc.step = w * 2;
            frameUvc.width = w;
            frameUvc.height = h;

            //fetchcode:

            struct timeval t1, t2;
            time_consuming_start(&t1, &t2);
            p = tusb->uvc_cb(&frameUvc, 0, frame);

            //rgb32_to_i420(256,192,(char*)p,(char*)tmpimg);
            //__rgb24_to_yuv420p((unsigned char*)p,256,192,(unsigned char*)yuvbuf);

            if (!frame.empty())
            {

                cv::resize(frame, frame2, cv::Size(320, 240), 0, 0);

                //printf("frame. %d %d %d\n", frame.size().width, frame.size().height, frame.type());
                //printf("frame2. %d %d %d\n", frame2.size().width, frame2.size().height, frame2.type());
                //printf("frame3. %d %d %d\n", frame3.size().width, frame3.size().height, frame3.type());
                //rgb32_to_i420((unsigned char *)frame2.data, 320, 240, (unsigned char *)yuvbuf);
                //rgb32_to_i420(320, 240, (char *)frame2.data, (char *)yuvbuf);
                encodeYUV420sp(yuvbuf, frame2.data, 320, 240);
                //__rgb24_to_yuv420p((unsigned char *)frame2.data, 320, 240, (unsigned char *)yuvbuf);
                //cv::cvtColor(frame2, frame3, CV_RGB2YUV_I420);

                //if (!frame3.empty())
                //{
                //static unsigned char ttt[320*240*3/2];
                //memcpy(ttt,frame3.data,320*240*3/2);
                //RUN_TEST;
                //rgb32_to_i420(320,240,(char*)frame2.data,(char*)tmpimg);
                SEND_TO_CHN2(yuvbuf, 0);

                //goto fetchcode;
                //}
            }

            time_consuming_print("uvc_cb", &t1, &t2);
        }

        free(frame_ptr);

        time_consuming_print("readfrm", &rt1, &rt2);
    }

    delete tusb;
}