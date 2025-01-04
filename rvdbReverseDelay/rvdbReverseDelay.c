/* include libs */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
// #include <math.h>
#include <string.h>
#include <lv2.h>

#include "../util/ringbuffer.h"

#define MAX_DELAY_S 3.0

/* struct definition */
typedef struct 
{
    float* audio_in_ptr;
    float* audio_out_ptr;
    float* delay_ptr;
    float* dry_ptr;
    float* wet_ptr;

	double sample_rate;

	RingBuffer* ringbuf;
	int    pos; // position (sample#) within delay window

} RvdbDelay;

/* internal core methods */
static LV2_Handle instantiate(const struct LV2_Descriptor *descriptor,
		double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    RvdbDelay* m = (RvdbDelay*) calloc(1, sizeof (RvdbDelay));
    m->sample_rate = sample_rate;
    m->ringbuf = ringbuffer_create(((int)(MAX_DELAY_S * sample_rate)) * 2); // TODO: check this eq.
    m->pos = 0;
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
    	m->dry_ptr = (float*)data_location;
    	break;

    case 4:
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
    if ((!m->delay_ptr) || (!m->dry_ptr) || (!m->wet_ptr)) return;
	if (!m->ringbuf) return;

	int windowsize = *m->delay_ptr * m->sample_rate; // window size in samples

	if (windowsize == 0) {
		for (uint32_t i = 0; i < sample_count; ++i) {
			m->audio_out_ptr[i] = m->audio_in_ptr[i] * *m->dry_ptr;
		}
	}
	else {
		for (uint32_t i = 0; i < sample_count; ++i) {
			float smpl = ringbuffer_read(m->ringbuf, 2*m->pos + 1);
			++m->pos;
			m->pos = m->pos >= windowsize ? m->pos % windowsize : m->pos;

			ringbuffer_write(m->ringbuf, m->audio_in_ptr[i]);

			m->audio_out_ptr[i] = m->audio_in_ptr[i] * *m->dry_ptr + smpl * *m->wet_ptr;
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
    "https://github.com/kellywashere/lv2/rvdbReverseDelay",
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
