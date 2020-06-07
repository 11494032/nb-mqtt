#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "nb_io.h"


#define PORT 1883
#define PORTSTR ",1883,"
#define AT_UPDATA_TIMEOUT 100 //ms

char at_send_buf[1024];
NB_CtrlTypeDef NB = {0};

/*----------------------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
extern int8_t NB_IO_Init(void);
extern void NB_IO_DeInit(void);
extern int8_t NB_IO_Send(char* pData, uint32_t Length);
extern int32_t NB_IO_Receive(uint8_t* Buffer, uint32_t Length);

/*----------------------------------------------------------------------------*/

NB_Status_t NB_Init(void)
{
  int ret;
  // data buf malloc
  NB.process_data = malloc(NB_IO_FIFO_SIZE);
  if(!NB.process_data)
  {
    printf("malloc io_fifo fail!\r\n");
    return NB_STATUS_ERROR;
  }
  
  ret = kfifo_malloc(&NB.io_fifo, NB_IO_FIFO_SIZE);
  if(0 != ret)
  {
    printf("malloc io_fifo fail!\r\n");
    return NB_STATUS_ERROR;
  }
  
  ret = kfifo_malloc(&NB.data_fifo, NB_DATA_FIFO_SIZE);
  if(0 != ret)
  {
    printf("malloc data_fifo fail!\r\n");
    return NB_STATUS_ERROR;
  }
  
  //uart init
  ret = NB_IO_Init();
  if(0 != ret)
  {
    printf("NB_IO_Init fail!\r\n");
    return NB_STATUS_ERROR;
  }
  while(1) {
    //NB module init
    ret = nbiot_module_init_manual();
    if(NB_STATUS_OK != ret)
      continue;
    //NB module network check
    ret = nbiot_module_netcheck(20000);
    if(NB_STATUS_ONLINE != ret)
      continue;
    else
      break;
  }
  return NB_STATUS_OK;
}

void nbiot_revc_data_process(int argc, char *argv[])
{
  
}

static void nbiot_send_at_cmd(char *pcCmd)
{	
  NB_IO_Send(pcCmd, strlen(pcCmd));
  printf("%s",(const char *)pcCmd);
}

NB_Status_t nbiot_recv_at_resp(uint16_t check_sta, uint32_t timeout)
{
  
  
}

NB_Status_t nbiot_module_init_manual(void)//初始化及附网
{
  NB_Status_t atRet;
//  int iLen = 0;
//  int iTry = 0;
  
  nbiot_send_at_cmd("ATI\r\n");
  
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  nbiot_send_at_cmd("AT+NCONFIG=AUTOCONNECT,FALSE\r\n");//关闭自动找网
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  nbiot_send_at_cmd("AT+NRB\r\n");//重启
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 20000);
  if(NB_STATUS_OK != atRet) return atRet;

  nbiot_send_at_cmd("AT+CGSN=1\r\n");//查询IMEI号
  atRet = nbiot_recv_at_resp(NB_STATUS_IMEI_OK, 1000);
  
  nbiot_send_at_cmd("AT+NBAND=8\r\n");//移动的频段
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  nbiot_send_at_cmd("AT+QLEDMODE=1\r\n");//打开板子的调试灯
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  nbiot_send_at_cmd("AT+CFUN=1\r\n");//开启射频等功能
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 5000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  nbiot_send_at_cmd("AT+NSONMI=2\r\n");//接收到TCP数据时，自动上报
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  nbiot_send_at_cmd("AT+CEDRXS=0,5\r\n");//stop eDRX
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
   
  nbiot_send_at_cmd("AT+CPSMS=0\r\n");//stop psm
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  if(NB_STATUS_OK != atRet) return atRet;
  
  printf("Check SIM...\n");
  do{
    nbiot_send_at_cmd("AT+CIMI\r\n");//查询 卡的国际 识别码
    atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
  }while(NB_STATUS_OK != atRet);
  
  nbiot_send_at_cmd("AT+CGATT=1\r\n");//附着网络
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 500);
  if(NB_STATUS_OK != atRet) return atRet;
  
  return atRet;
}

NB_Status_t nbiot_module_netcheck(uint32_t timeout)//检查附网状态
{
  NB_Status_t atRet;
 
  
  //Query whether the network has been activated.
  //	nbiot_send_at_cmd("AT+CSQ\r\n");//获取信号强度
  //	atRet = nbiot_recv_at_resp(NB_STATUS_OK, 2000);
  //        if(NB_STATUS_OK != atRet) return atRet;
  for(;;)
  {
    nbiot_send_at_cmd("AT+CGATT?\r\n");//查询附着网络 是否成功
    atRet = nbiot_recv_at_resp(NB_STATUS_ONLINE, 2000);
    if(NB_STATUS_ONLINE == atRet)
    {
      return atRet;
    }
 
  }
}

int net_get_imei(uint8_t *imei)
{
  if(NB.sta | NB_STATUS_IMEI_OK)
  {
    memcpy(imei, NB.imei, 15);
    return 0;
  }
  else
    return -1;
}

static void parse_number_from_string(uint8_t* string, uint8_t* number_p, uint8_t* separate_mark, uint8_t* end_mark)
{
     static uint32_t value=0;
    while((*string)!=*end_mark)
    {
       value = value*10 + (*string-'0');
       string++;
     if(*string==*separate_mark)
     {
       *(number_p) = value;
       number_p++;
       string++;
       value=0;
     }
    };
    *(number_p) = value;
}

NB_Status_t nbiot_domain_name_resolution(char *domainName, char *ip)//域名解析
{
//  int  j = 0;
  NB_Status_t atRet = NB_STATUS_TIMEOUT;
#if 0
  memset(at_send_buf, 0, sizeof(at_send_buf));
  sprintf(at_send_buf, "AT+QDNS=0,%s\r\n", domainName);//解析域名
  nbiot_send_at_cmd(at_send_buf);
  
  atRet = nbiot_recv_at_resp(NB_STATUS_QDNS_OK, 600000);
  if(NB_STATUS_QDNS_OK == atRet)
  {
    strcpy(ip, (char const *)NB.server_ip);
  }
//测试
#else
  memcpy(ip, "106.15.100.2", 16);
  memcpy(NB.server_ip, "106.15.100.2", 16);
  return NB_STATUS_QDNS_OK;
#endif
  return atRet;
}

//int nbiot_udp_create(int iPort)
//{
//  NB_Status_t atRet = NB_STATUS_TIMEOUT;
//  int fd = -1;
//  
//  memset(at_send_buf, 0, sizeof(at_send_buf));
//  sprintf(at_send_buf, "AT+NSOCR=DGRAM,17,%d,1\r\n", iPort);
//  nbiot_send_at_cmd(at_send_buf);
//  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 1000);
//  if(NB_STATUS_OK != atRet) return fd;
//  if(NB_STATUS_OK == atRet)
//    fd = 0;
//  return fd;
//}
//
//NB_Status_t nbiot_udp_send(char *pcIP, int iPort, int socketId, unsigned char *pucData, int iLen, int iTimeout)
//{
//  NB_Status_t atRet = NB_STATUS_TIMEOUT;
//  int i = 0, iDlen = 0;
//  
//  for(i=0; i<iLen; i++){
//    sprintf(&at_send_buf[iDlen], "%02X", pucData[i]);
//    iDlen += 2;
//    if(iDlen >= sizeof(at_send_buf)) break;
//  }
//  
//  iDlen >>=1;
//  snprintf(at_send_buf, sizeof(at_send_buf), "AT+NSOST=%d,%s,%d,%d,%s\r\n", 
//           socketId, pcIP, iPort, iDlen, at_send_buf);
//  
//  nbiot_send_at_cmd(at_send_buf);
//  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 10000);
//  return atRet;
//}

int nbiot_tcp_create(void)
{       
  memset(at_send_buf, 0, sizeof(at_send_buf));
  sprintf(at_send_buf, "AT+NSOCR=STREAM,6,0\r\n");//创建TCP
  
  //printf("NB.sta:%04x\r\n", NB.sta);
  nbiot_send_at_cmd(at_send_buf);
  
  if(NB_STATUS_OK == nbiot_recv_at_resp(NB_STATUS_OK, 5000))
    return NB.socket_id;
  else
    return -1;
}

NB_Status_t nbiot_tcp_connect(int fd, char *pcIP, int iPort)
{
  NB_Status_t atRet = NB_STATUS_TIMEOUT;
  
  memset(at_send_buf, 0, sizeof(at_send_buf));
  sprintf(at_send_buf, "AT+NSOCO=%d,%s,%d\r\n", fd, pcIP, iPort);
  nbiot_send_at_cmd(at_send_buf);
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, 5000);
  
  return atRet;
}

int nbiot_tcp_send(int socketId, unsigned char *pucData, int iLen, int iTimeout)
{
  NB_Status_t atRet = NB_STATUS_TIMEOUT;
  int i = 0, iDlen = 0;
  
  memset(at_send_buf, 0, sizeof(at_send_buf));
  
  for(i=0; i<iLen; i++){
    sprintf(&at_send_buf[iDlen+64], "%02X", pucData[i]);
    iDlen += 2;
    if(iDlen >= sizeof(at_send_buf)) break;
  }
  
  printf("[%d]%s\n", iDlen, at_send_buf);
  iDlen >>=1;
  snprintf(at_send_buf, sizeof(at_send_buf), "AT+NSOSD=%d,%d,%s\r\n", 
           NB.socket_id, iDlen, at_send_buf+64);
  //72D764E1D91C -- 114.215.100.225:7385
  
  nbiot_send_at_cmd(at_send_buf);
  
  atRet = nbiot_recv_at_resp(NB_STATUS_OK, iTimeout);

  return atRet;
}


NB_Status_t nbiot_data_recv(int iFd, int piRlen, unsigned char *pcOut, int *RcvDatalen, int iTimeout)
{
  NB_Status_t atRet = NB_STATUS_TIMEOUT;
  int i = 0, iDlen = piRlen*2;
  uint32_t data_len = 0;
  
  for(;;)
  {
    data_len = kfifo_getlen(&NB.data_fifo);
    if(data_len >= iDlen)
      break;
    else
    {
      
    }
  }
  
  //copy data
  *RcvDatalen = kfifo_out(&NB.data_fifo, pcOut, iDlen)/2;
  
  for(i = 0; i < piRlen; i++)
  {
              printf("%c %c ", pcOut[2*i], pcOut[2*i+1]);
    if((pcOut[2*i] >= '0') && (pcOut[2*i] <= '9'))
    {
      pcOut[i] = (pcOut[2*i] - '0')*16;
    }
    else
    {
      pcOut[i] = (pcOut[2*i] - 'A' + 10)*16;
    }
    
    if((pcOut[2*i+1] >= '0') && (pcOut[2*i+1] <= '9'))
    {
      pcOut[i] += pcOut[2*i+1] - '0';
    }
    else
    {
      pcOut[i] += pcOut[2*i+1] - 'A' + 10;
    }
             printf("%02x\n", pcOut[i]);
  }
  printf("nbiot_data_recv len:%d\n", piRlen);
  atRet = NB_STATUS_OK;
  return atRet;
}

NB_Status_t nbiot_close_socket(int iFd)
{
  NB_Status_t ret = NB_STATUS_TIMEOUT;
  
  memset(at_send_buf, 0, sizeof(at_send_buf));
  sprintf(at_send_buf, "AT+NSOCL=%d\r\n", iFd);
  
  nbiot_send_at_cmd(at_send_buf);
  ret = nbiot_recv_at_resp(NB_STATUS_OK, 500);
  return ret;
}
