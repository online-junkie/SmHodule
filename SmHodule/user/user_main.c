

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include "ets_sys.h"
#include "osapi.h"
#include "espmissingincludes.h"
#include "http/httpd.h"
#include "io/io.h"
#include "httpdespfs.h"
#include "cgi/cgi.h"
#include "cgi/cgiSetup.h"
#include "cgi/cgiwifi.h"
#include "cgi/cgiWeather.h"
#include "tty/stdout.h"
#include "http/auth.h"
#include "wifi/mac.h"
#include "user_interface.h"
#include "smhodule.h"

//Callback Function that tells the authentication system what users/passwords live on the system.
//This is disabled in the default build; if you want to try it, enable the authBasic line in
//the builtInUrls below.
int myPassFn(HttpdConnData *connData, int no, char *user, int userLen, char *pass, int passLen) {
	if (no==0) {
		os_sprintf(user, "%s", (uint8_t *) Setup.User);
		os_sprintf(pass, "%s", (uint8_t *) Setup.Pass);
		return 1;
	}
	return 0;
}

/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	{"/", cgiRedirect, "/index.tpl"},
	{"/index.tpl", cgiEspFsTemplate, tplCounter},
	{"/weather.tpl", cgiEspFsTemplate, tplWeather},
	{"/api/set", cgiApi, NULL},
	//Routines to make the /wifi URL and everything beneath it work.

//Enable the line below to protect the WiFi configuration with an username/password combo.
	{"/setup/*", authBasic, myPassFn},
	{"/setup", cgiRedirect, "/setup/setup.tpl"},
	{"/setup/", cgiRedirect, "/setup/setup.tpl"},
	{"/setup/setup.tpl", cgiEspFsTemplate, tplSetup},
	{"/setup/setup.cgi", cgiSetup, NULL},
	{"/setup/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/setup/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/setup/reset.cgi", cgiWiFiApReset, NULL},
	{"/setup/connect.cgi", cgiWiFiConnect, NULL},
	{"/setup/setmode.cgi", cgiWifiSetMode, NULL},

	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};

void user_rf_pre_init(void) {};

void user_init(void) {
	stdoutInit();
	os_delay_us(500000);
	os_printf("\n");
#ifdef PLATFORM_DEBUG
	os_printf("max heap available at start: %u\r\n",system_get_free_heap_size());
#endif
	getSetupData();
	httpdInit(builtInUrls, 80);
	smhoduleInit();
	os_printf("%s is ready\n",(char *) getSmHoduleName());
}
