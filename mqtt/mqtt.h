#ifndef __MQTT_H__
#define __MQTT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define ADDRESS "tcp://47.104.166.126:1883"  //更改此处地址
#define QOS 1
#define TIMEOUT 10000L
#define USERNAME ""
#define PASSWORD ""

/*
topic 规则
上行协议：公司名称/设别类型/设备标识/上行标记/#
yswl/0/1/up/123456789012345
*/

struct MQTTClientData
{
    unsigned char conn;
};

typedef int (*MQTT_RECV_MSG)(char *from, char *msg);

int start_mqtt_client();
void mqtt_client_thread(void *p);
int set_mqtt_clientid(char *clientid);
int set_p2psignal_callback(MQTT_RECV_MSG msgrecv);
void p2psignal_subscribe(void *p);
int send_p2psignal_msg(char *to, char *msg);
int pubmsg(char *msg);


#ifdef __cplusplus
}
#endif

#endif
