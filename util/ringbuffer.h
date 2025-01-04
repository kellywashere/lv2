#ifndef __RINGBUFFER__
#define __RINGBUFFER__

typedef struct
{
	float* buf;
	int    capacity;
	int    writepos;
} RingBuffer;

RingBuffer* ringbuffer_create(int capacity);
void ringbuffer_delete(RingBuffer* rb);
void ringbuffer_write(RingBuffer* rb, float f);
float ringbuffer_read(RingBuffer* rb, int delay);

#endif

