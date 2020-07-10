#include "Common.h"


int mySystem(const char *cmd)
{
    pid_t pid;
    if (cmd == NULL)
        return 1;
    int status;
    if ((pid = fork()) < 0)
        status = -1;
    else if (0 == pid)
    {
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    }
    else
    {
        while (waitpid(pid, &status, 0) < 0)
            if (errno != EINTR)
                return -1;
    }
    return status;
}

void debugbuf(char *name, unsigned char *data, int len)
{
    int i = 0;
    printf("%s:\n", name);
    for (i = 0; i < len; i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

#define KEYVALLEN 100

/*   删除左边的空格   */
static char *l_trim(char *szOutput, const char *szInput)
{
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    for (NULL; *szInput != '\0' && isspace(*szInput); ++szInput)
    {
        ;
    }
    return strcpy(szOutput, szInput);
}

/*   删除右边的空格   */
static char *r_trim(char *szOutput, const char *szInput)
{
    char *p = NULL;
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    strcpy(szOutput, szInput);
    for (p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p)
    {
        ;
    }
    *(++p) = '\0';
    return szOutput;
}

/*   删除两边的空格   */
static char *a_trim(char *szOutput, const char *szInput)
{
    char *p = NULL;
    assert(szInput != NULL);
    assert(szOutput != NULL);
    l_trim(szOutput, szInput);
    for (p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p)
    {
        ;
    }
    *(++p) = '\0';
    return szOutput;
}
//从配置文件中获取参数
int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal)
{
    char appname[32], keyname[32];
    char *buf, *c;
    char buf_i[KEYVALLEN], buf_o[KEYVALLEN];
    FILE *fp;
    int found = 0; /* 1 AppName 2 KeyName */
    if ((fp = fopen(profile, "r")) == NULL)
    {
        printf("openfile [%s] error [%s]\n", profile, strerror(errno));
        return (-1);
    }
    fseek(fp, 0, SEEK_SET);
    memset(appname, 0, sizeof(appname));
    sprintf(appname, "[%s]", AppName);

    while (!feof(fp) && fgets(buf_i, KEYVALLEN, fp) != NULL)
    {
        l_trim(buf_o, buf_i);
        if (strlen(buf_o) <= 0)
            continue;
        buf = NULL;
        buf = buf_o;

        if (found == 0)
        {
            if (buf[0] != '[')
            {
                continue;
            }
            else if (strncmp(buf, appname, strlen(appname)) == 0)
            {
                found = 1;
                continue;
            }
        }
        else if (found == 1)
        {
            if (buf[0] == '#')
            {
                continue;
            }
            else if (buf[0] == '[')
            {
                break;
            }
            else
            {
                if ((c = (char *)strchr(buf, '=')) == NULL)
                    continue;
                memset(keyname, 0, sizeof(keyname));

                sscanf(buf, "%[^=|^ |^\t]", keyname);
                if (strcmp(keyname, KeyName) == 0)
                {
                    sscanf(++c, "%[^\n]", KeyVal);
                    char *KeyVal_o = (char *)malloc(strlen(KeyVal) + 1);
                    if (KeyVal_o != NULL)
                    {
                        memset(KeyVal_o, 0, sizeof(KeyVal_o));
                        a_trim(KeyVal_o, KeyVal);
                        if (KeyVal_o && strlen(KeyVal_o) > 0)
                            strcpy(KeyVal, KeyVal_o);
                        free(KeyVal_o);
                        KeyVal_o = NULL;
                    }
                    found = 2;
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
    }
    fclose(fp);
    if (found == 2)
        return (0);
    else
        return (-1);
}

static unsigned char clip_value(unsigned char val, unsigned char min_val, unsigned char max_val)
{
    if (val > max_val)
    {
        return max_val;
    }
    else if (val < min_val)
    {
        return min_val;
    }
    else
    {
        return val;
    }
}

static int __rgb24_to_yuv420p(unsigned char *rgb24_buf, int w, int h, unsigned char *yuv420p_buf)
{
    unsigned char *yuv420p_y = NULL;
    unsigned char *yuv420p_u = NULL;
    unsigned char *yuv420p_v = NULL;
    unsigned char *rgb24 = NULL;
    unsigned char y, u, v, r, g, b;
    int i, j;

    memset(yuv420p_buf, 0, (w * h * 3) / 2);
    yuv420p_y = yuv420p_buf;
    yuv420p_u = yuv420p_buf + w * h;
    yuv420p_v = yuv420p_buf + w * h + (w * h) / 4;

    // j可以理解图片中的各行
    for (j = 0; j < h; j++)
    {
        // rgb24指向图片中每一行的开始
        rgb24 = rgb24_buf + j * w * 3; // j * w * 3每一行所有像素点占用内存空间

        // i可以理解图片中一行中的每一个像素点
        for (i = 0; i < w; i++)
        {
            // 获取第i行中每一个像素点的r,g,b值
            r = *(rgb24++);
            g = *(rgb24++);
            b = *(rgb24++);

            // rgb转yuv计算公式
            // Y =  0.299*R + 0.587*G + 0.114*B
            // U = -0.147*R - 0.289*G + 0.463*B
            // V =  0.615*R - 0.515*G - 0.100*B
            // 按转换公式转换成y,u,v，转换成整数运算，避免浮点运算
            y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
            u = (unsigned char)((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
            v = (unsigned char)((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;

            // 取全部的Y
            *(yuv420p_y++) = clip_value(y, 0, 255);

            if (j % 2 == 0)
            {
                // u取的是偶数行的偶数列，相当于宽、高都除2, 也就是w * h/4;
                if (i % 2 == 0)
                {
                    *(yuv420p_u++) = clip_value(u, 0, 255);
                }
            }
            else
            {
                // v取的是奇数行的偶数列，相当于宽、高都除2, 也就是w * h/4;
                if (i % 2 == 0)
                {
                    *(yuv420p_v++) = clip_value(v, 0, 255);
                }
            }
        }
    }

    return 0;
}

int rgb32_to_i420(int width, int height, const char *src, char *dst)
{
    unsigned char *dst_y_even;
    unsigned char *dst_y_odd;
    unsigned char *dst_u;
    unsigned char *dst_v;
    const unsigned char *src_even;
    const unsigned char *src_odd;
    int i, j;

    src_even = (const unsigned char *)src;
    src_odd = src_even + width * 4;

    // it's planar

    dst_y_even = (unsigned char *)dst;
    dst_y_odd = dst_y_even + width;
    dst_u = dst_y_even + width * height;
    dst_v = dst_u + ((width * height) >> 2);

    // NB this doesn't work perfectly for u and v values of the edges of the video if your video size is not divisible by 2. FWIW.
    for (i = 0; i < height / 2; ++i)
    {
        for (j = 0; j < width / 2; ++j)
        {
            short r, g, b;
            b = *src_even++;
            g = *src_even++;
            r = *src_even++;
            ++src_even;
            *dst_y_even++ = ((r * 66 + g * 129 + b * 25 + 128) >> 8) + 16;
            short sum_r = r, sum_g = g, sum_b = b;

            b = *src_even++;
            g = *src_even++;
            r = *src_even++;
            ++src_even;
            *dst_y_even++ = ((r * 66 + g * 129 + b * 25 + 128) >> 8) + 16;
            sum_r += r;
            sum_g += g;
            sum_b += b;

            b = *src_odd++;
            g = *src_odd++;
            r = *src_odd++;
            ++src_odd;
            *dst_y_odd++ = ((r * 66 + g * 129 + b * 25 + 128) >> 8) + 16;
            sum_r += r;
            sum_g += g;
            sum_b += b;

            b = *src_odd++;
            g = *src_odd++;
            r = *src_odd++;
            ++src_odd;
            *dst_y_odd++ = ((r * 66 + g * 129 + b * 25 + 128) >> 8) + 16;
            sum_r += r;
            sum_g += g;
            sum_b += b;

            // compute ave's of this 2x2 bloc for its u and v values
            // could use Catmull-Rom interpolation possibly? http://msdn.microsoft.com/en-us/library/Aa904813#yuvformats_420formats_16bitsperpixel
            // rounding by one? don't care enough... 39 ms -> 36.8
            //sum_r += 2;
            //sum_g += 2;
            //sum_b += 2;

            // divide by 4 to average
            sum_r /= 4;
            sum_g /= 4;
            sum_b /= 4;

            *dst_u++ = ((sum_r * -38 - sum_g * 74 + sum_b * 112 + 128) >> 8) + 128; // only one
            *dst_v++ = ((sum_r * 112 - sum_g * 94 - sum_b * 18 + 128) >> 8) + 128;  // only one
        }

        dst_y_even += width;
        dst_y_odd += width;
        src_even += width * 4;
        src_odd += width * 4;
    }

    return 0;
}

void rgb_to_yuv420p(unsigned char *yuv420p, unsigned char *rgb, int width, int height)
{
    if (yuv420p == NULL || rgb == NULL)
        return;
    int frameSize = width * height;
    int chromaSize = frameSize / 4;

    int yIndex = 0;
    int uIndex = frameSize;
    int vIndex = frameSize + chromaSize;

    int R, G, B, Y, U, V;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            B = rgb[(i * width + j) * 3 + 0];
            G = rgb[(i * width + j) * 3 + 1];
            R = rgb[(i * width + j) * 3 + 2];

            //RGB to YUV
            Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
            U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
            V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

            yuv420p[yIndex++] = (unsigned char)((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
            if (i % 2 == 0 && j % 2 == 0)
            {
                yuv420p[uIndex++] = (unsigned char)((U < 0) ? 0 : ((U > 255) ? 255 : U));
                yuv420p[vIndex++] = (unsigned char)((V < 0) ? 0 : ((V > 255) ? 255 : V));
            }
        }
    }
}

void encodeYUV420sp(unsigned char *yuv420sp, unsigned char *rgb, int width, int height)
{
    if (yuv420sp == NULL || rgb == NULL)
        return;
    int frameSize = width * height;
    int yIndex = 0;
    int uvIndex = frameSize;

    int R, G, B, Y, U, V;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            B = rgb[(i * width + j) * 3 + 0];
            G = rgb[(i * width + j) * 3 + 1];
            R = rgb[(i * width + j) * 3 + 2];

            //RGB to YUV
            Y = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
            U = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
            V = ((112 * R - 94 * G - 18 * B + 128) >> 8) + 128;

            yuv420sp[yIndex++] = (unsigned char)((Y < 0) ? 0 : ((Y > 255) ? 255 : Y));
            if (i % 2 == 0 && j % 2 == 0)
            {
                yuv420sp[uvIndex++] = (unsigned char)((V < 0) ? 0 : ((V > 255) ? 255 : V));
                yuv420sp[uvIndex++] = (unsigned char)((U < 0) ? 0 : ((U > 255) ? 255 : U));
            }
        }
    }
}

int getBandwidth(int len)
{
    static unsigned int __sec = 0;
    static unsigned int __total = 0;

    struct timeval Tpend;
    gettimeofday(&Tpend, NULL);

    //RUN_TEST;

    if (__sec == 0)
        __sec = Tpend.tv_sec;
    else
    {
        if ((Tpend.tv_sec - __sec) > 5)
        {
            printf("Current SPEED %d %d %d KB/S\n", __total, (Tpend.tv_sec - __sec), __total / (Tpend.tv_sec - __sec) / 1024);
            __total = 0;
            __sec = Tpend.tv_sec;
        }
        else
        {
            __total += len;
        }
    }
}

void time_consuming_start(struct timeval *gTpstart, struct timeval *gTpend)
{
    memset(gTpstart, 0, sizeof(struct timeval));
    memset(gTpend, 0, sizeof(struct timeval));
    gettimeofday(gTpstart, NULL); // 开始时间
}

float time_consuming_print(char *strPuts, struct timeval *gTpstart, struct timeval *gTpend)
{
    float timeuse;
    gettimeofday(gTpend, NULL); // 结束时间
    timeuse = 1000000 * (gTpend->tv_sec - gTpstart->tv_sec) + gTpend->tv_usec - gTpstart->tv_usec;
    timeuse /= 1000000;
    printf("@ %s -----> Used Time:%f  S\n", strPuts, timeuse);

    return timeuse;
}
