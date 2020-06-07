#ifndef _KFIFO_H_
#define _KFIFO_H_

struct kfifo {
  unsigned int	in;		// ��һ��������offset
  unsigned int	out;	// ��һ��������offset
  unsigned int	mask;	// FIFO����1��FIFO����ҪΪ2�������η�
  void	        *data;	// ����ָ�룬�ɶ�̬���䣬���û��������ݵ�ַ��
};

int kfifo_malloc(struct kfifo *fifo, unsigned int size);
void kfifo_free(struct kfifo *fifo);
int kfifo_init(struct kfifo *fifo, void *buffer, unsigned int size);
unsigned int kfifo_in(struct kfifo *fifo, const void *buf, unsigned int len);
unsigned int kfifo_out(struct kfifo *fifo, void *buf, unsigned int len);
unsigned int kfifo_getlen(struct kfifo *fifo);
#endif
