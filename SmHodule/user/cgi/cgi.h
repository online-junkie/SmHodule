#ifndef CGI_H
#define CGI_H

#include "httpd.h"

int cgiApi(HttpdConnData *connData);
void tplCounter(HttpdConnData *connData, char *token, void **arg);

#endif
