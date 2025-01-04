/* include libs */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <lv2.h>

#define TWO_PI_FLOAT 6.283185307179586476925286766559005768394f

/* struct definition */
typedef struct 
{
    float* audio_in_ptr;
    float* audio_out_ptr;
    float* freq_ptr;
	float* modidx_ptr;

	double sample_rate;
	double norm_phase;
} RvdbTremolo;

/* internal core methods */
static LV2_Handle instantiate(const struct LV2_Descriptor *descriptor,
		double sample_rate, const char *bundle_path, const LV2_Feature *const *features)
{
    RvdbTremolo* m = (RvdbTremolo*) calloc(1, sizeof (RvdbTremolo));
    m->sample_rate = sample_rate;
    return m;
}

static void connect_port(LV2_Handle instance, uint32_t port, void *data_location)
{
    RvdbTremolo* m = (RvdbTremolo*)instance;
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
        m->modidx_ptr = (float*)data_location;
        break;

    default:
        break;
    }
}

static void activate(LV2_Handle instance)
{
    RvdbTremolo* m = (RvdbTremolo*)instance;
    if (!m) return;

	m->norm_phase = 0.0;
}

static void run (LV2_Handle instance, uint32_t sample_count)
{
    RvdbTremolo* m = (RvdbTremolo*)instance;
    if (!m) return;
    if ((!m->audio_in_ptr) || (!m->audio_out_ptr) || (!m->freq_ptr) || (!m->modidx_ptr)) return;

	float freq = *m->freq_ptr;
	float modidx2 = 0.5 * (double)*m->modidx_ptr;

	float gain_avg = 1.0 - modidx2; // DC value of amp modulation

    for (uint32_t i = 0; i < sample_count; ++i) {
        float gain = gain_avg + modidx2 * sinf(TWO_PI_FLOAT * m->norm_phase);
        m->audio_out_ptr[i] = m->audio_in_ptr[i] * gain;

    	m->norm_phase += *(m->freq_ptr) / m->sample_rate;
    	m->norm_phase = m->norm_phase > 1.0f ? m->norm_phase - 1.0f : m->norm_phase;
    }
}

static void deactivate (LV2_Handle instance)
{
    /* not needed here */
}

static void cleanup (LV2_Handle instance)
{
    RvdbTremolo* m = (RvdbTremolo*) instance;
    if (!m) return;
    free (m);
}

static const void* extension_data (const char *uri)
{
    return NULL;
}

/* descriptor */
static LV2_Descriptor const descriptor =
{
    "https://github.com/kellywashere/lv2/rvdbTremolo",
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
