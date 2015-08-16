/*
 * cgiSetup.c
 *
 *  Created on: 27.02.2015
 *      Author: K4546
 */

#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "http/httpd.h"
#include "cgiSetup.h"
#include "io/io.h"
#include <ip_addr.h>
#include "smhodule.h"
#include "espmissingincludes.h"

//Cgi that handles setup param in the POST data
int ICACHE_FLASH_ATTR cgiSetup(HttpdConnData *connData) {
	int len;
	char buff[1024];
	SETUPDATA Settings;

	// not required data is already in memory: getSetupData();
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	httpdFindArg(connData->postBuff, "ALTITUDE", (uint8_t *) Settings.Altitude, 6);
	httpdFindArg(connData->postBuff, "DP", (uint8_t *) Settings.DP, 16);
	httpdFindArg(connData->postBuff, "DPTYPE", (uint8_t *) Settings.DPType, 2);
	httpdFindArg(connData->postBuff, "IP", (uint8_t *) Settings.IP, 42);
	httpdFindArg(connData->postBuff, "PORT", (uint8_t *) Settings.Port, 6);
	httpdFindArg(connData->postBuff, "AIP", (uint8_t *) Settings.AIP, 42);
	httpdFindArg(connData->postBuff, "APORT", (uint8_t *) Settings.APort, 6);
	httpdFindArg(connData->postBuff, "SSL", (uint8_t *) Settings.SSL, 2);
	//httpdFindArg(connData->postBuff, "MQTTID", (uint8_t *) Settings.device_id, 20);
	httpdFindArg(connData->postBuff, "AUTH", (uint8_t *) Settings.Auth, 2);
	httpdFindArg(connData->postBuff, "INTERVAL", (uint8_t *) Settings.Interval, 2);
	httpdFindArg(connData->postBuff, "USER", (uint8_t *) Settings.User, 64);
	httpdFindArg(connData->postBuff, "PASSWORD", (uint8_t *) Settings.Pass, 64);
	httpdFindArg(connData->postBuff, "TYPE", (uint8_t *) Settings.Type, 2);
	httpdFindArg(connData->postBuff, "DEVICE", (uint8_t *) Settings.Device, 20);
	httpdFindArg(connData->postBuff, "OLED", (uint8_t *) Settings.OLED, 2);
	httpdFindArg(connData->postBuff, "BMP180", (uint8_t *) Settings.BMP180, 2);

	os_strcpy((uint8_t *) Setup.isInitialized, "1");
	os_strcpy((uint8_t *) Setup.OLED, (uint8_t *) Settings.OLED );
	os_strcpy((uint8_t *) Setup.BMP180, (uint8_t *) Settings.BMP180 );
	os_sprintf((uint8_t *) Setup.Altitude, "%s", (uint8_t *) Settings.Altitude);
	os_sprintf((uint8_t *) Setup.Device, "%s", (uint8_t *) Settings.Device);
	//os_sprintf((uint8_t *) Setup.device_id, ""); // unused reserved for MQTT
	os_sprintf((uint8_t *) Setup.DP, "%s", (uint8_t *) Settings.DP);
	os_sprintf((uint8_t *) Setup.User, "%s", (uint8_t *) Settings.User);
	os_sprintf((uint8_t *) Setup.Pass, "%s", (uint8_t *) Settings.Pass);
	os_strcpy((uint8_t *) Setup.DPType, (uint8_t *) Settings.DPType);
	os_strcpy((uint8_t *) Setup.Type, (uint8_t *) Settings.Type);
	os_strcpy((uint8_t *) Setup.Interval, (uint8_t *) Settings.Interval);
	os_sprintf((uint8_t *) Setup.IP, "%s", (uint8_t *) Settings.IP);
	os_sprintf((uint8_t *) Setup.Port, "%s", (uint8_t *) Settings.Port);
	os_sprintf((uint8_t *) Setup.AIP, "%s", (uint8_t *) Settings.AIP);
	os_sprintf((uint8_t *) Setup.APort, "%s", (uint8_t *) Settings.APort);
	os_strcpy((uint8_t *) Setup.SSL, ""); // unused comes later
	os_strcpy((uint8_t *) Setup.Auth, (uint8_t *)  Settings.Auth);
	// so we save the changes..
	saveConfig();
	httpdRedirect(connData, "/setup/setup.tpl");
	return HTTPD_CGI_DONE;
}

//Template code for the Setup page.
void ICACHE_FLASH_ATTR tplSetup(HttpdConnData *connData, char *token, void **arg) {
	char buff[1024];
	int x;
	if (token==NULL) return;

	struct ip_info ipconfig;
	wifi_get_ip_info(STATION_IF, &ipconfig);

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "DPTYPE")==0) {
		os_strcpy(buff,(uint8_t *) Setup.DPType);
	} else if (os_strcmp(token, "INTERVAL")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Interval);
	} else if (os_strcmp(token, "TYPE")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Type);
	} else if (os_strcmp(token, "IP")==0) {
		os_strcpy(buff,(uint8_t *) Setup.IP);
	} else if (os_strcmp(token, "PORT")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Port);
	} else if (os_strcmp(token, "AIP")==0) {
		os_strcpy(buff,(uint8_t *) Setup.AIP);
	} else if (os_strcmp(token, "APORT")==0) {
		os_strcpy(buff,(uint8_t *) Setup.APort);
	} else if (os_strcmp(token, "SSL")==0) {
		os_strcpy(buff,(uint8_t *) Setup.SSL);
	//} else if (os_strcmp(token, "MQTTID")==0) {
	//	os_strcpy(buff,(uint8_t *) Setup.device_id);
	} else if (os_strcmp(token, "AUTH")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Auth);
	} else if (os_strcmp(token, "DP")==0) {
		os_strcpy(buff,(uint8_t *) Setup.DP);
	} else if (os_strcmp(token, "USER")==0) {
		os_strcpy(buff,(uint8_t *) Setup.User);
	} else if (os_strcmp(token, "PASSWORD")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Pass);
	} else if (os_strcmp(token, "MYMAC")==0) {
		os_strcpy(buff,(uint8_t *) getMacAddress());
	} else if (os_strcmp(token, "MYIP")==0) {
		os_sprintf(buff,IPSTR,IP2STR(&ipconfig.ip));
	} else if (os_strcmp(token, "DEVICE")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Device);
	} else if (os_strcmp(token, "OLED")==0) {
		os_strcpy(buff,(uint8_t *) Setup.OLED);
	} else if (os_strcmp(token, "BMP180")==0) {
		os_strcpy(buff,(uint8_t *) Setup.BMP180);
	} else if (os_strcmp(token, "ALTITUDE")==0) {
		os_strcpy(buff,(uint8_t *) Setup.Altitude);
	}
	httpdSend(connData, buff, -1);
}

