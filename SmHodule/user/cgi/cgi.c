/*
Some random cgi routines. Used in the LED example and the page that returns the entire
flash as a binary. Also handles the hit counter on the main page.
*/

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "http/httpd.h"
#include "cgi.h"
#include "io/gpio16.h"
#include "wifi/mac.h"
#include "io/io.h"
#include <ip_addr.h>
#include "smhodule.h"
#include "espmissingincludes.h"


static long hitCounter=0;

int ICACHE_FLASH_ATTR cgiApi(HttpdConnData *connData) {
	char payload[200];
	char value[10];
	char set[20];
	char key[20];
	char duration[20];
	char mode[20];
	if (connData->conn==NULL) {
			//Connection aborted. Clean up.
			return HTTPD_CGI_DONE;
	}

	httpdFindArg(connData->getArgs, "switch", set, sizeof(set));
	httpdFindArg(connData->getArgs, "key", key, sizeof(key));
	httpdFindArg(connData->getArgs, "duration", duration, sizeof(duration));
	httpdFindArg(connData->getArgs, "mode", mode, sizeof(mode));

	if ( atoi(key) == 1) {
		if ( gpio16_input_get() ) {
			os_strcpy(value,"off");
			ioGPIO16(0);
			sendStatus(0);
		} else {
			os_strcpy(value,"on");
			ioGPIO16(1);
			sendStatus(1);
		}
		os_printf("gpio16:%s\n",value);
	}
/*	} else if ( atoi(set) == 0 || os_strstr(set, "off") ) {
		os_printf("gpio16:off\n");
		ioGPIO16(0);
		sendStatus(0);
	}
	os_delay_us(50000);
	*/
	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);
	os_sprintf(payload,"{\"SmHodule\":{\"Name\":%s,\"gpio16\":%s}}",getSmHoduleName(),value);
	httpdSend(connData, payload, os_strlen(payload));
	return HTTPD_CGI_DONE;
}

//Template code for the counter on the index page.
void ICACHE_FLASH_ATTR tplCounter(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "");
	if (os_strcmp(token, "counter")==0) {
		hitCounter++;
		os_sprintf(buff, "%ld", hitCounter);
	} else if (os_strcmp(token, "WEATHER")==0 && (atoi((uint8_t *) Setup.BMP180) == 1) ) {
		os_sprintf(buff, "Barometric pressure data: <a href=\"weather.tpl\">Weather</a>");
	}

	httpdSend(connData, buff, os_strlen(buff));
}
