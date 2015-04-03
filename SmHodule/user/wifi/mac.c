/*
 * mac.c
 *
 *  Created on: 26.02.2015
 *      Author: K4546
 */

#include "osapi.h"
#include "mac.h"
#include "user_interface.h"
#include "espmissingincludes.h"

char* ICACHE_FLASH_ATTR getMacAddress(void) {

	static char temp[18];	// buffer for formatting the output (xx:xx:xx:xx:xx:xx\r\n) = 20 bytes
	uint8_t macaddr[6];		// array that will receive the MAC address from the chip

	wifi_get_macaddr(0x00, macaddr);
	os_sprintf(temp, MACSTR, macaddr[0],
			macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	return temp;
}

char* ICACHE_FLASH_ATTR getSmHoduleName(void) {
	static char SmHoduleName[18];
	uint8_t macaddr[6];		// array that will receive the MAC address from the chip
	wifi_get_macaddr(0x00, macaddr);
	os_sprintf(SmHoduleName, "SmHodule-%02x%02x%02x", macaddr[3], macaddr[4], macaddr[5]);
	return SmHoduleName;
}
