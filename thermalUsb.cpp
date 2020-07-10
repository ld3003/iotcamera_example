

#include "thermalUsb.h"
#include <stdio.h>
#include <unistd.h>
#include "Common.h"
#include <iostream>
using namespace std;

thermalUsb::thermalUsb()
{

    startFlag = false;
    openFlag = false;
    frameIndex = 0;
    frame[0].ptr = NULL;
    frame[1].ptr = NULL;
    frame[0].width = 0;
    frame[0].height = 0;
    frame[1].width = 0;
    frame[1].height = 0;

    temperatureRawFrame[0].ptr = NULL;
    temperatureRawFrame[1].ptr = NULL;
    temperatureRawFrame[0].width = 0;
    temperatureRawFrame[0].height = 0;
    temperatureRawFrame[1].width = 0;
    temperatureRawFrame[1].height = 0;

    imgRawFrame[0].ptr = NULL;
    imgRawFrame[1].ptr = NULL;
    imgRawFrame[0].width = 0;
    imgRawFrame[0].height = 0;
    imgRawFrame[1].width = 0;
    imgRawFrame[1].height = 0;

    for (int i = 0; i < T_SIZE; i++)
    {
        temperatureFilterFrame[i].ptr = NULL;
        temperatureFilterFrame[i].width = 0;
        temperatureFilterFrame[i].height = 0;
    }
    temperatureResultFrame[0].ptr = NULL;
    temperatureResultFrame[1].ptr = NULL;
    temperatureResultFrame[0].width = 0;
    temperatureResultFrame[0].height = 0;
    temperatureResultFrame[1].width = 0;
    temperatureResultFrame[1].height = 0;

    pixelFmt = Format_RGB24;
    pseudoColorMode = AdaptiveThreshold_1;

    cb = NULL;
    temperatureMode = CentigradeMode;
    rotateMode = Rotate_0;
    frameCnt = 0;
}
thermalUsb::~thermalUsb()
{
}

bool thermalUsb::uvcOpen(int w, int h)
{

    uvcFrameHeight = h;
    uvcFrameWidth = w;

    int width;
    int height;
    if ((rotateMode == Rotate_0) || (rotateMode == Rotate_180))
    {
        width = uvcFrameWidth;
        height = uvcFrameHeight / 2;
    }
    else if ((rotateMode == Rotate_90) || (rotateMode == Rotate_270))
    {
        width = uvcFrameHeight / 2;
        height = uvcFrameWidth;
    }
    for (int i = 0; i < 2; i++)
    {
        frame[i].width = width;
        frame[i].height = height;
        if ((pixelFmt == Format_RGB24) || (pixelFmt == Format_BGR24))
        {
            frame[i].bytesPerLine = ((frame[i].width * 3 + 3) / 4) * 4;
        }
        else if ((pixelFmt == Format_RGB32) || (pixelFmt == Format_BGR32))
        {
            frame[i].bytesPerLine = 4 * ((frame[i].width * 4 + 3) / 4);
        }
        else
        {
            cout << "Unsupport format!" << endl;
            return false;
        }
        frame[i].pixelFmt = pixelFmt;
        if (frame[i].ptr != NULL)
        {
            free(frame[i].ptr);
            frame[i].ptr = NULL;
        }

        temperatureRawFrame[i].width = width;
        temperatureRawFrame[i].height = height;
        temperatureRawFrame[i].bytesPerLine = ((temperatureRawFrame[i].width * 2 + 3) / 4) * 4;
        temperatureRawFrame[i].pixelFmt = Format_Y16;
        if (temperatureRawFrame[i].ptr != NULL)
        {
            free(temperatureRawFrame[i].ptr);
            temperatureRawFrame[i].ptr = NULL;
        }

        imgRawFrame[i].width = width;
        imgRawFrame[i].height = height;
        imgRawFrame[i].bytesPerLine = ((imgRawFrame[i].width * 2 + 3) / 4) * 4;
        imgRawFrame[i].pixelFmt = Format_Y16;
        if (imgRawFrame[i].ptr != NULL)
        {
            free(imgRawFrame[i].ptr);
            imgRawFrame[i].ptr = NULL;
        }

        temperatureResultFrame[i].width = width;
        temperatureResultFrame[i].height = height;
        temperatureResultFrame[i].bytesPerLine = ((temperatureResultFrame[i].width * 2 + 3) / 4) * 4;
        temperatureResultFrame[i].pixelFmt = Format_Y16;
        if (temperatureResultFrame[i].ptr != NULL)
        {
            free(temperatureResultFrame[i].ptr);
            temperatureResultFrame[i].ptr = NULL;
        }
    }
    for (int i = 0; i < T_SIZE; i++)
    {
        temperatureFilterFrame[i].width = width;
        temperatureFilterFrame[i].height = height;
        temperatureFilterFrame[i].bytesPerLine = ((temperatureFilterFrame[i].width * 2 + 3) / 4) * 4;
        temperatureFilterFrame[i].pixelFmt = Format_Y16;
        if (temperatureFilterFrame[i].ptr != NULL)
        {
            free(temperatureFilterFrame[i].ptr);
            temperatureFilterFrame[i].ptr = NULL;
        }
    }
    cout << "before myAllocBuffer" << endl;
    if (myAllocBuffer() == false)
    {
        cout << "Alloc  buffer failed!" << endl;
        return false;
    }
    openFlag = true;
    printf("thermalUsb::uvcOpen ok\n");

    return true;
}
bool thermalUsb::uvcClose()
{
    //uvc_error_t res;
    if (startFlag)
    {
        //uvcStop();
        startFlag = false;
    }
    if (openFlag)
    {
        //uvc_close(devh);
        /* Release the device descriptor */
        openFlag = false;
    }
#if 1
    for (int i = 0; i < 2; i++)
    {
        frame[i].width = 0;
        frame[i].height = 0;
        frame[i].bytesPerLine = 0;
        if (frame[i].ptr != NULL)
        {
            free(frame[i].ptr);
            frame[i].ptr = NULL;
        }

        temperatureRawFrame[i].width = 0;
        temperatureRawFrame[i].height = 0;
        temperatureRawFrame[i].bytesPerLine = 0;
        if (temperatureRawFrame[i].ptr != NULL)
        {
            free(temperatureRawFrame[i].ptr);
            temperatureRawFrame[i].ptr = NULL;
        }

        imgRawFrame[i].width = 0;
        imgRawFrame[i].height = 0;
        imgRawFrame[i].bytesPerLine = 0;
        if (imgRawFrame[i].ptr != NULL)
        {
            free(imgRawFrame[i].ptr);
            imgRawFrame[i].ptr = NULL;
        }

        temperatureResultFrame[i].width = 0;
        temperatureResultFrame[i].height = 0;
        temperatureResultFrame[i].bytesPerLine = 0;
        if (temperatureResultFrame[i].ptr != NULL)
        {
            free(imgRawFrame[i].ptr);
            temperatureResultFrame[i].ptr = NULL;
        }
    }

    for (int i = 0; i < T_SIZE; i++)
    {
        temperatureFilterFrame[i].width = 0;
        temperatureFilterFrame[i].height = 0;
        temperatureFilterFrame[i].bytesPerLine = 0;
        if (temperatureFilterFrame[i].ptr != NULL)
        {
            free(temperatureFilterFrame[i].ptr);
            temperatureFilterFrame[i].ptr = NULL;
        }
    }
    myFreeBuffer();
//uvc_unref_device(dev);
#endif

    return true;
}

bool thermalUsb::myAllocBuffer()
{
    bool flag = true;
    for (int i = 0; i < 2; i++)
    {
        if (frame[i].ptr != NULL)
        {
            free(frame[i].ptr);
            frame[i].ptr = NULL;
        }
        frame[i].ptr = malloc(frame[i].bytesPerLine * frame[i].height);
        if (frame[i].ptr == NULL)
        {
            flag = false;
            cout << "Malloc frame[" << i << "] buffer failed" << endl;
            break;
        }

        if (temperatureRawFrame[i].ptr != NULL)
        {
            free(temperatureRawFrame[i].ptr);
            temperatureRawFrame[i].ptr = NULL;
        }
        temperatureRawFrame[i].ptr = malloc(temperatureRawFrame[i].bytesPerLine * temperatureRawFrame[i].height);
        if (temperatureRawFrame[i].ptr == NULL)
        {
            flag = false;
            cout << "Malloc temperatureRawFrame[" << i << "] buffer failed" << endl;
            break;
        }

        if (imgRawFrame[i].ptr != NULL)
        {
            free(imgRawFrame[i].ptr);
            imgRawFrame[i].ptr = NULL;
        }
        imgRawFrame[i].ptr = malloc(imgRawFrame[i].bytesPerLine * imgRawFrame[i].height);
        if (imgRawFrame[i].ptr == NULL)
        {
            flag = false;
            cout << "Malloc imgRawFrame[" << i << "] buffer failed" << endl;
            break;
        }

        if (temperatureResultFrame[i].ptr != NULL)
        {
            free(temperatureResultFrame[i].ptr);
            temperatureResultFrame[i].ptr = NULL;
        }
        temperatureResultFrame[i].ptr = malloc(temperatureResultFrame[i].bytesPerLine * temperatureResultFrame[i].height);
        if (temperatureResultFrame[i].ptr == NULL)
        {
            flag = false;
            cout << "Malloc temperatureResultFrame[" << i << "] buffer failed" << endl;
            break;
        }
    }
    for (int i = 0; i < T_SIZE; i++)
    {
        if (temperatureFilterFrame[i].ptr != NULL)
        {
            free(temperatureFilterFrame[i].ptr);
            temperatureFilterFrame[i].ptr = NULL;
        }
        temperatureFilterFrame[i].ptr = malloc(temperatureFilterFrame[i].bytesPerLine * temperatureFilterFrame[i].height);
        if (temperatureFilterFrame[i].ptr == NULL)
        {
            flag = false;
            cout << "Malloc temperatureFilterFrame[" << i << "] buffer failed" << endl;
            break;
        }
    }
    if (flag)
        return flag;
    else
    {
        myFreeBuffer();
        return false;
    }
}
bool thermalUsb::myFreeBuffer()
{
    for (int i = 0; i < 2; i++)
    {
        if (frame[i].ptr != NULL)
        {
            free(frame[i].ptr);
            frame[i].ptr = NULL;
        }
        if (temperatureRawFrame[i].ptr != NULL)
        {
            free(temperatureRawFrame[i].ptr);
            temperatureRawFrame[i].ptr = NULL;
        }
        if (imgRawFrame[i].ptr != NULL)
        {
            free(imgRawFrame[i].ptr);
            imgRawFrame[i].ptr = NULL;
        }
        if (temperatureResultFrame[i].ptr != NULL)
        {
            free(temperatureResultFrame[i].ptr);
            temperatureResultFrame[i].ptr = NULL;
        }
    }
    for (int i = 0; i < T_SIZE; i++)
    {
        if (temperatureFilterFrame[i].ptr != NULL)
        {
            free(temperatureFilterFrame[i].ptr);
            temperatureFilterFrame[i].ptr = NULL;
        }
    }
    return true;
}
bool thermalUsb::uvcStart()
{
    return true;
}

void *thermalUsb::uvc_cb(uvc_frame_t *frameUvc, void *ptr, cv::Mat &outframe)
{

    uvc_frame_t *bgr;
    //uvc_error_t ret;

    thermalUsb *thisPtr = (thermalUsb *)this;
    if (thisPtr->frame[thisPtr->frameIndex].ptr == NULL)
        return 0;

    thisPtr->frame[thisPtr->frameIndex].pixelFmt = thisPtr->pixelFmt;
    if (thisPtr->rotateMode == Rotate_0)
    {

        for (int i = 0; i < frameUvc->height / 2; i++)
        {
            int16_t *ptrU16 = (int16_t *)(((uint8_t *)frameUvc->data) +
                                          frameUvc->step * i);
            int16_t *ptrU16OriTmp = (int16_t *)(((uint8_t *)frameUvc->data) +
                                                frameUvc->step * (i + frameUvc->height / 2));
            int16_t *ptrU16Temp = (int16_t *)(((uint8_t *)thisPtr->temperatureRawFrame[thisPtr->frameIndex].ptr) +
                                              thisPtr->temperatureRawFrame[thisPtr->frameIndex].bytesPerLine * i);
            int16_t *ptrU16Img = (int16_t *)(((uint8_t *)thisPtr->imgRawFrame[thisPtr->frameIndex].ptr) +
                                             thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine * i);

            for (int j = 0; j < frameUvc->width; j++)
            {
                ptrU16Img[j] = ptrU16[j];
                if (thisPtr->temperatureMode == KelvinMode)
                    ptrU16Temp[j] = ptrU16OriTmp[j];
                else if (thisPtr->temperatureMode == CentigradeMode)
                {
                    ptrU16Temp[j] = (int16_t)ptrU16OriTmp[j] - 4370;
                }
                else if (thisPtr->temperatureMode == FahrenheitMode)
                {
                    ptrU16Temp[j] = (int16_t)(ptrU16OriTmp[j] * 1.8 - 7354);
                }
            }
        }
    }
    else if (thisPtr->rotateMode == Rotate_180)
    {
        for (int i = 0; i < frameUvc->height / 2; i++)
        {
            int16_t *ptrU16 = (int16_t *)(((uint8_t *)frameUvc->data) +
                                          frameUvc->step * i);
            int16_t *ptrU16OriTmp = (int16_t *)(((uint8_t *)frameUvc->data) +
                                                frameUvc->step * (i + frameUvc->height / 2));
            int16_t *ptrU16Temp = (int16_t *)(((uint8_t *)thisPtr->temperatureRawFrame[thisPtr->frameIndex].ptr) +
                                              thisPtr->temperatureRawFrame[thisPtr->frameIndex].bytesPerLine *
                                                  (thisPtr->temperatureRawFrame[thisPtr->frameIndex].height - 1 - i));
            int16_t *ptrU16Img = (int16_t *)(((uint8_t *)thisPtr->imgRawFrame[thisPtr->frameIndex].ptr) +
                                             thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine * (thisPtr->imgRawFrame[thisPtr->frameIndex].height - 1 - i));
            for (int j = 0; j < frameUvc->width; j++)
            {
                ptrU16Img[thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - j] = ptrU16[j];
                if (thisPtr->temperatureMode == KelvinMode)
                    ptrU16Temp[thisPtr->temperatureRawFrame[thisPtr->frameIndex].width - 1 - j] = ptrU16OriTmp[j];
                else if (thisPtr->temperatureMode == CentigradeMode)
                {
                    ptrU16Temp[thisPtr->temperatureRawFrame[thisPtr->frameIndex].width - 1 - j] = (int16_t)ptrU16OriTmp[j] - 4370;
                }
                else if (thisPtr->temperatureMode == FahrenheitMode)
                {
                    ptrU16Temp[thisPtr->temperatureRawFrame[thisPtr->frameIndex].width - 1 - j] = (int16_t)(ptrU16OriTmp[j] * 1.8 - 7354);
                }
            }
        }
    }
    else if (thisPtr->rotateMode == Rotate_90)
    {
        int16_t *ptrU16Temp = (int16_t *)thisPtr->temperatureRawFrame[thisPtr->frameIndex].ptr;
        int16_t *ptrU16Img = (int16_t *)thisPtr->imgRawFrame[thisPtr->frameIndex].ptr;
        for (int i = 0; i < frameUvc->height / 2; i++)
        {
            int16_t *ptrU16 = (int16_t *)(((uint8_t *)frameUvc->data) +
                                          frameUvc->step * i);
            int16_t *ptrU16OriTmp = (int16_t *)(((uint8_t *)frameUvc->data) +
                                                frameUvc->step * (i + frameUvc->height / 2));

            for (int j = 0; j < frameUvc->width; j++)
            {
                ptrU16Img[j * thisPtr->temperatureRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                          thisPtr->temperatureRawFrame[thisPtr->frameIndex].width - 1 - i] = ptrU16[j];
                if (thisPtr->temperatureMode == KelvinMode)
                    ptrU16Temp[j * thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine +
                               thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - i] = ptrU16OriTmp[j];
                else if (thisPtr->temperatureMode == CentigradeMode)
                {
                    ptrU16Temp[j * thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                               thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - i] = (int16_t)ptrU16OriTmp[j] - 4370;
                }
                else if (thisPtr->temperatureMode == FahrenheitMode)
                {
                    ptrU16Temp[j * thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                               thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - i] = (int16_t)(ptrU16OriTmp[j] * 1.8 - 7354);
                }
            }
        }
    }
    else if (thisPtr->rotateMode == Rotate_270)
    {
        int16_t *ptrU16Temp = (int16_t *)thisPtr->temperatureRawFrame[thisPtr->frameIndex].ptr;
        int16_t *ptrU16Img = (int16_t *)thisPtr->imgRawFrame[thisPtr->frameIndex].ptr;
        for (int i = 0; i < frameUvc->height / 2; i++)
        {
            int16_t *ptrU16 = (int16_t *)(((uint8_t *)frameUvc->data) +
                                          frameUvc->step * i);
            int16_t *ptrU16OriTmp = (int16_t *)(((uint8_t *)frameUvc->data) +
                                                frameUvc->step * (i + frameUvc->height / 2));

            for (int j = 0; j < frameUvc->width; j++)
            {
                ptrU16Img[(thisPtr->temperatureRawFrame[thisPtr->frameIndex].height - 1 - j) * thisPtr->temperatureRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                          i] = ptrU16[j];
                if (thisPtr->temperatureMode == KelvinMode)
                    ptrU16Temp[(thisPtr->imgRawFrame[thisPtr->frameIndex].height - 1 - j) * thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                               thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - i] = ptrU16OriTmp[j];
                else if (thisPtr->temperatureMode == CentigradeMode)
                {
                    ptrU16Temp[(thisPtr->imgRawFrame[thisPtr->frameIndex].height - 1 - j) * thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                               thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - i] = (int16_t)ptrU16OriTmp[j] - 4370;
                }
                else if (thisPtr->temperatureMode == FahrenheitMode)
                {
                    ptrU16Temp[(thisPtr->imgRawFrame[thisPtr->frameIndex].height - 1 - j) * thisPtr->imgRawFrame[thisPtr->frameIndex].bytesPerLine / 2 +
                               thisPtr->imgRawFrame[thisPtr->frameIndex].width - 1 - i] = (int16_t)(ptrU16OriTmp[j] * 1.8 - 7354);
                }
            }
        }
    }

    thisPtr->pseudoColor(thisPtr);
    //thisPtr->processTemperature(thisPtr);

    //if(thisPtr->cb != NULL)

#if 0
        thisPtr->cb(&thisPtr->frame[thisPtr->frameIndex], 
            &thisPtr->temperatureResultFrame[thisPtr->frameIndex], thisPtr->cbParaPtr);
#endif
    //outframe
    void *p;
    struct Thermal_FrameFormat *frameImg = &thisPtr->frame[thisPtr->frameIndex];

    cv::Mat depthMat(frameImg->height, frameImg->width, CV_8UC3, (uchar *)frameImg->ptr, frameImg->bytesPerLine);
    depthMat.copyTo(outframe);

    p = frameImg->ptr;

    thisPtr->frameIndex = 1 - thisPtr->frameIndex;

    return p;
}

bool thermalUsb::open()
{
    return true;
}
bool thermalUsb::close()
{
    return uvcClose();
}
bool thermalUsb::start(therma_callback *cb, void *ptr)
{
    this->cb = cb;
    this->cbParaPtr = ptr;
    return true;
}
bool thermalUsb::stop()
{
    return true;
}

bool thermalUsb::setPixelFormat(enum Thermal_PixelFormat fmt)
{
    if (pixelFmt != fmt)
    {
        for (int i = 0; i < 2; i++)
        {
            if ((fmt == Format_RGB24) || (fmt == Format_BGR24))
            {
                frame[i].bytesPerLine = ((frame[i].width * 3 + 3) / 4) * 4;
                if (frame[i].ptr != NULL)
                {
                    free(frame[i].ptr);
                    frame[i].ptr = NULL;
                }
            }
            else if ((fmt == Format_RGB32) || (fmt == Format_BGR32))
            {
                frame[i].bytesPerLine = ((frame[i].width * 4 + 3) / 4) * 4;
                if (frame[i].ptr != NULL)
                {
                    free(frame[i].ptr);
                    frame[i].ptr = NULL;
                }
            }
            else
            {
                cout << "Unsupport format!" << endl;
                return false;
            }
            frame[i].ptr = malloc(frame[i].bytesPerLine * frame[i].height);
            if (frame[i].ptr == NULL)
            {
                cout << "Malloc frame[i].ptr[" << i << "] failed!" << endl;
                for (int j = 0; j < i; j++)
                {
                    if (frame[j].ptr != NULL)
                    {
                        free(frame[j].ptr);
                        frame[j].ptr = NULL;
                    }
                }
                return false;
            }
        }
        pixelFmt = fmt;
    }
    return true;
}
enum Thermal_PixelFormat thermalUsb::getPixelFormat()
{
    return pixelFmt;
}
void thermalUsb::setPseudoColorMode(enum Thermal_PseudoColorMode mode)
{
    pseudoColorMode = mode;
}
enum Thermal_PseudoColorMode thermalUsb::getPseudoColorMode()
{
    return pseudoColorMode;
}
void thermalUsb::infraredFindMaxMin(thermalUsb *objPtr, uint16_t &yMax, uint16_t &yMin)
{
    if (objPtr->imgRawFrame[objPtr->frameIndex].ptr != NULL)
    {
        uint16_t *ptr16 = (uint16_t *)((uint8_t *)
                                           objPtr->imgRawFrame[objPtr->frameIndex]
                                               .ptr);
        yMin = ptr16[0];
        yMax = ptr16[0];
        for (int i = 0; i < objPtr->imgRawFrame[objPtr->frameIndex].height; i++)
        {
            ptr16 = (uint16_t *)(((uint8_t *)
                                      objPtr->imgRawFrame[objPtr->frameIndex]
                                          .ptr) +
                                 i * objPtr->imgRawFrame[objPtr->frameIndex].bytesPerLine);
            for (int j = 0; j < objPtr->imgRawFrame[objPtr->frameIndex].width; j++)
            {
                if (yMin > ptr16[j])
                {
                    yMin = ptr16[j];
                }
                if (yMax < ptr16[j])
                {
                    yMax = ptr16[j];
                }
            }
        }
    }
}
void thermalUsb::pseudoColor(thermalUsb *objPtr)
{
    if (objPtr->frame[objPtr->frameIndex].ptr == NULL)
        return;
    if (objPtr->pseudoColorMode == AdaptiveThreshold_1)
    {
        pseudoColorAdaptiveThreshold_1(objPtr);
    }
    processTemperature(objPtr);
}
void thermalUsb::pseudoColorAdaptiveThreshold_1(thermalUsb *objPtr)
{
    uint16_t yMin;
    uint16_t yMax;
    uint16_t *ptr16;
    uint8_t *ptr8;
    uint16_t yImgL[3];
    infraredFindMaxMin(objPtr, yMax, yMin);

    yImgL[0] = yMin + (yMax - yMin) / 4;
    yImgL[1] = yMin + 2 * (yMax - yMin) / 4;
    yImgL[2] = yMin + 3 * (yMax - yMin) / 4;

    uint8_t r, g, b;

    if (objPtr->imgRawFrame[objPtr->frameIndex].ptr != NULL)
    {
        ptr16 = (uint16_t *)((uint8_t *)
                                 objPtr->imgRawFrame[objPtr->frameIndex]
                                     .ptr);
        yMin = ptr16[0];
        yMax = ptr16[0];
        for (int i = 0; i < objPtr->imgRawFrame[objPtr->frameIndex].height; i++)
        {
            ptr16 = (uint16_t *)(((uint8_t *)
                                      objPtr->imgRawFrame[objPtr->frameIndex]
                                          .ptr) +
                                 i * objPtr->imgRawFrame[objPtr->frameIndex].bytesPerLine);
            for (int j = 0; j < objPtr->imgRawFrame[objPtr->frameIndex].width; j++)
            {
                if (ptr16[j] <= yImgL[0])
                {
                    r = 0;
                    g = 0;
                    b = (uint8_t)((float)(ptr16[j] - yMin) / (yImgL[0] - yMin) * 255);
                }
                else if (ptr16[j] <= yImgL[1])
                {
                    r = 0;
                    g = (uint8_t)((float)(ptr16[j] - yImgL[0]) / (yImgL[1] - yImgL[0]) * 255);
                    b = 255 - (uint8_t)((float)(ptr16[j] - yImgL[0]) / (yImgL[1] - yImgL[0]) * 255);
                }
                else if (ptr16[j] <= yImgL[2])
                {
                    r = (uint8_t)((float)(ptr16[j] - yImgL[1]) / (yImgL[2] - yImgL[1]) * 255);
                    ;
                    g = 255;
                    b = 0;
                }
                else
                {
                    r = 255;
                    g = 255 - (uint8_t)((float)(ptr16[j] - yImgL[2]) / (yMax - yImgL[2]) * 255);
                    b = 0;
                }
                if ((objPtr->pixelFmt == Format_RGB32) || (objPtr->pixelFmt == Format_BGR32))
                {
                    ptr8 = (uint8_t *)objPtr->frame[objPtr->frameIndex].ptr + i * objPtr->frame[objPtr->frameIndex].bytesPerLine + j * 4;
                    if (objPtr->pixelFmt == Format_RGB32)
                    {
                        ptr8[0] = b;
                        ptr8[1] = g;
                        ptr8[2] = r;
                    }
                    else if (objPtr->pixelFmt == Format_BGR32)
                    {
                        ptr8[0] = r;
                        ptr8[1] = g;
                        ptr8[2] = b;
                    }
                }
                else if ((objPtr->pixelFmt == Format_RGB24) || (objPtr->pixelFmt == Format_BGR24))
                {
                    ptr8 = (uint8_t *)objPtr->frame[objPtr->frameIndex].ptr + i * objPtr->frame[objPtr->frameIndex].bytesPerLine + j * 3;
                    if (objPtr->pixelFmt == Format_RGB24)
                    {
                        ptr8[0] = b;
                        ptr8[1] = g;
                        ptr8[2] = r;
                    }
                    else if (objPtr->pixelFmt == Format_BGR24)
                    {
                        ptr8[0] = r;
                        ptr8[1] = g;
                        ptr8[2] = b;
                    }
                }
            }
        }
    }
}
void thermalUsb::processTemperature(
    thermalUsb *objPtr)
{
    int16_t *ptr16;
    int16_t *ptr16Dst;
    //暂时未实现旋转
    for (int i = K_SIZE; i < objPtr->temperatureRawFrame[objPtr->frameIndex].height - K_SIZE; i++)
    {
        ptr16Dst = (int16_t *)((uint8_t *)objPtr->temperatureFilterFrame[objPtr->frameCnt].ptr +
                               i * objPtr->temperatureFilterFrame[objPtr->frameCnt].bytesPerLine);
        for (int j = K_SIZE; j < objPtr->temperatureRawFrame[objPtr->frameIndex].width - K_SIZE; j++)
        {
            int sum = 0;
            for (int m = 0; m < (2 * K_SIZE + 1); m++)
            {
                ptr16 = (int16_t *)((uint8_t *)objPtr->temperatureRawFrame[objPtr->frameIndex].ptr +
                                    (i - K_SIZE + m) * objPtr->temperatureRawFrame[objPtr->frameIndex].bytesPerLine + (j - K_SIZE) * 2);
                for (int n = 0; n < (2 * K_SIZE + 1); n++)
                {
                    sum += ptr16[n];
                }
            }
            sum /= ((2 * K_SIZE + 1) * (2 * K_SIZE + 1));
            ptr16Dst[j] = (int16_t)sum;
        }
    }
    objPtr->frameCnt++;
    if (objPtr->frameCnt >= T_SIZE)
        objPtr->frameCnt = 0;

    for (int i = 0; i < objPtr->temperatureResultFrame[objPtr->frameIndex].height; i++)
    {
        ptr16Dst = (int16_t *)((uint8_t *)objPtr->temperatureResultFrame[objPtr->frameIndex].ptr +
                               i * objPtr->temperatureResultFrame[objPtr->frameIndex].bytesPerLine);
        for (int j = 0; j < objPtr->temperatureRawFrame[objPtr->frameIndex].width; j++)
        {
            int sum = 0;
            for (int m = 0; m < T_SIZE; m++)
            {
                ptr16 = (int16_t *)((uint8_t *)objPtr->temperatureFilterFrame[m].ptr +
                                    i * objPtr->temperatureFilterFrame[m].bytesPerLine);
                sum += ptr16[0];
            }
            sum /= T_SIZE;
            ptr16Dst[j] = sum;
        }
    }
}

void thermalUsb::setTemperatureMode(enum Thermal_TemperatureMode mode)
{
    temperatureMode = mode;
}
enum Thermal_TemperatureMode thermalUsb::getTemperatureMode()
{
    return temperatureMode;
}
int thermalUsb::kbcSet(enum KBC_PARA para, uint8_t val)
{
    int ret;
    return ret;
}
int thermalUsb::kbcGet(enum KBC_PARA para, uint8_t &val)
{

    return 0;
}

int thermalUsb::kbcSetAutoMode(bool mode)
{
    if (mode)
        return kbcSet(KBC_PARA0, 1);
    else
        return kbcSet(KBC_PARA0, 0);
}
int thermalUsb::KbcGetAutoMode(bool &mode)
{
    int ret;
    uint8_t tmp;
    ret = kbcGet(KBC_PARA0, tmp);
    if (tmp)
        mode = true;
    else
        mode = false;
    return ret;
}
int thermalUsb::KbcSetAutoMin(uint8_t min)
{
    return kbcSet(KBC_PARA2, min);
}
int thermalUsb::KbcGetAutoMin(uint8_t &min)
{
    return kbcGet(KBC_PARA2, min);
}
int thermalUsb::KbcSetAutoMax(uint8_t max)
{
    return kbcSet(KBC_PARA3, max);
}
int thermalUsb::KbcGetAutoMax(uint8_t &max)
{
    return kbcGet(KBC_PARA3, max);
}
int thermalUsb::KbcSetManual()
{
    uint8_t tmp;
    return kbcSet(KBC_Manual, tmp);
}
int thermalUsb::KbcSetReset()
{
    uint8_t tmp;
    return kbcSet(KBC_Reset, tmp);
}
/*
*函数名：   rmvcSetEnable
*函数功能： 设置锅盖校正使能
*参数说明： en true使能锅盖校正，false禁用锅盖校正
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::rmvcSetEnable(bool en)
{

    uint8_t byLen = 0;
    uint8_t byPara;
    uint8_t data;
    int ret;

    return ret;
}
/*
*函数名：   rmvcStartCali
*函数功能： 开始锅盖校准
*参数说明： 没有参数
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::rmvcStartCali()
{

    uint8_t byLen = 0;
    uint8_t byPara;
    uint8_t data;
    int ret;

    return ret;
}
/*
*函数名：   rmvcReset
*函数功能： 复位锅盖校准
*参数说明： 没有参数
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::rmvcReset()
{

    uint8_t byLen = 0;
    uint8_t byPara;
    uint8_t data;
    int ret;

    return ret;
}
/*
*函数名：   tpdSetEnable
*函数功能： 设置温度测量使能
*参数说明： en true使能温度测量，false禁用温度测量 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdSetEnable(bool en)
{

    uint8_t byLen = 0;
    uint8_t byPara;
    uint8_t data;
    int ret;

    if (ret == byLen)
    {
        tpdEnable = en;
    }
    return ret;
}
/*
*函数名：   tpdSetEnvTargetEmissivity
*函数功能： 设置目标反射率，使用7位定点数
*参数说明： r 7位定点数，0~128 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdSetEnvTargetEmissivity(uint8_t r)
{
    int ret;
    if (r > 128)
    {
        return -1;
    }

    ret = tpdEnvSet((uint8_t)TPD_ENV_TargetEmissivity, (uint16_t)r);

    return ret;
}
/*
*函数名：   tpdSetEnvAtmosphericTransmittance
*函数功能： 设置大气透过率，使用7位定点数
*参数说明： t 7位定点数，0~128 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdSetEnvAtmosphericTransmittance(uint8_t t)
{
    int ret;
    if (t > 128)
    {
        return -1;
    }

    ret = tpdEnvSet((uint8_t)TPD_ENV_AtmosphericTransmittance, (uint16_t)t);

    return ret;
}
/*
*函数名：   tpdSetEnvAtmosphericTemperature
*函数功能： 设置大气温度
*参数说明： t单位K,温度范围250~500 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdSetEnvAtmosphericTemperature(uint16_t t)
{
    int ret;
    if ((t < 250) || (t > 500))
    {
        return -1;
    }

    ret = tpdEnvSet((uint8_t)TPD_ENV_AtmosphericTemperature, t);

    return ret;
}
/*
*函数名：   tpdSetEnvTemperature
*函数功能： 设置环境温度
*参数说明： t单位K,温度范围250~500 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdSetEnvTemperature(uint16_t t)
{
    int ret;
    if ((t < 250) || (t > 500))
    {
        return -1;
    }

    ret = tpdEnvSet((uint8_t)TPD_ENV_Temperature, t);

    return ret;
}
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
int thermalUsb::tpdEnvSet(uint8_t para, uint16_t val)
{

    uint8_t byLen;
    uint8_t byPara;
    uint16_t data;
    int ret;

    switch ((enum TPD_ENV_PARA)para)
    {
    case TPD_ENV_TargetEmissivity:
        if (val <= 128)
        {
            byLen = 1;
            data = val;
            byPara = 0;
        }
        else
        {
            return -1;
        }

        break;
    case TPD_ENV_AtmosphericTransmittance:
        if (val <= 128)
        {
            byLen = 1;
            data = val;
            byPara = 1;
        }
        else
        {
            return -1;
        }
        break;
    case TPD_ENV_AtmosphericTemperature:
        if ((val >= 250) && (val <= 500))
        {
            byLen = 2;
            data = val;
            byPara = 2;
        }
        else
        {
            return -1;
        }
        break;
    case TPD_ENV_Temperature:
        if ((val >= 250) && (val <= 500))
        {
            byLen = 2;
            data = val;
            byPara = 3;
        }
        else
        {
            return -1;
        }
        break;
    default:
        return -1;
    }

    return ret;
}
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
int thermalUsb::tpdEnvGet(uint8_t para, uint16_t &val)
{
    enum UVC_byCmdType cmdType = TPD;
    enum UVC_bySubCmd subCmd = TPD_ENV_PARA_GET;
    uint8_t byLen;
    uint8_t byPara;
    int ret;

    switch ((enum TPD_ENV_PARA)para)
    {
    case TPD_ENV_TargetEmissivity:
        byLen = 1;
        byPara = 0;
        break;
    case TPD_ENV_AtmosphericTransmittance:
        byLen = 1;
        byPara = 1;
        break;
    case TPD_ENV_AtmosphericTemperature:
        byLen = 2;
        byPara = 2;
        break;
    case TPD_ENV_Temperature:
        byLen = 2;
        byPara = 3;
        break;
    default:
        return -1;
    }

    return ret;
}

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
                目标距离参数:使用7位定点数,取值范围0~2048，距离单位为m
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdDisSet(uint8_t para, int32_t val)
{

    uint8_t byLen;
    uint8_t byPara;
    int32_t data;
    int ret;

    switch ((enum TPD_DIS_PARA)para)
    {
    case TPD_DIS_PARA_A0:
        byLen = 4;
        data = val;
        byPara = 0;
        break;
    case TPD_DIS_PARA_B0:
        byLen = 4;
        data = val;
        byPara = 1;
        break;
    case TPD_DIS_PARA_C0:
        byLen = 2;
        data = val;
        byPara = 2;
        break;
    case TPD_DIS_PARA_A1:
        byLen = 4;
        data = val;
        byPara = 3;
        break;
    case TPD_DIS_PARA_B1:
        byLen = 4;
        data = val;
        byPara = 4;
        break;
    case TPD_DIS_PARA_C1:
        byLen = 2;
        data = val;
        byPara = 5;
        break;
    case TPD_DIS_PARA_TargetDist:
        if ((val <= 2048) && (val >= 0))
        {
            byLen = 2;
            data = val;
            byPara = 6;
        }
        else
        {
            return -1;
        }
        break;
    default:
        return -1;
    }

    return ret;
}
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
int thermalUsb::tpdDisGet(uint8_t para, uint16_t &val)
{

    uint8_t byLen;
    uint8_t byPara;
    int ret;

    return ret;
}
/*
*函数名：   tpdDisSetEnable
*函数功能： 设置距离校正使能
*参数说明： en true使能距离校正，false禁用距离校正 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdDisSetEnable(bool en)
{

    uint8_t byLen = 0;
    uint8_t byPara;
    uint8_t data;
    int ret;

    if (ret == byLen)
    {
        tpdDistCorrEnable = en;
    }
    return ret;
}

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
int thermalUsb::tpdSetReset(uint8_t index)
{

    uint8_t byLen = 0;
    uint8_t byPara;
    uint8_t data;
    int ret;

    return ret;
}

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
int thermalUsb::tpdSetTemperatureMode(uint8_t index, uint16_t t)
{

    uint8_t byLen;
    uint8_t byPara;
    uint8_t data;
    int ret;

    byLen = (uint8_t)((t & 0xff00) >> 8);
    byPara = (uint8_t)(t & 0xff);

    return ret;
}
/*
*函数名：   tpdSetRecal
*函数功能： 开始二次标定
*参数说明： 无 
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::tpdSetRecal()
{

    uint8_t byLen = 0;
    uint8_t byPara = 0;
    uint8_t data = 0;
    int ret;

    return ret;
}

/*
*函数名：   irsnrSetMode
*函数功能： 设置高低增益模式
*参数说明： mode  true为高增益， false为低增益
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::irsnrSetMode(bool mode)
{

    uint8_t byLen = 0;
    uint8_t byPara = 0;
    uint8_t data = 0;
    int ret;

    return ret;
}

/*
*函数名：   irsnrGetMode
*函数功能： 设置高低增益模式
*参数说明： mode  true为高增益， false为低增益
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::irsnrGetMode(bool &mode)
{

    uint8_t byLen = 0;
    uint8_t byPara = 0;
    uint8_t data = 0;
    int ret;

    return ret;
}

/*
*函数名：   irsnrReset
*函数功能： 重置用户配置，停止出图后发送，重新出图后生效
*参数说明： 无
*返回值说明：负数返回错误
            非负数正确返回
*/
int thermalUsb::irsnrReset()
{

    uint8_t byLen = 0;
    uint8_t byPara = 0;
    uint8_t data = 0;
    int ret;
    if (startFlag)
    {
    }

    return ret;
}
