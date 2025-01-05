#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "rand.h"

Rand* rand_create() {
	Rand* r = (Rand*) malloc(sizeof(Rand));
	r->seed = time(NULL);
	return r;
}

void rand_delete(Rand* r) {
	free(r);
}

uint64_t rand_get_next64(Rand* r) {
	// numbers from https://en.wikipedia.org/wiki/Linear_congruential_generator, Knuth MMIX
	r->seed = r->seed * 6364136223846793005ULL + 1442695040888963407ULL;
	return r->seed;
}

int rand_get_uniform(Rand* r, int low, int high) {
	// gets integer in range [low, high)
	uint64_t x = rand_get_next64(r);
	int m = high - low;
	int y = x % m;
	return y + low;
}

float rand_get_normal(Rand* r) {
	float sum = 0;
	for (int ii = 0; ii < 12; ++ii)
		sum += ((float)(rand_get_next64(r) & 0x07ffffff));
	sum = sum / ((float)(0x07ffffff)) - 6.0f;
	return sum;
}

void rand_set_seed(Rand* r, uint64_t seed) {
	r->seed = seed;
}

#if defined(TEST_NORMAL) || defined(TEST_UNIFORM)

#include <stdio.h>

int main() {
	Rand* r = rand_create();
	for (int ii = 0; ii < 10000; ++ii) {
#ifdef TEST_NORMAL
		printf("%f\n", rand_get_normal(r));
#else
		printf("%d\n", rand_get_uniform(r, -10, 10));
#endif
	}
	rand_delete(r);
	return 0;
}

#endif
