#include "osapi.h"
#include "user_interface.h"
#include "user_config.h"
#include "wifi.h"
#include "wifi/mac.h"
#include "smhodule.h"
#include "espmissingincludes.h"

const char *WiFiMode[] =
{
		"NULL",		// 0x00
		"STATION",	// 0x01
		"SOFTAP", 	// 0x02
		"STATIONAP"	// 0x03
};

void ICACHE_FLASH_ATTR wifiInit()
{
	if(wifi_get_opmode() != USE_WIFI_MODE)
	{
		#ifdef PLATFORM_DEBUG
		os_printf("%s not in %s mode, restarting in %s mode...\r\n",(char *) getSmHoduleName(), WiFiMode[USE_WIFI_MODE], WiFiMode[USE_WIFI_MODE]);
		#endif
		if(USE_WIFI_MODE & ( SOFTAP_MODE | STATIONAP_MODE))
			setup_wifi_ap_mode();
		if(USE_WIFI_MODE & ( STATION_MODE | STATIONAP_MODE))
			setup_wifi_st_mode();
	}
	if(wifi_get_phy_mode() != PHY_MODE_11N)
		wifi_set_phy_mode(PHY_MODE_11N);
	if(wifi_station_get_auto_connect() == 0)
		wifi_station_set_auto_connect(1);

	#ifdef PLATFORM_DEBUG
	os_printf("Wi-Fi mode: %s\r\n", WiFiMode[wifi_get_opmode()]);
	if(USE_WIFI_MODE & (SOFTAP_MODE | STATIONAP_MODE))
	{
		struct softap_config apConfig;
		if(wifi_softap_get_config(&apConfig)) {
			os_printf("AP config: SSID: %s, PASSWORD: %s\r\n",
				apConfig.ssid,
				apConfig.password);
		}
	}
	if(USE_WIFI_MODE & (STATION_MODE | STATIONAP_MODE))
	{
		struct station_config stationConfig;
		if(wifi_station_get_config(&stationConfig)) {
			os_printf("STA config: SSID: %s, PASSWORD: %s\r\n",
				stationConfig.ssid,
				stationConfig.password);
		}
	}
	#endif
}

void setup_wifi_ap_mode(void)
{
//	wifi_set_opmode((wifi_get_opmode()|SOFTAP_MODE)&STATIONAP_MODE);
//	wifi_set_opmode(SOFTAP_MODE);
	struct softap_config apconfig;
	if(wifi_softap_get_config(&apconfig))
	{
		wifi_softap_dhcps_stop();
		memset(apconfig.ssid, 0, sizeof(apconfig.ssid));
		memset(apconfig.password, 0, sizeof(apconfig.password));
		apconfig.ssid_len = os_sprintf(apconfig.ssid, "%s",(char *) getSmHoduleName());
		os_sprintf(apconfig.password, "%s", Setup.WifiApPassword);
		apconfig.authmode = AUTH_WPA_WPA2_PSK;
		apconfig.ssid_hidden = 0;
		apconfig.channel = 7;
		apconfig.max_connection = 255;
		if(!wifi_softap_set_config(&apconfig))
		{
			#ifdef PLATFORM_DEBUG
			os_printf("%s not set AP config!\r\n",(char *) getSmHoduleName());
			#endif
		};
		struct ip_info ipinfo;
		wifi_get_ip_info(SOFTAP_IF, &ipinfo);
		IP4_ADDR(&ipinfo.ip, 192, 168, 4, 1);
		IP4_ADDR(&ipinfo.gw, 192, 168, 4, 1);
		IP4_ADDR(&ipinfo.netmask, 255, 255, 255, 0);
		wifi_set_ip_info(SOFTAP_IF, &ipinfo);
		wifi_softap_dhcps_start();
	}
	#ifdef PLATFORM_DEBUG
	os_printf("%s in AP mode configured. ssid:%s, password:%s\r\n",(char *) getSmHoduleName(),apconfig.ssid,apconfig.password);
	#endif
}

void setup_wifi_st_mode(void)
{
//	wifi_set_opmode((wifi_get_opmode()|STATION_MODE)&STATIONAP_MODE);
	struct station_config stconfig;
	wifi_station_disconnect();
	wifi_station_dhcpc_stop();
	if(wifi_station_get_config(&stconfig))
	{
		memset(stconfig.ssid, 0, sizeof(stconfig.ssid));
		memset(stconfig.password, 0, sizeof(stconfig.password));
		os_sprintf(stconfig.ssid, "AP%s",(char *) getSmHoduleName());
		os_sprintf(stconfig.password, "%s", Setup.WifiStPassword);
		if(!wifi_station_set_config(&stconfig))
		{
			#ifdef PLATFORM_DEBUG
			os_printf("%s not set station config!\r\n",(char *) getSmHoduleName());
			#endif
		}
	}
	wifi_station_connect();
	wifi_station_dhcpc_start();
	wifi_station_set_auto_connect(1);
	#ifdef PLATFORM_DEBUG
	os_printf("%s in STA mode configured.\r\n",(char *) getSmHoduleName());
	#endif
}

