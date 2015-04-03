#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#define CFG_HOLDER	0x00FF55A4	/* Change this value to load default configurations */
#define CLIENT_SSL_ENABLE

/*DEFAULT CONFIGURATIONS*/

//#define MQTT_HOST			"192.168.11.122" //or "mqtt.yourdomain.com"
//#define MQTT_PORT			1880
//#define MQTT_BUF_SIZE		1024
//#define MQTT_KEEPALIVE		120	 /*second*/

//#define MQTT_CLIENT_ID		"DVES_%08X"
//#define MQTT_USER			"DVES_USER"
//#define MQTT_PASS			"DVES_PASS"

//#define MQTT_RECONNECT_TIMEOUT 	5	/*second*/

#define DEFAULT_SECURITY	0
#define QUEUE_BUFFER_SIZE		 		2048

#define USE_WIFI_MODE		STATIONAP_MODE
#define PLATFORM_DEBUG		true

#endif
