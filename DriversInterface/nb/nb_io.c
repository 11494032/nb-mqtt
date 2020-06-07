#include "nb_io.h"
//socket µœ÷¥˙¬Î

int8_t NB_IO_Init(void)
{

}

/**
* @brief  NB IO Deinitialization.
*         This function Deinits the UART interface of the NB. When called
*         the NB commands can't be executed anymore.
* @param  None.
* @retval None.
*/
void NB_IO_DeInit(void)
{
 
}

/**
* @brief  Send Data to the NB module over the UART interface.
*         This function allows sending data to the  NB NB Module, the
*          data can be either an AT command or raw data to send over 
a pre-established NB connection.
* @param pData: data to send.
* @param Length: the data length.
* @retval 0 on success, -1 otherwise.
*/
int8_t NB_IO_Send(char* pData, uint32_t Length)
{
  
  return 0;
}

/**
* @brief  Rx Callback when new data is received on the UART.
* @param  UartHandle: Uart handle receiving the data.
* @retval None.
*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
 
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
 
}
/**
* @brief  Function called when error happens on the UART.
* @param  UartHandle: Uart handle receiving the data.
* @retval None.
*/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  /* Add user code here*/
}

int32_t NB_IO_Receive(uint8_t* Buffer, uint32_t Length)
{
//  return kfifo_out(&NB.io_fifo, Buffer, Length);
}