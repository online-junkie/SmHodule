#ifndef __IP_LINK_H
#define __IP_LINK_H

#include "ipconfig.h"
#include "espconn.h"
#include "smhodule.h"


/* Protocol family and type of the espconn  */
typedef enum  {
    INVALID    = 0,
    TCP        = 0x10,
    UDP        = 0x20
} espTypes;

typedef struct
{
  BOOL linkEn;
  BOOL teToff;
  uint8_t linkId;
  teType teType;
  uint8_t repeaTime;
  struct espconn *pCon;
} espConnectionType;

typedef void(*tcp_send_cb)(void);

void TcpSend(espTypes type, char * ipAddress, int32_t port, char * cmd, tcp_send_cb send_cb);
//void TcpClient_Dns_Cb(const char *name, ip_addr_t *ipaddr, void *arg);
//static void TcpClient_Connect_Cb(void *arg);
//static void TcpClient_Reconnect_Cb(void *arg, sint8 errType);
//static void TcpClient_Disconnect_Cb(void *arg);
//static void TcpClient_Send_Cb(void *arg);
//static void  TcpClient_Receive(void *arg, char *pdata, unsigned short len);


#endif
