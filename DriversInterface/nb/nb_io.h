#ifndef _NB_IOT_H_
#define _NB_IOT_H_

#include "kfifo.h"
#include <stdint.h>
#include "stm32l1xx.h"
/* Exported constants --------------------------------------------------------*/
#define NB_MAX_CONNECTIONS          6
#define NB_MAX_MODULE_NAME          100
#define NB_MAX_CONNECTED_STATIONS   2
#define NB_MSG_JOINED               1                                    
#define NB_MSG_ASSIGNED             2  
#define NB_PAYLOAD_SIZE             1200
#define NB_PRODUCT_INFO_SIZE        20 
#define NB_WRITE_TIMEOUT            30000
#define NB_READ_TIMEOUT             4000
#define NB_READ_RETRY_TIME          1

#define NB_IO_FIFO_SIZE             (256)//必须为2的次幂
#define NB_DATA_FIFO_SIZE           (256)//必须为2的次幂

typedef enum{
  NB_STATUS_OK          = 0x0001,
  NB_STATUS_TIMEOUT     = 0x0002,
  NB_STATUS_ERROR       = 0x0004,
  NB_STATUS_ONLINE      = 0x0008,
  NB_STATUS_SOCKET_ON   = 0x0010,
  NB_STATUS_QDNS_OK     = 0x0020,
  NB_STATUS_IMEI_OK     = 0x0040,
}NB_Status_t;

typedef enum {
  NB_TCP_PROTOCOL = 0,
  NB_UDP_PROTOCOL = 1,
}NB_Protocol_t;

typedef struct {
  volatile uint16_t sta;
  struct kfifo      io_fifo;
  uint8_t           *process_data;
  struct kfifo      data_fifo;
  uint8_t           server_ip[20];
  uint8_t           imei[16];
  volatile uint32_t io_updata_time;
  volatile uint32_t io_updata_sign;
  volatile uint32_t socket_id;
}NB_CtrlTypeDef;

NB_Status_t nbiot_module_init_manual(void);//初始化及附网
NB_Status_t nbiot_module_netcheck(uint32_t timeout);//检查附网状态
NB_Status_t nbiot_domain_name_resolution(char *domainName, char *ip);//域名解析
int nbiot_tcp_create(void);
NB_Status_t nbiot_tcp_connect(int fd, char *pcIP, int iPort);
int nbiot_tcp_send(int socketId, unsigned char *pucData, int iLen, int iTimeout);
NB_Status_t nbiot_data_recv(int iFd, int piRlen, unsigned char *pcOut, int *RcvDatalen, int iTimeout);
NB_Status_t nbiot_close_socket(int iFd);
void nbiot_revc_data_process(int argc, char *argv[]);
NB_Status_t NB_Init(void);
int net_get_imei(uint8_t *imei);
#endif

