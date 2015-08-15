/*
 * msg_handler.c
 *
 *  Created on: 30.01.2015
 *      Author: K4546
 */

#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "http/httpd.h"
#include "cgi/cgi.h"
#include "io/io.h"
#include "ip/TcpClient.h"
#include "spi_flash.h"
#include "smhodule.h"
#include "espmissingincludes.h"
//#include "mqtt/mqtt.h"

#define FLASHSECTOR 0x3A
SETUPDATA Setup;
char payload[1024];

//MQTT_Client mqttClient;
//static char pheadbuffer[] = "Connection: close\r\n\r\n";

static char pheadbuffer[] = "Keep-Alive: 5\r\n\
Connection: keep-alive\r\n\r\n";

/*
static void ICACHE_FLASH_ATTR nop_cb(void) {
	os_printf("nop_cb\r\n");
}
*/
static void ICACHE_FLASH_ATTR createPayload( int count, char * ip ) {
	char hostdata[480];
    if ( count < 0 ) {
    	count = getGpio13Counter();
    	resetGpio13Counter();
    }
    	os_sprintf(hostdata,"HTTP/1.0\r\nHost: %s\r\n%s",ip,pheadbuffer);
    	switch (atoi((char *) Setup.DPType)) {
    	case 1: // HM: http://IP_DER_ZENTRALE:8181/test.exe?x=dom.GetObject('BidCos-RF.SERIENNUMMER:1.STATE').State(1);
    		os_sprintf(payload, "GET /execute.exe?x=dom.GetObject(\"%s\").Value(%d) %s",(uint8_t *) Setup.DP,count,hostdata);
    		break;
    	case 2: //CCUIO: http://ccu-io-host:ccu.io-port/api/set/950/?value=1
    		os_sprintf(payload, "GET /api/set/%s/?value=%d %s",(uint8_t *) Setup.DP,count,hostdata);
    		break;
    	case 3: // IOBroker: http://ip:8087/set/hm-rpc.0.IEQ12345.LEVEL?user=admin&pass=iobroker?value=1&prettyPrint
    		if ( os_strlen((uint8_t *) Setup.User) > 0 ) {
    			os_sprintf(payload, "GET /set/%s?user=%s&pass=%s?value=%d&prettyPrint %s",(uint8_t *) Setup.DP,count,hostdata);
    		} else {
    			os_sprintf(payload, "GET /set/%s?value=%d&prettyPrint %s",(uint8_t *) Setup.DP,count,hostdata);
    		}
    		break;
    	case 4: // NATIVE: http://IP:PORT/irgendetwas/DataPointer/Val=%VALUE%
     		os_sprintf(payload,"GET /%s%d %s",(uint8_t *) Setup.DP,count,hostdata);
    		break;
    	case 5: // MQTT
    		break;
    }
}

void ICACHE_FLASH_ATTR sendStatus(int i) {
	//if ( blocked == 0 ) {
    	if ( os_strlen(Setup.IP[0]) > 0 ) {
    		os_printf("Sending status\n");
    		ETS_GPIO_INTR_DISABLE();
    		createPayload(i, (uint8_t *) Setup.IP[0]);
    	    TcpSend(TCP, (uint8_t *) Setup.IP[0], atoi((uint8_t *) Setup.Port[0]),payload, NULL);
        	ETS_GPIO_INTR_ENABLE();
    	}
	//}
}

void ICACHE_FLASH_ATTR readConfig( void ) {
	spi_flash_read((SPI_FLASH_SEC_SIZE * FLASHSECTOR), (uint32 *)&Setup, sizeof(SETUPDATA));
}

void ICACHE_FLASH_ATTR resetPermMode( void ) {
	spi_flash_read((SPI_FLASH_SEC_SIZE * FLASHSECTOR), (uint32 *)&Setup, sizeof(SETUPDATA));
	os_strcpy((uint8_t *) Setup.Interval, "0");
	saveConfig();
}

void ICACHE_FLASH_ATTR saveConfig() {
	os_printf("Saving: %s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s-%s\r\n",(uint8_t *) Setup.DPType,(uint8_t *) Setup.DP,(uint8_t *) Setup.IP[0],(uint8_t *) Setup.Port[0],(uint8_t *) Setup.IP[1],(uint8_t *) Setup.Port[1],(uint8_t *) Setup.Auth,(uint8_t *) Setup.Interval,(uint8_t *) Setup.User,(uint8_t *) Setup.Pass,(uint8_t *) Setup.Type,(uint8_t *) Setup.Device);

	os_timer_disarm(&smhodule_timer);
	os_printf("saving config to flash\n");
	ETS_GPIO_INTR_DISABLE();
	ETS_UART_INTR_DISABLE();
	spi_flash_erase_sector(FLASHSECTOR);
	spi_flash_write((SPI_FLASH_SEC_SIZE * FLASHSECTOR), (uint32 *)&Setup, sizeof(SETUPDATA));
	ETS_UART_INTR_ENABLE();
	ETS_GPIO_INTR_ENABLE();
	os_timer_arm(&smhodule_timer, (seconds * 1000), 1);
}

static void ICACHE_FLASH_ATTR sleep_cb(void) {
	os_printf("sleep_cb\r\n");
	if ( (atoi((uint8_t *) Setup.Interval) > 0) && (atoi((uint8_t *) Setup.Type) == 1)) {
		// if used as a Sender then we can use deep-sleep to save power, acting as a receiver or Permanent then skip deep sleep
		// GPIO 16 has to be connected to RST for making deep_sleep work.
		system_deep_sleep(atoi((uint8_t *) Setup.Interval)*60*1000*1000);
	}
}

void ICACHE_FLASH_ATTR sendKeyData(int i, int j) {
	char payload[1024];
	int id;
	static int lastvalue[10] = {0,0,0,0,0,0,0,0,0,0};
	lastvalue[i] = ( lastvalue[i] ) ? 0 : 1;
	lastvalue[i+1] = j;
	os_sprintf(payload, "GET /api/set?switch=1&toggle=%d&key=%d&percent=%d HTTP/1.0\r\nHost: ",lastvalue[i],i,lastvalue[i+1]);
//	if ( blocked == 0 ) {
    	ETS_GPIO_INTR_DISABLE();
    	for (id=1;id<=3;id++) {
    		if ( os_strlen(Setup.IP[id]) > 0 ) {
    			os_sprintf(payload, "%s %s\r\n%s",payload,(uint8_t *) Setup.IP[id],pheadbuffer);
    			os_printf(payload);
    			TcpSend(TCP, (uint8_t *) Setup.IP[id], atoi((uint8_t *) Setup.Port[id]), payload, NULL);
    		}
    	}
    	ETS_GPIO_INTR_ENABLE();
//    }
}

static void ICACHE_FLASH_ATTR sendData(void *arg) {
//    if ( blocked == 0 ) {
/*    	response if ccu.io
 * 		{
    	  "100015": {
    	    "val": 50,
    	    "ts": "2015-03-03 23:04:19"
    	  }
    	} */
    	if ( os_strlen(Setup.IP[0]) > 0 ) {
    		ETS_GPIO_INTR_DISABLE();
    		createPayload(-1, (uint8_t *) Setup.IP);
    		TcpSend(TCP, (uint8_t *) Setup.IP[0], atoi((uint8_t *) Setup.Port[0]), payload, sleep_cb);
    		ETS_GPIO_INTR_ENABLE();
    	}
//    }
}

// normal system startup
void smhoduleInit (void ) {
	readConfig();
//try at higher cpu speed.
//	REG_SET_BIT(0x3ff00014, BIT(0));
//    os_update_cpu_frequency(160);
//    REG_CLR_BIT(0x3ff00014, BIT(0));
//    os_update_cpu_frequency(80);

#ifdef PLATFORM_DEBUG
	os_printf("max heap available: %u\r\n",system_get_free_heap_size());
#endif
	if ( atoi((uint8_t *) Setup.DPType) == 5 ) {
		// MQTT
		//mqtt_init();
	} else {
		ioInit();
		if ( atoi((uint8_t *) Setup.Type) == 1 ) {
			os_printf("SmHodule is acting as a sensor %d\n", seconds);
			os_timer_disarm(&smhodule_timer);
			os_timer_setfn(&smhodule_timer, sendData, NULL); // set the timer function, dot get os_timer_func_t to force function convert
			os_timer_arm(&smhodule_timer, (seconds * 1000), 1);
		} else if  (atoi((uint8_t *) Setup.Type) == 2 ) {
			os_printf("SmHodule is acting as a control\n");
		} else {
			os_printf("SmHodule is acting as an actor\n");
		}
	}
}

//os_sprintf(sysCfg.mqtt_user, "%s", MQTT_USER);
void ICACHE_FLASH_ATTR initSetupData( void ) {
	// only on first time setup or when wifi is reset
	int id;
	os_printf("reset Setup\n");
/*
	os_strcpy((uint8_t *) Setup.isInitialized, "0");
	os_strcpy((uint8_t *) Setup.OLED, "0");
	os_strcpy((uint8_t *) Setup.BMP180, "0");
	os_sprintf((uint8_t *) Setup.Device, "%s", getSmHoduleName());
	os_sprintf((uint8_t *) Setup.device_id, "");
	os_sprintf((uint8_t *) Setup.DP, "");
	os_sprintf((uint8_t *) Setup.Altitude, "0");
	os_sprintf((uint8_t *) Setup.User, "");
	os_sprintf((uint8_t *) Setup.Pass, "");
	os_strcpy((uint8_t *) Setup.DPType, "1");
	os_strcpy((uint8_t *) Setup.Type, "3"); //defaults to act as switch
	os_strcpy((uint8_t *) Setup.Interval, "0");
	for ( id=0; id<=3; id++) {
		os_sprintf((uint8_t *) Setup.IP[id], "");
		os_sprintf((uint8_t *) Setup.Port[id], "8080");
	}
	os_strcpy((uint8_t *) Setup.SSL, "0");
	os_strcpy((uint8_t *) Setup.Auth, "0");
	*/
	os_strcpy((uint8_t *) Setup.WifiApPassword, "00000000");
	os_strcpy((uint8_t *) Setup.WifiStPassword, "00000000");
	saveConfig();
}

void ICACHE_FLASH_ATTR getSetupData() {
	readConfig();
}

/*
void blockSmhodule(void) {
	blocked=1;
}

void unblockSmhodule(void) {
	blocked=0;
}
*/
/*
void mqttConnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Connected\r\n");
	MQTT_Subscribe(client, "/mqtt/topic/0", 0);
	MQTT_Subscribe(client, "/mqtt/topic/1", 1);
	MQTT_Subscribe(client, "/mqtt/topic/2", 2);

	MQTT_Publish(client, "/mqtt/topic/0", "hello0", 6, 0, 0);
	MQTT_Publish(client, "/mqtt/topic/1", "hello1", 6, 1, 0);
	MQTT_Publish(client, "/mqtt/topic/2", "hello2", 6, 2, 0);

}

void mqttDisconnectedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Disconnected\r\n");
}

void mqttPublishedCb(uint32_t *args)
{
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Published\r\n");
}

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t data_len)
{
	char *topicBuf = (char*)os_zalloc(topic_len+1),
			*dataBuf = (char*)os_zalloc(data_len+1);

	MQTT_Client* client = (MQTT_Client*)args;

	os_memcpy(topicBuf, topic, topic_len);
	topicBuf[topic_len] = 0;

	os_memcpy(dataBuf, data, data_len);
	dataBuf[data_len] = 0;

	os_printf("Receive topic: %s, data: %s \r\n", topicBuf, dataBuf);
	os_free(topicBuf);
	os_free(dataBuf);
}


void mqtt_init(void)
{
	MQTT_InitConnection(&mqttClient, (char *) Setup.IP, atoi((char *) Setup.Port), atoi((char *) Setup.SSL));
	//MQTT_InitConnection(&mqttClient, "192.168.11.122", 1880, 0);

	MQTT_InitClient(&mqttClient, (char *) Setup.device_id, (char *) Setup.User, (char *) Setup.Pass, 120, 1);
	//MQTT_InitClient(&mqttClient, "client_id", "user", "pass", 120, 1);

	MQTT_InitLWT(&mqttClient, "/lwt", "offline", 0, 0);
	MQTT_OnConnected(&mqttClient, mqttConnectedCb);
	MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
	MQTT_OnPublished(&mqttClient, mqttPublishedCb);
	MQTT_OnData(&mqttClient, mqttDataCb);

}
*/
