/* include libs */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <lv2.h>

#include "../util/rand.h"

/* struct definition */
typedef struct 
{
    float* audio_out_ptr;
    float* gain_ptr;

	double sample_rate; // not rly needed here
	Rand*  rnd;
} RvdbNoiseGen;

/* internal core methods */
static LV2_Handle instantiate(const struct LV2_Descriptor *descriptor,
		double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    RvdbNoiseGen* m = (RvdbNoiseGen*) calloc(1, sizeof (RvdbNoiseGen));
    m->sample_rate = sample_rate;
    m->rnd = rand_create();
    return m;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    RvdbNoiseGen* m = (RvdbNoiseGen*)instance;
    if (!m) return;

    switch (port)
    {
    case 0:
        m->audio_out_ptr = (float*)data_location;
        break;

    case 1:
        m->gain_ptr = (float*)data_location;
        break;

    default:
        break;
    }
}

static void activate(LV2_Handle instance)
{
    RvdbNoiseGen* m = (RvdbNoiseGen*)instance;
    if (!m) return;
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    RvdbNoiseGen* m = (RvdbNoiseGen*)instance;
    if (!m) return;
    if ((!m->audio_out_ptr) || (!m->gain_ptr)) return;

    for (uint32_t i = 0; i < sample_count; ++i) {
		float rnd = rand_get_normal(m->rnd);
        m->audio_out_ptr[i] = rnd * *m->gain_ptr;
    }
}

static void deactivate (LV2_Handle instance)
{
    /* not needed here */
}

static void cleanup (LV2_Handle instance)
{
    RvdbNoiseGen* m = (RvdbNoiseGen*) instance;
    if (!m) return;
	rand_delete(m->rnd);
    free(m);
}

static const void* extension_data (const char *uri)
{
    return NULL;
}

/* descriptor */
static LV2_Descriptor const descriptor =
{
    "https://github.com/kellywashere/lv2/rvdbNoiseGen",
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
