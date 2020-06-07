/*
Functions in this file process the input message and prepare the output message
*/

#include <stdint.h>
#include <string.h>
#include "msg.h"
#include "MQTTClient.h"
#include "version.h"
#include "main.h"

extern uint8_t f_clearAlarm;
extern int updateTempThreshold(int8_t value);
extern void SetTempratureThreshold(int8_t value);
extern void StartAlarmLedBlinking(void);

void Parameters_message_handler(MessageData * data)
{
  int8_t* value_temp=NULL;
   MQTTMessage* message = data->message;
  uint16_t message_size = (uint16_t)message->payloadlen;
  *((char*)message->payload + message_size) = '\0';
  
  msg_info("\r\n [D]. MQTT payload received is: \r\n");
  msg_info((char*)message->payload);  
  msg_info("\r\n"); 

  value_temp = (int8_t*)message->payload;
  updateTempThreshold(*value_temp); //update flash
  SetTempratureThreshold(*value_temp);//update device_s
  msg_info("\n received Temprature threshold:%d\n",*value_temp );
}

void Service_message_handler(MessageData * data)
{
   MQTTMessage* message = data->message;
  uint16_t message_size = (uint16_t)message->payloadlen;
  *((char*)message->payload + message_size) = '\0';
  
  msg_info("\r\n [D]. MQTT payload received is: \r\n");
  msg_info((char*)message->payload);  
  msg_info("\r\n"); 
  
  msg_info("\n receive ClearAlarm message\n");

}
