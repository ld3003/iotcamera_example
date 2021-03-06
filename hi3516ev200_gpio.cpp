#include <stdio.h>
#include <string.h>
#include "hi3516ev200_gpio.h"

int hi3516ev200_gpio_in(unsigned int gpio_chip_num, unsigned int gpio_offset_num)
{
    FILE *fp;
    char file_name[50];
    unsigned char buf[10];
    unsigned int gpio_num;
    gpio_num = gpio_chip_num * 8 + gpio_offset_num;
    sprintf(file_name, "/sys/class/gpio/export");
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    fprintf(fp, "%d", gpio_num);
    fclose(fp);
    sprintf(file_name, "/sys/class/gpio/gpio%d/direction", gpio_num);
    fp = fopen(file_name, "rb+");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    fprintf(fp, "in");
    fclose(fp);
    sprintf(file_name, "/sys/class/gpio/gpio%d/value", gpio_num);
    fp = fopen(file_name, "rb+");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    memset(buf, 0, 10);
    fread(buf, sizeof(char), sizeof(buf) - 1, fp);
    printf("%s: gpio%d_%d = %d\n", __func__,
           gpio_chip_num, gpio_offset_num, buf[0] - 48);
    fclose(fp);
    sprintf(file_name, "/sys/class/gpio/unexport");
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    fprintf(fp, "%d", gpio_num);
    fclose(fp);
    return (int)(buf[0] - 48);
}
int hi3516ev200_gpio_out(unsigned int gpio_chip_num, unsigned int gpio_offset_num,
                  unsigned int gpio_out_val)
{
    FILE *fp;
    char file_name[50];
    char buf[10];
    unsigned int gpio_num;
    gpio_num = gpio_chip_num * 8 + gpio_offset_num;

    //导出
    sprintf(file_name, "/sys/class/gpio/export");
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    fprintf(fp, "%d", gpio_num);
    fclose(fp);

    //方向
    sprintf(file_name, "/sys/class/gpio/gpio%d/direction", gpio_num);
    fp = fopen(file_name, "rb+");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    fprintf(fp, "out");
    fclose(fp);

    //设置
    sprintf(file_name, "/sys/class/gpio/gpio%d/value", gpio_num);
    fp = fopen(file_name, "rb+");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    if (gpio_out_val)
        strcpy(buf, "1");
    else
        strcpy(buf, "0");

    fwrite(buf, sizeof(char), sizeof(buf) - 1, fp);
    printf("%s: gpio%d_%d = %s\n", __func__,
           gpio_chip_num, gpio_offset_num, buf);
    fclose(fp);

    //关闭
    sprintf(file_name, "/sys/class/gpio/unexport");
    fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Cannot open %s.\n", file_name);
        return -1;
    }
    fprintf(fp, "%d", gpio_num);
    fclose(fp);
    return 0;
}