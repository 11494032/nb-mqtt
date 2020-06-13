
#include "stm32l1xx_hal.h"
#include "app.h"
#include "paho_timer.h"
#include "Ali_iotclient.h"  
#include "msg.h"
#include "usart.h"

#define  PERIOD_VALUE       (uint32_t)(50000 - 1)  /* Period Value  */
#define  PULSE1_VALUE       PERIOD_VALUE/2   /* Capture Compare 1 Value  */
#define  PULSE2_VALUE       PERIOD_VALUE   /* Capture Compare 1 Value  */

void initApplication(void);
int initPlatform(void);
char *product_key       = "a13FN5TplKq";
char *product_secret    = "y7GSILD480lBSsP8";
char *device_name       = "dynreg_basic_demo";
char *device_secret			= "ssssssssssss";
char *RegionId				  = "";

int net_if_init(void * if_ctxt);

net_hnd_t         hnet;
Timer   regular_pub_timer;
/**
  * @brief  The application entry point.
  * @retval int
  */
int app(void)
{
  
  int ret;
 
  /* USER CODE BEGIN 2 */
  msg_info("\n");
  msg_info("**************************************************************************\n");
  msg_info("***                   STM32 based AliIoT Client Demo                   ***\n");
  msg_info("***                            Without TLS                             ***\n");
  msg_info("***              FW version %d.%d.%d - %s, %s              ***\n", 
           FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH, __DATE__, __TIME__);
  msg_info("**************************************************************************\n");
  
  //WIFI SSID/passowrd config and initialization
  //and config device certificate
  ret = initPlatform();
  if(ret!=0)
  {
    msg_info("nb initial failed!\n");
    while(1);
  }
  //connect to Ali Iot platform
  ret = connect2Aliiothub();
  if(ret!=0)
  {
    msg_info("MQTT connection failed!\n");
    while(1);
  }
  //subscribe to topics
  deviceSubscribe();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    //MQTT reconnection check
    if(rebuildMQTTConnection()==0)
    {
      doMqttYield();
   
  
      //check if regular_pub_timer expired
      //prepared data package, and send to cloud
      if(TimerIsExpired(&regular_pub_timer))
      {
				printf("--\n");
      }
    }
    else
    {
      //delay 5s then try to connect to server again
      HAL_Delay(5000);
    }
  }
  /* USER CODE END 3 */
}




int initPlatform(void)
{
  if (net_init(&hnet, NET_IF, (net_if_init)) != NET_OK)
  {
    //CLOUD_Error_Handler(CLOUD_DEMO_IP_ADDRESS_ERROR);
    return -1;
  }

  build_mqtt_topic();
  return 0;
}

#if (defined(__GNUC__) && !defined(__CC_ARM))
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart3, (uint8_t *) &ch, 1, 1000);
  return ch;
}

/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. read a character on USART and loop until the end of read */
  uint8_t ch = 0;
  HAL_UART_Receive(&huart3, (uint8_t *)&ch, 1, 1000);
  return ch;
}

