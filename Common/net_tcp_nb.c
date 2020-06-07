/**
  ******************************************************************************
  * @file    net_tcp_nb.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    04-September-2017
  * @brief   Network abstraction at transport layer level. TCP implementation
  *          on ST NB connectivity API.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "net_internal.h"
#include "nb_io.h"
#include <string.h>

#ifdef USE_NB

/* Private defines -----------------------------------------------------------*/
/* The socket timeout of the non-blocking sockets is supposed to be 0.
 * But the underlying component does not necessarily supports a non-blocking
 * socket interface.
 * The NOBLOCKING timeouts are intended to be set to the lowest possible value
 * supported by the underlying component. */
#define NET_DEFAULT_NOBLOCKING_WRITE_TIMEOUT  1
#define NET_DEFAULT_NOBLOCKING_READ_TIMEOUT   1

/* Workaround for the incomplete NB_LL API */
#ifdef ES_NB_MAX_SSID_NAME_SIZE
#define NB_PRODUCT_INFO_SIZE                      ES_NB_MAX_SSID_NAME_SIZE
#define NB_PAYLOAD_SIZE                           ES_NB_PAYLOAD_SIZE
#endif

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
int net_sock_create_tcp_nb(net_hnd_t nethnd, net_sockhnd_t * sockhnd, net_proto_t proto);
int net_sock_open_tcp_nb(net_sockhnd_t sockhnd, const char * hostname, int dstport);
int net_sock_recv_tcp_nb(net_sockhnd_t sockhnd, uint8_t * buf, size_t len);
int net_sock_send_tcp_nb(net_sockhnd_t sockhnd, const uint8_t * buf, size_t len);
int net_sock_close_tcp_nb(net_sockhnd_t sockhnd);
int net_sock_destroy_tcp_nb(net_sockhnd_t sockhnd);

/* Functions Definition ------------------------------------------------------*/

int net_sock_create_tcp_nb(net_hnd_t nethnd, net_sockhnd_t * sockhnd, net_proto_t proto)
{
  int rc = NET_ERR;
  net_ctxt_t *ctxt = (net_ctxt_t *) nethnd;
  net_sock_ctxt_t *sock = NULL;
  
  sock = net_malloc(sizeof(net_sock_ctxt_t));
  if (sock == NULL)
  {
    msg_error("net_sock_create allocation failed.\n");
    rc = NET_ERR;
  }
  else
  {
    memset(sock, 0, sizeof(net_sock_ctxt_t));
    sock->net = ctxt;
    sock->next = ctxt->sock_list;
    sock->methods.create  = (net_sock_create_tcp_nb);
    sock->methods.open    = (net_sock_open_tcp_nb);
    sock->methods.recv    = (net_sock_recv_tcp_nb);
    sock->methods.send    = (net_sock_send_tcp_nb);
    sock->methods.close   = (net_sock_close_tcp_nb);
    sock->methods.destroy = (net_sock_destroy_tcp_nb);
    sock->proto           = proto;
    sock->blocking        = NET_DEFAULT_BLOCKING;
    sock->read_timeout    = NET_DEFAULT_BLOCKING_READ_TIMEOUT;
    sock->write_timeout   = NET_DEFAULT_BLOCKING_WRITE_TIMEOUT;
    ctxt->sock_list       = sock; /* Insert at the head of the list */
    *sockhnd = (net_sockhnd_t) sock;

    rc = NET_OK;
  }
  
  return rc;
}


int net_sock_open_tcp_nb(net_sockhnd_t sockhnd, const char * hostname, int dstport)
{
  int rc = NET_ERR;
  net_sock_ctxt_t *sock = (net_sock_ctxt_t * ) sockhnd;

  
  sock->underlying_sock_ctxt = (net_sockhnd_t) -1; /* Initialize to a non-null value which may not be confused with a valid port number. */
  
  /* Find a free underlying socket on the network interface. */
//  bool underlying_socket_busy[NB_MAX_CONNECTIONS];
//  memset(underlying_socket_busy, 0, sizeof(underlying_socket_busy));
  
//  net_sock_ctxt_t * cur = sock->net->sock_list;
//  do 
//  {
//    if ((cur->proto == NET_PROTO_TCP) && ((int) cur->underlying_sock_ctxt >= 0) )
//    {
//      underlying_socket_busy[(int) cur->underlying_sock_ctxt] = true;
//    }
//    cur = cur->next;
//  } while (cur != NULL);
  
//  for (int i = 0; i < NB_MAX_CONNECTIONS; i++)
//  {
//    if (underlying_socket_busy[i] == false)
//    {
//      sock->underlying_sock_ctxt = (net_sockhnd_t) i;
//      break;
//    }
//  }
  
  /* Free socket found */
//  if (sock->underlying_sock_ctxt >= 0 && (sock->underlying_sock_ctxt) < (net_sockhnd_t)NB_MAX_CONNECTIONS) //@sz
//  {
    if (nbiot_domain_name_resolution((char *)hostname, (char *)sock->remote_ip) != NB_STATUS_QDNS_OK)
    {
      // TODO: Defect report on NB_GetHostAddress() which return code is not informative.
      // NB: This blocking call may take several seconds before returning. An asynchronous interface should be added.
      msg_info("The address of %s could not be resolved.\n", hostname);
      rc = NET_ERR;
    }
    else
    {
      // creat socket
      sock->underlying_sock_ctxt = (net_sockhnd_t)nbiot_tcp_create();
      // connect
      if( NB_STATUS_OK == nbiot_tcp_connect((int) sock->underlying_sock_ctxt, (char *)sock->remote_ip, dstport))
      {
        rc = NET_OK;
      }
      else
      {
//        underlying_socket_busy[(int) sock->underlying_sock_ctxt] = false;
        msg_error("Failed opening the underlying Wifi socket %d.\n", (int) sock->underlying_sock_ctxt);
        sock->underlying_sock_ctxt = (net_sockhnd_t) -1;
        rc = NET_ERR;
      }
    }
//  }
//  else
//  {
//    msg_error("Could not find a free socket on the specified network interface.\n");
//    rc = NET_PARAM;
//  }
  
  return rc;
}


int net_sock_recv_tcp_nb(net_sockhnd_t sockhnd, uint8_t * buf, size_t len)
{
  int rc = 0;
  NB_Status_t status = NB_STATUS_OK;
  net_sock_ctxt_t *sock = (net_sock_ctxt_t * ) sockhnd;
  uint32_t tmp_len = MIN(len, NB_PAYLOAD_SIZE);
  uint32_t read = 0;
  uint8_t * tmp_buf = buf;
  uint32_t start_time = HAL_GetTick();
    
  /* Read the received payload by chuncks of NB_PAYLOAD_SIZE bytes because of
   * a constraint of NB_ReceiveData(). */
  do
  {
    if ( (sock->blocking == true) && (net_timeout_left_ms(start_time, HAL_GetTick(), sock->read_timeout) <= 0) )
    {
      rc = NET_TIMEOUT;
      break;
    }
    status = nbiot_data_recv((uint8_t) ((uint32_t)sock->underlying_sock_ctxt & 0xFF), tmp_len, tmp_buf, &read, 
                             (sock->blocking == true) ? sock->read_timeout : NET_DEFAULT_NOBLOCKING_READ_TIMEOUT);
    msg_debug("Read %d/%d.\n", read, tmp_len);
//    printf("Read %d/%d.\n", read, tmp_len);
    if(NB_STATUS_TIMEOUT == status)
    {
      rc = NET_TIMEOUT;
      break;
    }
    if (status != NB_STATUS_OK)
    {
      msg_error("net_sock_recv(): error %d in NB_ReceiveData() - socket=%d requestedLen=%d received=%d\n",
             status, (int) sock->underlying_sock_ctxt, tmp_len, read);
      msg_error("The port is likely to have been closed by the server.\n")
      rc = NET_EOF;
      break;  
    }
    else
    {
      if (read > tmp_len)
      {
        msg_error("NB_ReceiveData() returned a longer payload than requested (%d/%d).\n", read, tmp_len);
        rc = NET_ERR;
        break;
      }
      tmp_buf += read;
      tmp_len = MAX(0, MIN(len - (tmp_buf - buf), NB_PAYLOAD_SIZE));
    }
    
  } while ( (read == 0) && (sock->blocking == true) && (rc == 0) );
    
  return (rc < 0) ? rc : tmp_buf - buf;
}


int net_sock_send_tcp_nb( net_sockhnd_t sockhnd, const uint8_t * buf, size_t len)
{
  int rc = 0;
  NB_Status_t status = NB_STATUS_OK;
  net_sock_ctxt_t *sock = (net_sock_ctxt_t * ) sockhnd;
  uint16_t sent = 0;
  uint32_t start_time = HAL_GetTick();
  
  do
  {
    if ( (sock->blocking == true) && (net_timeout_left_ms(start_time, HAL_GetTick(), sock->write_timeout) <= 0) )
    {
      rc = NET_TIMEOUT;
      break;
    }
    status = nbiot_tcp_send(0, (unsigned char *)buf, len, sock->write_timeout);
    if (status !=  NB_STATUS_OK)
    {
      rc = NET_ERR;
      msg_error("Send failed.\n");
      break;
    }
    else
    {
      rc = NET_OK;
      sent = len;
    }
  
  } while ( (sent == 0) && (sock->blocking == true) && (rc == 0) );
  
  return (rc < 0) ? rc : sent;
}


int net_sock_close_tcp_nb(net_sockhnd_t sockhnd)
{
  int rc = NET_ERR;
  net_sock_ctxt_t *sock = (net_sock_ctxt_t * ) sockhnd;
  NB_Status_t status = nbiot_close_socket((uint8_t) ((uint32_t)sock->underlying_sock_ctxt && 0xFF));
  if (status == NB_STATUS_OK)
  {
    //underlying_socket_busy[(int) sock->underlying_sock_ctxt] = false;//@sz
    sock->underlying_sock_ctxt = (net_sockhnd_t) -1;
    rc = NET_OK;
  }
  return rc;
}


int net_sock_destroy_tcp_nb(net_sockhnd_t sockhnd)
{
  int rc = NET_ERR;
  net_sock_ctxt_t *sock = (net_sock_ctxt_t * ) sockhnd;
  net_ctxt_t *ctxt = sock->net;
    
  /* Find the parent in the linked list.
   * Unlink and free. 
   */
  if (sock == ctxt->sock_list)
  {
    ctxt->sock_list = sock->next;
    rc = NET_OK;
  }
  else
  {
    net_sock_ctxt_t *cur = ctxt->sock_list;
    do
    {
      if (cur->next == sock)
      {
        cur->next = sock->next;
        rc = NET_OK;
        break;
      }
      cur = cur->next;
    } while(cur->next != NULL);
  }
  if (rc == NET_OK)
  {
    net_free(sock);
    sock =NULL;//@sz
  }
  
  return rc;
}

#endif /* USE_NB */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
