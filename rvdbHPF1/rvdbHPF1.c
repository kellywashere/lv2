#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <lv2.h>
#include "../util/hpf1.h"

#define TWO_PI_FLOAT 6.283185307179586476925286766559005768394f

typedef struct 
{
    float* audio_in_ptr;
    float* audio_out_ptr;
    float* freq_ptr;
	float* gain_ptr;

	double sample_rate;
	HPF1*  hpf;
} RvdbHPF1;

/* internal core methods */
static LV2_Handle instantiate(const struct LV2_Descriptor *descriptor,
		double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    RvdbHPF1* m = (RvdbHPF1*) calloc(1, sizeof (RvdbHPF1));
    m->sample_rate = sample_rate;
	m->hpf = hpf1_create(m->sample_rate);
    return m;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    RvdbHPF1* m = (RvdbHPF1*)instance;
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
        m->freq_ptr = (float*)data_location;
        break;

    case 3:
        m->gain_ptr = (float*)data_location;
        break;

    default:
        break;
    }
}

static void activate(LV2_Handle instance)
{
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    RvdbHPF1* m = (RvdbHPF1*)instance;
    if (!m) return;
    if ((!m->audio_in_ptr) || (!m->audio_out_ptr) || (!m->freq_ptr) || (!m->gain_ptr)) return;

	hpf1_set_freq(m->hpf, *m->freq_ptr);

    for (uint32_t i = 0; i < sample_count; ++i) {
        float y = hpf1_process(m->hpf, m->audio_in_ptr[i]);
        m->audio_out_ptr[i] = y * *m->gain_ptr;
    }
}

static void deactivate (LV2_Handle instance)
{
}

static void cleanup (LV2_Handle instance)
{
    RvdbHPF1* m = (RvdbHPF1*) instance;
    if (!m) return;
	hpf1_delete(m->hpf);
    free (m);
}

static const void* extension_data (const char *uri)
{
    return NULL;
}

/* descriptor */
static LV2_Descriptor const descriptor =
{
    "https://github.com/kellywashere/lv2/rvdbHPF1",
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
