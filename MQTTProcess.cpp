#include "mqtt/mqtt.h"
#include "JSON/cJSON.h"
#include "MQTTProcess.h"
#include "PCM/pcm.h"
#include "Common.h"

static char *xintiao(char *UUID)
{
	char *p;
	cJSON *pJsonRoot = NULL;
	cJSON *pJsonData = NULL;
	char tmpstr[32];

	pJsonRoot = cJSON_CreateObject();
	pJsonData = cJSON_CreateObject();
	if (NULL == pJsonRoot)
	{
		goto err;
	}
	if (NULL == pJsonData)
	{
		goto err;
	}

	cJSON_AddStringToObject(pJsonRoot, "DEVID", DEVID);
	cJSON_AddStringToObject(pJsonRoot, "MSGID", UUID);
	cJSON_AddStringToObject(pJsonRoot, "TIME", "");
	cJSON_AddNumberToObject(pJsonRoot, "MSGTYPE", 1); //上行消息
	cJSON_AddNumberToObject(pJsonRoot, "DEVTYPE", 0); //安全帽
	cJSON_AddItemToObject(pJsonRoot, "DATA", pJsonData);
	cJSON_AddNumberToObject(pJsonData, "CMDID", 2);

	p = cJSON_Print(pJsonRoot);
	if (NULL == p)
	{
		cJSON_Delete(pJsonRoot);
		goto err;
	}
	cJSON_Delete(pJsonRoot);
	return p;

err:
	return 0;
}

class MQTTData : public QDataBase
{
public:
	char from[128];
	char msg[256];
	MQTTData(){};
	~MQTTData(){};
	virtual int process()
	{
		printf("%s %s\n", from, msg);
		cJSON *pSub;
		cJSON *pJson;
		if (NULL == msg)
		{
			return 0;
		}
		pJson = cJSON_Parse(msg);
		if (NULL == pJson)
		{
			return 0;
		}

		pSub = cJSON_GetObjectItem(pJson, "MSGID");
		if (NULL != pSub)
		{
			printf("UUID:%s\n", pSub->valuestring);
			char *str = xintiao(pSub->valuestring);
			pubmsg(str);
			free(str);
		}

		cJSON_Delete(pJson);
	}
};

/*本地MQTT接收到数据的回调函数*/
int mqttArrived_Fun(char *from, char *msg)
{
	printf("%s %s\n", from, msg);
#if 1
	MQTTData *md = new MQTTData();
	snprintf(md->from, sizeof(md->from), "%s", from);
	snprintf(md->msg, sizeof(md->msg), "%s", msg);
	producter_push(md);
#endif
	return 0;
}