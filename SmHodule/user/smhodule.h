#ifndef __SMODULE_H
#define __SMODULE_H

#include "wifi/mac.h"

static bool blocked = 0;

typedef struct {
	uint8_t isInitialized[2]; // 0 or 1
	uint8_t OLED[2]; // 0 or 1
	uint8_t BMP180[2]; // 0 or 1
	uint8_t Altitude[6]; // 0 or 1
	uint8_t Device[20];
	//uint8_t device_id[20];
	uint8_t DP[16];
	uint8_t User[64];
	uint8_t Pass[64];
	uint8_t DPType[2];		//HM=1,CCUIO=2,IOBroker=3,NATIVE=4,MQTT=5 (needs to be 2 uint8_t for being able to store \0
	uint8_t Interval[2];	//1MIN=1,5MIN=5,PERM=0
	uint8_t IP[42];
	uint8_t APort[6];
	uint8_t AIP[42];
	uint8_t Port[6];
	uint8_t SSL[2];
	uint8_t Auth[2];
	uint8_t Type[2];
	uint8_t WifiApPassword[16];
	uint8_t WifiStPassword[16];
	uint8_t pad[2];   // pad to 4 byte boundary
} SETUPDATA;

typedef struct {
	uint8_t Line1[4][8]; // 4 line 8 characters
	// mind to use 4byte chunks
} OLEDDATA;

extern SETUPDATA Setup;
extern OLEDDATA OLed;

static ETSTimer smhodule_timer;
static int seconds = 10;

static void sendData(void *arg);
void sendKeyData(int key, int duration, int mode);
void smhoduleInit (void );
void initSetupData( void );
void getSetupData();
void saveConfig();
void resetPermMode( void );
//void blockSmhodule(void);
//void unblockSmhodule(void);
void sendStatus(int i);
void writeOLed(int line, char *message);
void saveOLedData();
void getOLedData();
//void mqtt_init(void);
#endif
