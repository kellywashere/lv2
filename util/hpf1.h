#ifndef __HPF1__
#define __HPF1__

// H(z) = p * (1 - z^-1) / (1 - p * z^-1)
typedef struct
{
	float fs;   // sample rate in Hz
	float x1;   // x(n-1)
	float y1;   // y(n-1)
	float p;    // pole in z domain
} HPF1;

HPF1* hpf1_create(float fs);
void  hpf1_delete(HPF1* hpf);

void  hpf1_set_freq(HPF1* hpf, float freq);

float hpf1_process(HPF1* hpf, float x);

#endif

