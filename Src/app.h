
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

#endif /*APP_H */

/***********END OF FILE****/
