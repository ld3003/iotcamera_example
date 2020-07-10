#include "config.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//#include "cJSON.h"
#include "string.h"
#include "stdlib.h"
#include "Common.h"
#include "config.h"
#define CFGPATH "/data/cfg.json"

static struct CFGDATA cfgdata;
static struct CFGDATA cfgdata_bak;

#define DEBUGCFG(X) debugbuf("X", X, sizeof(X));
//比较字符串
int mymemcmp(unsigned char *s1, unsigned char *s2, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (s1[i] != s2[i])
            return -1;
    }
    return 0;
}
//读取系统配置
void readcfg()
{
    int fd, n;
    unsigned char *buf = (unsigned char *)&cfgdata;
    char *path = CFGPATH; //"/mnt/extsd/cfg.json";

    memset(&cfgdata, 0x0, sizeof(struct CFGDATA));

    fd = open(path, O_RDWR);
    if (fd)
    {
        n = read(fd, buf, sizeof(struct CFGDATA));
        if (n > 0)
        {
        }

        close(fd);
    }
    //

    if ((cfgdata.checkstart == CHECKSTART) && (cfgdata.checkend == CHEKEND))
    {
        printf("read success\n");
    }
    else
    {
        memset(&cfgdata, 0x0, sizeof(cfgdata));
        cfgdata.checkstart = CHECKSTART;
        cfgdata.checkend = CHEKEND;

        cfgdata.network_mod = 0;



        savecfg((char*)__FUNCTION__, __LINE__);
    }

    //DEBUGCFG(cfgdata.qrcode);
    //DEBUGCFG(cfgdata.supercard);

    memcpy(&cfgdata_bak, &cfgdata, sizeof(struct CFGDATA));
}

struct CFGDATA *getCfg()
{
    return &cfgdata;
}
//保存系统配置 
void savecfg(char *log, int line)
{
    int fd, n;
    char *path = CFGPATH;
    if (mymemcmp((unsigned char *)&cfgdata, (unsigned char *)&cfgdata_bak, sizeof(struct CFGDATA)) == 0)
    {
        printf("bu yong xie ru \n");
        return;
    }
    fd = open(path, O_RDWR | O_CREAT | O_TRUNC);
    if (fd)
    {
        n = write(fd, &cfgdata, sizeof(struct CFGDATA)); /*写入文件*/
        //fflush(fd);
        memcpy(&cfgdata_bak, &cfgdata, sizeof(struct CFGDATA));
        //screen_debug("writeconfig !!!!");
        //debugbuf("cfgdata",(unsigned char*)&cfgdata,sizeof(struct CFGDATA));
        //debugbuf("cfgdata_bak",(unsigned char*)&cfgdata_bak,sizeof(struct CFGDATA));
        if (log)
        {
            //screen_debug("%d:%s", line, log);
        }
        close(fd);
    }
}
