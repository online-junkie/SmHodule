/*
 * cgiSetup.h
 *
 *  Created on: 28.02.2015
 *      Author: K4546
 */

#ifndef USER_CGI_CGISETUP_H_
#define USER_CGI_CGISETUP_H_

#include "httpd.h"

int cgiSetup(HttpdConnData *connData);
void tplSetup(HttpdConnData *connData, char *token, void **arg);

#endif /* USER_CGI_CGISETUP_H_ */
