#ifndef _Common_h_
#define _Common_h_


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include <sys/statfs.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define RUN_TEST printf("RUN_TEST @@@ %s %s:%d\n", __FUNCTION__, __FILE__, __LINE__);
#define DEVID "123456789012345f"

    void time_consuming_start(struct timeval *gTpstart, struct timeval *gTpend);
    float time_consuming_print(char *strPuts, struct timeval *gTpstart, struct timeval *gTpend);

    int mySystem(const char *cmd);
    void debugbuf(char *name, unsigned char *data, int len);

    /*获取配置文件INI 文件信息, profile 配置文件路径  keyname 键值名称 ， keyal 键值内容 */
    int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal);

    /*RGB24->YUV420SP*/
    void encodeYUV420sp(unsigned char *yuv420sp, unsigned char *rgb, int width, int height);

    int getBandwidth(int len);

    int getiotcameramem();
    int get_total_mem();
    int get_sysMemUsage();

#ifdef __cplusplus
}
#endif

#endif