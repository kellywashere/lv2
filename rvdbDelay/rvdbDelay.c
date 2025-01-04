/* include libs */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
// #include <math.h>
#include <string.h>
#include <lv2.h>

#define MAX_DELAY_S 3.0

typedef struct
{
	float* buf;
	int    capacity;
	int    writepos;
} RingBuffer;

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

/* struct definition */
typedef struct 
{
    float* audio_in_ptr;
    float* audio_out_ptr;
    float* delay_ptr;
    float* feedback_ptr;
    float* dry_ptr;
    float* wet_ptr;

	double sample_rate;

	RingBuffer* ringbuf;

} RvdbDelay;

/* internal core methods */
static LV2_Handle instantiate(const struct LV2_Descriptor *descriptor,
		double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    RvdbDelay* m = (RvdbDelay*) calloc(1, sizeof (RvdbDelay));
    m->sample_rate = sample_rate;
    m->ringbuf = ringbuffer_create(((int)(MAX_DELAY_S * sample_rate)) * 2); // TODO: check this eq.
    return m;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    RvdbDelay* m = (RvdbDelay*)instance;
    if (!m) return;

    switch (port)
    {
    case 0:
        m->audio_in_ptr = (float*)data_location;
        break;

    case 1:
        m->audio_out_ptr = (float*)data_location;
        break;

    case 2:
    	m->delay_ptr = (float*)data_location;
    	break;

    case 3:
    	m->feedback_ptr = (float*)data_location;
    	break;

    case 4:
    	m->dry_ptr = (float*)data_location;
    	break;

    case 5:
    	m->wet_ptr = (float*)data_location;
    	break;

    default:
        break;
    }
}

static void activate(LV2_Handle instance)
{
    RvdbDelay* m = (RvdbDelay*)instance;
    if (!m) return;
	; // no actions
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    RvdbDelay* m = (RvdbDelay*)instance;
    if (!m) return;
    if ((!m->audio_in_ptr) || (!m->audio_out_ptr)) return;
    if ((!m->delay_ptr) || (!m->feedback_ptr) ||
    		(!m->dry_ptr) || (!m->wet_ptr)) return;
	if (!m->ringbuf) return;

	int delay = *m->delay_ptr * m->sample_rate; // delay in samples

	if (delay == 0) {
		for (uint32_t i = 0; i < sample_count; ++i) {
			m->audio_out_ptr[i] = m->audio_in_ptr[i] * *m->dry_ptr;
		}
	}
	else {
		for (uint32_t i = 0; i < sample_count; ++i) {
			float delayed = ringbuffer_read(m->ringbuf, delay);
			ringbuffer_write(m->ringbuf, m->audio_in_ptr[i] + delayed * *m->feedback_ptr);
			m->audio_out_ptr[i] = m->audio_in_ptr[i] * *m->dry_ptr + delayed * *m->wet_ptr;
		}
	}
}

static void deactivate (LV2_Handle instance)
{
    /* not needed here */
}

static void cleanup (LV2_Handle instance)
{
    RvdbDelay* m = (RvdbDelay*) instance;
    if (!m) return;
    ringbuffer_delete(m->ringbuf);
    free (m);
}

static const void* extension_data (const char *uri)
{
    return NULL;
}

/* descriptor */
static LV2_Descriptor const descriptor =
{
    "https://github.com/kellywashere/lv2/rvdbDelay",
    instantiate,
    connect_port,
    activate /* or NULL */,
    run,
    deactivate /* or NULL */,
    cleanup,
    extension_data /* or NULL */
};

/* interface */
const LV2_SYMBOL_EXPORT LV2_Descriptor* lv2_descriptor (uint32_t index)
{
    if (index == 0) return &descriptor;
    else return NULL;
}
