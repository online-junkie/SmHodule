#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "http/httpd.h"
#include "cgi.h"
#include "io/io.h"
#include "io/gpio16.h"
#include "espmissingincludes.h"
#include "i2c/i2c_bmp180.h"
#include "smhodule.h"


//Cgi that handles setup param in the POST data
int ICACHE_FLASH_ATTR cgiWeather(HttpdConnData *connData) {
	int len;
	char buff[1024];
	SETUPDATA Settings;

	// not required data is already in memory: getSetupData();
	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}


//	httpdFindArg(connData->postBuff, "DP", (uint8_t *) Settings.DP, 16);
//	httpdFindArg(connData->postBuff, "DPTYPE", (uint8_t *) Settings.DPType, 2);

//	os_strcpy((uint8_t *) Setup.isInitialized, "1");
//	os_sprintf((uint8_t *) Setup.Device, "%s", (uint8_t *) Settings.Device);

	httpdRedirect(connData, "/Pressure.tpl");
	return HTTPD_CGI_DONE;
}

//Template code for the weather page.
void ICACHE_FLASH_ATTR tplWeather(HttpdConnData *connData, char *token, void **arg) {
	char buff[1024];
	int x;
	int32_t temperature;
	int32_t pressure;
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");
	if ( atoi(Setup.BMP180) == 1 ) {
    	os_printf("Get temperature and pressure...\r\n");
    	temperature = BMP180_GetTemperature();
    	pressure = BMP180_GetPressure(OSS_0);
    	os_printf("Temperature: %ld\r\n", temperature);
    	os_printf("Pressure: %ld Pa\r\n", pressure);
    	os_printf("Altitude: %ld m\r\n", BMP180_CalcAltitude(pressure)/10);

		if (os_strcmp(token, "TEMPERATURE")==0) {
			os_sprintf(buff,"%ld", temperature);
		} else if (os_strcmp(token, "PRESSURE")==0) {
			os_sprintf(buff,"%ld", pressure);
		} else if (os_strcmp(token, "ALTITUDE")==0) {
			os_sprintf(buff,"%ld", BMP180_CalcAltitude(pressure)/10);
		}
    } else {
    	os_printf("BMP180 not configured...\r\n");
    }
	httpdSend(connData, buff, -1);
}

