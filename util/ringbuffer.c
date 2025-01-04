#include <stdlib.h>
#include "ringbuffer.h"

RingBuffer* ringbuffer_create(int capacity) {
	RingBuffer* rb = (RingBuffer*) calloc(1, sizeof(RingBuffer));
	// TODO: error handling
	rb->buf = (float*) calloc(capacity, sizeof(float));
	rb->capacity = capacity;
	rb->writepos = 0;
	return rb;
}

void ringbuffer_delete(RingBuffer* rb) {
	if (rb) {
		free(rb->buf);
		free(rb);
	}
}

/*
void ringbuffer_write_from_buf(RingBuffer* rb, float* b, int count) {
	if (count >= rb->capacity)
		return;

	int count1 = rb->capacity - rb->writepos; // available before wrapping
	count1 = count1 > count ? count : count1; // max is count
	int count2 = count - count1;
	// write part 1
	memcpy(rb->buf + rb->writepos, b, count1 * sizeof(float));
	b += count1;
	rb->writepos += count1;
	rb->writepos = rb->writepos == rb->capacity ? 0 : rb->writepos;
	// write part 2
	if (count2 > 0) {
		memcpy(rb->buf, b, count2 * sizeof(float));
		rb->writepos = count2;
		rb->writepos = rb->writepos == rb->capacity ? 0 : rb->writepos;
	}
}
*/

void ringbuffer_write(RingBuffer* rb, float f) {
	rb->buf[rb->writepos++] = f;
	rb->writepos = rb->writepos == rb->capacity ? 0 : rb->writepos;
}

float ringbuffer_read(RingBuffer* rb, int delay) {
	int idx = rb->writepos - delay;
	idx = idx < 0 ? idx + rb->capacity : idx;
	return rb->buf[idx];
}


