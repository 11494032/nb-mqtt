
#ifndef APP_H
#define APP_H

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdbool.h>
#include "stm32l1xx.h"
#include "stm32l1xx_hal.h"
#include "msg.h"
#include "net.h"

void initApplication(void);
int initPlatform(void);
int net_if_init(void * if_ctxt);

extern net_hnd_t         hnet;
extern char *product_key;
extern char *product_secret;
extern char *device_name;
extern char *device_secret;
extern char *RegionId;


#if defined(USE_WIFI)
#define NET_IF  NET_IF_WLAN
#elif defined(USE_NB)
#define NET_IF  NET_IF_NB
#elif defined(USE_LWIP)
#define NET_IF  NET_IF_ETH
#elif defined(USE_C2C)
#define NET_IF  NET_IF_C2C
#endif

#endif /*APP_H */

/***********END OF FILE****/
