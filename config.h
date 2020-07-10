#ifndef __json_cfg_h__
#define __json_cfg_h__

#ifdef __cplusplus
extern "C"
{
#endif

//定义两个随机数字，确保读取出来的数据是正确的
#define CHECKSTART 9423434
#define CHEKEND 342134

#define SUPERCARD_LENGTH (500 * 8)

#pragma pack(push, 1) //数据在内存中对其方式
struct
    CFGDATA
{
    unsigned int runcount;//运行或者是重启次数
    unsigned int checkstart;

    unsigned char network_mod; // 0 4g wifi eth auto
    char rtmpurl[128];

    unsigned int checkend;
};

#pragma pack(pop)

void readcfg();
struct CFGDATA *getCfg();
void savecfg(char *log, int line);

#ifdef __cplusplus
}
#endif

#endif