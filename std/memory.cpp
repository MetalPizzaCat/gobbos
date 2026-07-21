#include "memory.hpp"


void *memset(void *dst, uint8_t v, size_t c)

{
	uint8_t *d = (uint8_t *)dst;
	while (c > 0)
	{
		*d = v;
		d++;
		c--;
	}

	return dst;
}