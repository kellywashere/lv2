#include <stdlib.h>
#include <math.h>
#include "hpf1.h"

#define TWO_PI_FLOAT 6.283185307179586476925286766559005768394f

HPF1* hpf1_create(float fs) {
	HPF1* hpf = (HPF1*) calloc(1, sizeof(HPF1));
	// TODO: error handling
	hpf->fs = fs;
	hpf->x1 = 0.0f;
	hpf->y1 = 0.0f;
	hpf->p  = 0.0f;
	return hpf;
}

void  hpf1_delete(HPF1* hpf) {
	free(hpf);
}

void  hpf1_set_freq(HPF1* hpf, float freq) {
	// TODO: range checking
	hpf->p = expf(-TWO_PI_FLOAT * freq / hpf->fs);
}

float hpf1_process(HPF1* hpf, float x) {
	float y = hpf->p * (x - hpf->x1 + hpf->y1);
	hpf->x1 = x;
	hpf->y1 = y;
	return y;
}

