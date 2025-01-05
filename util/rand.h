#include <stdint.h>

typedef struct {
	uint64_t seed;
} Rand;

Rand* rand_create();
void rand_delete(Rand* r);

uint64_t rand_get_next64(Rand* r);
int rand_get_uniform(Rand* r, int low, int high);
float rand_get_normal(Rand* r);

void rand_set_seed(Rand* r, uint64_t seed);
