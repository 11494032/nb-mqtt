//一个环形缓冲区的实现
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "kfifo.h"
#define MIN(a,b)        (((a) < (b)) ? (a) : (b))

/*
* internal helper to calculate the unused elements in a fifo
*/
static inline unsigned int kfifo_unused(struct kfifo *fifo)
{
  return (fifo->mask + 1) - (fifo->in - fifo->out);
}

unsigned int kfifo_getlen(struct kfifo *fifo)
{
  return fifo->in - fifo->out;
}

//注意：size必须是2的整数次幂，否则必然出错
int kfifo_malloc(struct kfifo *fifo, unsigned int size)
{
  printf("%s:%d\r\n", __func__, __LINE__);
  fifo->in = 0;
  fifo->out = 0;
  
  if (size < 2) {
    fifo->data = NULL;
    fifo->mask = 0;
    return -1;
  }
  
  fifo->data = malloc(size);
  
  if (!fifo->data) {
    fifo->mask = 0;
    return -2;
  }
  fifo->mask = size - 1;
  
  return 0;
}

void kfifo_free(struct kfifo *fifo)
{
  free(fifo->data);
  fifo->in = 0;
  fifo->out = 0;
  fifo->data = NULL;
  fifo->mask = 0;
}

//注意：size必须是2的整数次幂，否则必然出错
int kfifo_init(struct kfifo *fifo, void *buffer, unsigned int size)
{
  fifo->in = 0;
  fifo->out = 0;
  fifo->data = buffer;
  
  if (size < 2) {
    fifo->mask = 0;
    return -1;
  }
  fifo->mask = size - 1;
  
  return 0;
}

static void kfifo_copy_in(struct kfifo *fifo, const void *src,
                          unsigned int len, unsigned int off)
{
  unsigned int size = fifo->mask + 1;
  unsigned int l;
  
  off &= fifo->mask;
  
  l = MIN(len, size - off);
  
  memcpy((unsigned char *)fifo->data + off, src, l);
  memcpy((unsigned char *)fifo->data, (unsigned char *)src + l, len - l);
}

unsigned int kfifo_in(struct kfifo *fifo, const void *buf, unsigned int len)
{
  unsigned int l;
  
  l = kfifo_unused(fifo);
  if (len > l)
    len = l;
  
  kfifo_copy_in(fifo, buf, len, fifo->in);
  fifo->in += len;
  return len;
}


static void kfifo_copy_out(struct kfifo *fifo, void *dst, unsigned int len, unsigned int off)
{
  unsigned int size = fifo->mask + 1;
  unsigned int l;
  
  off &= fifo->mask;
  
  l = MIN(len, size - off);
  
  memcpy(dst, (unsigned char *)fifo->data + off, l);
  memcpy((unsigned char *)dst + l, fifo->data, len - l);
}

unsigned int __kfifo_out_peek(struct kfifo *fifo,
                              void *buf, unsigned int len)
{
  unsigned int l;
  
  l = fifo->in - fifo->out;
  if (len > l)
    len = l;
  
  kfifo_copy_out(fifo, buf, len, fifo->out);
  return len;
}

unsigned int kfifo_out(struct kfifo *fifo, void *buf, unsigned int len)
{
  len = __kfifo_out_peek(fifo, buf, len);
  fifo->out += len;
  return len;
}
