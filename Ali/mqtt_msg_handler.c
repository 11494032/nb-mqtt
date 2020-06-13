/*
Functions in this file process the input message and prepare the output message
*/

#include <stdint.h>
#include <string.h>
#include "msg.h"
#include "MQTTClient.h"
#include "version.h"
#include "main.h"

void Parameters_message_handler(MessageData * data)
{
   MQTTMessage* message = data->message;
  uint16_t message_size = (uint16_t)message->payloadlen;
  *((char*)message->payload + message_size) = '\0';
  
  msg_info("\r\n [D]. MQTT payload received is: \r\n");
  msg_info((char*)message->payload);  
  msg_info("\r\n"); 
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
