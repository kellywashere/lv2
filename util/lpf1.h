#ifndef __LPF1__
#define __LPF1__

// H(z) = (1-p) * z^-1 / (1 - p * z^-1), DC gain is 1
typedef struct
{
	float fs;   // sample rate in Hz
	float y1;   // y(n-1)
	float p;    // pole in z domain
} LPF1;

LPF1* lpf1_create(float fs);
void  lpf1_delete(LPF1* lpf);

void  lpf1_set_freq(LPF1* lpf, float freq);

float lpf1_process(LPF1* lpf, float x);

#endif

