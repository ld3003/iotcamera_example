#ifndef Thermal_VIDEODEVICE_H
#define Thermal_VIDEODEVICE_H

#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>

#define T_SIZE 5
#define K_SIZE 1

#define MY_VID (0x0bda)
#define MY_PID (0x3901)
typedef void(therma_callback)(struct Thermal_FrameFormat *frameImg, struct Thermal_FrameFormat *frameTemperature, void *);

/*输出图像像素格式
*考虑到如果输出温度数据，一个温度数据需要占用2字节，因此暂时不支持Format_RGB24
*Format_BGR24这种一个像素占用奇数字节的像素格式
*/
enum Thermal_PixelFormat
{
    Format_RGB24 = 0, //RGB24位像素格式，其中B在低字节
    Format_BGR24,     //BGR24位像素格式，其中R在低字节
    Format_RGB32,     //RGB32位像素格式，其中B在低字节
    Format_BGR32,     //BGR24位像素格式，其中R在低字节
    Format_Y16
};
/*输出图像帧格式
*
*/
struct Thermal_FrameFormat
{
    enum Thermal_PixelFormat pixelFmt; //输出图像像素格式
    unsigned int width;                //图像宽度
    unsigned int height;               //图像高度
    unsigned int bytesPerLine;         //图像每行数据占用字节数，需要注意已经进行了4字节对齐
    void *ptr;                         //图像数据指针
};
/*
*伪彩处理模式
*/
enum Thermal_PseudoColorMode
{
    AdaptiveThreshold_1 = 0 //自适应伪彩处理1
};
/*
*温度单位
*/
enum Thermal_TemperatureMode
{
    KelvinMode = 0, //国际开尔文温度k
    CentigradeMode, //摄氏度C
    FahrenheitMode  //华氏度F
};
enum Thermal_RotateMode
{
    Rotate_0 = 0,
    Rotate_90,
    Rotate_180,
    Rotate_270
};

typedef struct uvc_frame
{
    /** Image data for this frame */
    void *data;
    /** Size of image data buffer */
    size_t data_bytes;
    /** Width of image in pixels */
    uint32_t width;
    /** Height of image in pixels */
    uint32_t height;
    /** Pixel data format */
    //enum uvc_frame_format frame_format;
    /** Number of bytes per horizontal line (undefined for compressed format) */
    size_t step;
    /** Frame number (may skip, but is strictly monotonically increasing) */
    uint32_t sequence;
    /** Estimate of system time when the device started capturing the image */
    struct timeval capture_time;
    /** Estimate of system time when the device finished receiving the image */
    struct timespec capture_time_finished;
    /** Handle on the device that produced the image.
   * @warning You must not call any uvc_* functions during a callback. */
    //uvc_device_handle_t *source;
    /** Is the data buffer owned by the library?
   * If 1, the data buffer can be arbitrarily reallocated by frame conversion
   * functions.
   * If 0, the data buffer will not be reallocated or freed by the library.
   * Set this field to zero if you are supplying the buffer.
   */
    uint8_t library_owns_data;
    /** Metadata for this frame if available */
    void *metadata;
    /** Size of metadata buffer */
    size_t metadata_bytes;
} uvc_frame_t;

class thermalUsb
{
public:
    thermalUsb();

    bool open();
    bool openFlag;
    bool close();
    bool start(therma_callback *cb, void *ptr); //开启视频，cb为回调函数
    bool stop();
    bool setPixelFormat(enum Thermal_PixelFormat fmt);
    enum Thermal_PixelFormat getPixelFormat();
    void setPseudoColorMode(enum Thermal_PseudoColorMode mode);
    enum Thermal_PseudoColorMode getPseudoColorMode();
    void setTemperatureMode(enum Thermal_TemperatureMode mode);
    enum Thermal_TemperatureMode getTemperatureMode();
    ~thermalUsb();
    /*
    *函数名：   kbcSetAutoMode
    *函数功能： 设置自动挡片模式
    *参数说明： mode自动挡片模式，true表示是自动模式，false表示手动模式
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int kbcSetAutoMode(bool mode);
    /*
    *函数名：   KbcGetAutoMode
    *函数功能： 获取自动挡片模式
    *参数说明： mode得到自动挡片模式，true表示是自动模式，false表示手动模式
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcGetAutoMode(bool &mode);
    /*
    *函数名：   KbcSetAutoMin
    *函数功能： 设置自动挡片最小时间间隔，单位为s
    *参数说明： min最大自动挡片时间间隔
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcSetAutoMin(uint8_t min);
    /*
    *函数名：   KbcGetAutoMin
    *函数功能： 获取自动挡片最小时间间隔，单位为s
    *参数说明： min最大自动挡片时间间隔
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcGetAutoMin(uint8_t &min);
    /*
    *函数名：   KbcSetAutoMax
    *函数功能： 设置自动挡片最大时间间隔，单位为s
    *参数说明： max最大自动挡片时间间隔
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcSetAutoMax(uint8_t max);
    /*
    *函数名：   KbcGetAutoMax
    *函数功能： 获取自动挡片最大时间间隔，单位为s
    *参数说明： max最大自动挡片时间间隔
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcGetAutoMax(uint8_t &max);
    /*
    *函数名：   KbcSetManual
    *函数功能： 设置为手动挡片
    *参数说明： 无
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcSetManual();
    /*
    *函数名：   KbcSetReset
    *函数功能： 重置挡片设置为默认设置
    *参数说明： 无
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int KbcSetReset();

    /*
    *函数名：   rmvcSetEnable
    *函数功能： 设置锅盖校正使能
    *参数说明： en true使能锅盖校正，false禁用锅盖校正
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int rmvcSetEnable(bool en);

    /*
    *函数名：   rmvcStartCali
    *函数功能： 开始锅盖校准
    *参数说明： 没有参数
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int rmvcStartCali();

    /*
    *函数名：   rmvcReset
    *函数功能： 复位锅盖校准
    *参数说明： 没有参数
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int rmvcReset();

    /*
    *函数名：   tpdSetEnable
    *函数功能： 设置温度测量使能
    *参数说明： en true使能温度测量，false禁用温度测量 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetEnable(bool en);

    /*
    *函数名：   tpdSetEnvTargetEmissivity
    *函数功能： 设置目标反射率，使用7位定点数
    *参数说明： r 7位定点数，0~128 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetEnvTargetEmissivity(uint8_t r);

    /*
    *函数名：   tpdSetEnvAtmosphericTransmittance
    *函数功能： 设置大气透过率，使用7位定点数
    *参数说明： t 7位定点数，0~128 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetEnvAtmosphericTransmittance(uint8_t t);

    /*
    *函数名：   tpdSetEnvAtmosphericTemperature
    *函数功能： 设置大气温度
    *参数说明： t单位K,温度范围250~500 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetEnvAtmosphericTemperature(uint16_t t);

    /*
    *函数名：   tpdSetEnvTemperature
    *函数功能： 设置环境温度
    *参数说明： t单位K,温度范围250~500 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetEnvTemperature(uint16_t t);

    /*
    *函数名：   tpdEnvSet
    *函数功能： 获取环境修正参数
    *参数说明： para距离修正参数索引
                    0:目标发射率 
                    1:大气透过率 
                    2:大气温度 
                    3:环境温度 
                val环境修正参数
                    目标发射率：7位定点数,取值范围0~128
                    大气透过率：7位定点数,取值范围0~128
                    大气温度：单位为开尔文K,取值范围250~500
                    环境温度：单位为开尔文K,取值范围250~500
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdEnvSet(uint8_t para, uint16_t val);

    /*
    *函数名：   tpdEnvGet
    *函数功能： 获取环境修正参数
    *参数说明： para距离修正参数索引
                    0:目标发射率 
                    1:大气透过率 
                    2:大气温度 
                    3:环境温度 
                val环境修正参数
                    目标发射率：7位定点数
                    大气透过率：7位定点数
                    大气温度：单位为开尔文K
                    环境温度：单位为开尔文K
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdEnvGet(uint8_t para, uint16_t &val);

    /*
    *函数名：   tpdDisSet
    *函数功能： 设置距离修正参数
    *参数说明： para距离修正参数索引
                    0:修正系数A0 
                    1:修正系数B0 
                    2:修正系数C0 
                    3:修正系数A1 
                    4:修正系数B1 
                    5:修正系数C1
                    6:目标距离参数
                val距离修正参数
                    A0、A1：使用31位定点数
                    B0、B1：使用24位定点数
                    C0：使用11位定点数
                    C1：使用5位定点数
                    目标距离参数:距离单位为m,使用7位定点数,取值范围0~2048，对应0~16m
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdDisSet(uint8_t para, int32_t val);

    /*
    *函数名：   tpdDisGet
    *函数功能： 获取距离修正参数
    *参数说明： para距离修正参数索引
                    6:目标距离参数
                val距离修正参数
                    目标距离参数:距离单位为m,使用7位定点数,取值范围0~2048，对应0~16m
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdDisGet(uint8_t para, uint16_t &val);

    /*
    *函数名：   tpdDisSetEnable
    *函数功能： 设置距离校正使能
    *参数说明： en true使能距离校正，false禁用距离校正 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdDisSetEnable(bool en);

    /*
    *函数名：   tpdSetReset
    *函数功能： 复位温度校准参数
    *参数说明： index温度修正参数索引
                    0:温度计算参数重置 
                    1:环境修正参数重置 
                    2:距离修正参数重置
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetReset(uint8_t index);

    /*
    *函数名：   tpdSetTemperatureMode
    *函数功能： 设置高低温采集模式
    *参数说明： index温度修正参数索引
                    0:高温采集模式 
                    1:低温采集模式 
                t温度数值，单位为开尔文温度，取值范围250~500
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetTemperatureMode(uint8_t index, uint16_t t);

    /*
    *函数名：   tpdSetRecal
    *函数功能： 开始二次标定
    *参数说明： 无 
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int tpdSetRecal();

    /*
    *函数名：   irsnrSetMode
    *函数功能： 设置高低增益模式
    *参数说明： mode  true为高增益， false为低增益
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int irsnrSetMode(bool mode);

    /*
    *函数名：   irsnrGetMode
    *函数功能： 设置高低增益模式
    *参数说明： mode  true为高增益， false为低增益
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int irsnrGetMode(bool &mode);

    /*
    *函数名：   irsnrReset
    *函数功能： 重置用户配置，停止出图后发送，重新出图后生效
    *参数说明： 无
    *返回值说明：负数返回错误
                非负数正确返回
    */
    int irsnrReset();

    bool uvcOpen(int w, int h);
    void *uvc_cb(uvc_frame_t *frame, void *ptr, cv::Mat &outframe);

private:
#if 0
    static const int CONTROL_REQUEST_TYPE_IN = LIBUSB_ENDPOINT_IN |
        LIBUSB_REQUEST_TYPE_VENDOR |
        LIBUSB_RECIPIENT_INTERFACE;

    static const int CONTROL_REQUEST_TYPE_OUT = LIBUSB_ENDPOINT_OUT |
        LIBUSB_REQUEST_TYPE_VENDOR |
        LIBUSB_RECIPIENT_INTERFACE;
#endif

    typedef struct dev_cfg
    {
        int devs_cnt;
        unsigned int pid;
        unsigned int vid;
        std::string ser_num;
    } dev_cfg_t;

    typedef struct
    {
        std::string format;
        unsigned int width;
        unsigned int height;
        std::vector<unsigned int> fps;
    } CameraStreamInfo;

    enum Thermal_vendor_request
    {
        VREQ_GET_STATUS = 0x6,
        VREQ_REBOOT = 0x15,
        VREQ_RESET = 0x16,
        VREQ_I2C_RD = 0x17,
        VREQ_I2C_WR = 0x18,
        VREQ_GET_ALG_PARAM = 0x19,
        VREQ_SET_ALG_PARAM = 0x20,
        VREQ_DEBUG = 0x21,
        VREQ_I2C_RD_EXT = 0x22,
        VREQ_I2C_WR_EXT = 0x23,
    };
    enum UVC_byCmdType
    {
        KBC = 0x03,
        RMVC = 0x05,
        TPD = 0x06,
        IRSNR = 0x01
    };
    enum UVC_bySubCmd
    {
        KBC_B_CAL_SETTING = 0xc4,
        KBC_B_CAL = 0x45,
        KBC_RESET = 0x49,
        RMVC_BYPASS = 0x41,
        RMVC_CAL = 0x42,
        RMVC_RESET = 0x43,
        TPD_BYPASS = 0x141,
        TPD_ENV_PARA_SET = 0xcd,
        TPD_DIS_PARA_SET = 0xc8,
        TPD_DIS_CORR_EN = 0x4a,
        TPD_TEMP_CAL_RESET = 0x149,
        TPD_ENV_CAL_RESET = 0x50,
        TPD_DIS_CAL_RESET = 0x51,
        TDP_CAL_HIGH_TEMP = 0x44,
        TDP_CAL_LOW_TEMP = 0x45,
        TDP_NEW_RECAL = 0x46,
        SW_HIGH_FLAG = 0x142,
        SW_LOW_FLAG = 0x143,
        RESET_ALL = 0x149,

        KBC_B_CAL_SETTING_RD = 0x8a,
        TPD_ENV_PARA_GET = 0x8f,
        TPD_DIS_PARA_GET = 0x92,
        SNSR_HL_FLAG_RD = 0x18a
    };
    enum KBC_PARA
    {
        KBC_PARA0 = 0,
        KBC_PARA2,
        KBC_PARA3,
        KBC_Manual,
        KBC_Reset
    };
    enum TPD_ENV_PARA
    {
        TPD_ENV_TargetEmissivity = 0,
        TPD_ENV_AtmosphericTransmittance,
        TPD_ENV_AtmosphericTemperature,
        TPD_ENV_Temperature
    };
    enum TPD_DIS_PARA
    {
        TPD_DIS_PARA_A0 = 0,
        TPD_DIS_PARA_B0,
        TPD_DIS_PARA_C0,
        TPD_DIS_PARA_A1,
        TPD_DIS_PARA_B1,
        TPD_DIS_PARA_C1,
        TPD_DIS_PARA_TargetDist
    };

    std::vector<struct dev_cfg> devs_cfgList;
    std::vector<CameraStreamInfo> camera_stream_infoList;
    struct dev_cfg dev_cfg;
    uint8_t dev_idx;

    //    bool openFlag;
    bool startFlag;

    int uvcFrameWidth;
    int uvcFrameHeight;
    int uvcFrameFps;

    bool uvcClose();
    bool uvcStart();
    bool uvcStop();

    // enum Thermal_VideoOutMode videoOutMode;
    enum Thermal_PixelFormat pixelFmt;
    struct Thermal_FrameFormat frame[2];
    struct Thermal_FrameFormat temperatureRawFrame[2];
    struct Thermal_FrameFormat imgRawFrame[2];
    struct Thermal_FrameFormat temperatureFilterFrame[T_SIZE];
    struct Thermal_FrameFormat temperatureResultFrame[2];
    int frameCnt;

    unsigned char frameIndex;
    enum Thermal_PseudoColorMode pseudoColorMode;
    static void pseudoColor(thermalUsb *ptr);
    static void infraredFindMaxMin(thermalUsb *objPtr, uint16_t &ymax, uint16_t &ymin);
    static void pseudoColorAdaptiveThreshold_1(thermalUsb *ptr);
    static void processTemperature(thermalUsb *ptr);
    therma_callback *cb;
    void *cbParaPtr;

    Thermal_TemperatureMode temperatureMode;

    bool kbcAuto;
    uint8_t kbcMin;
    uint8_t kbcMax;
    int kbcSet(enum KBC_PARA para, uint8_t val);
    int kbcGet(enum KBC_PARA para, uint8_t &val);
    bool rmvcEnable;
    bool tpdEnable;

    bool tpdDistCorrEnable;
    bool irsnrGainFlag;
    Thermal_RotateMode rotateMode;
    bool myAllocBuffer();
    bool myFreeBuffer();
};

#endif // Thermal_VIDEODEVICE_H
