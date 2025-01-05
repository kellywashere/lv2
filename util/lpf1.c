#include <stdlib.h>
#include <math.h>
#include "lpf1.h"

#define TWO_PI_FLOAT 6.283185307179586476925286766559005768394f

LPF1* lpf1_create(float fs) {
	LPF1* lpf = (LPF1*) calloc(1, sizeof(LPF1));
	// TODO: error handling
	lpf->fs = fs;
	lpf->y1 = 0.0f;
	lpf->p  = 0.0f;
	return lpf;
}

void  lpf1_delete(LPF1* lpf) {
	free(lpf);
}

void  lpf1_set_freq(LPF1* lpf, float freq) {
	// TODO: range checking
	lpf->p = expf(-TWO_PI_FLOAT * freq / lpf->fs);
}

float lpf1_process(LPF1* lpf, float x) {
	float y = lpf->p * lpf->y1 + x * (1 - lpf->p);
	lpf->y1 = y;
	return y;
}

