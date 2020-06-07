#ifndef _KFIFO_H_
#define _KFIFO_H_

struct kfifo {
  unsigned int	in;		// 下一个入数的offset
  unsigned int	out;	// 下一个出数的offset
  unsigned int	mask;	// FIFO数减1，FIFO数需要为2的整数次方
  void	        *data;	// 数据指针，可动态分配，或用户传递数据地址。
};

int kfifo_malloc(struct kfifo *fifo, unsigned int size);
void kfifo_free(struct kfifo *fifo);
int kfifo_init(struct kfifo *fifo, void *buffer, unsigned int size);
unsigned int kfifo_in(struct kfifo *fifo, const void *buf, unsigned int len);
unsigned int kfifo_out(struct kfifo *fifo, void *buf, unsigned int len);
unsigned int kfifo_getlen(struct kfifo *fifo);
#endif
