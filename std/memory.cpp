#include "memory.hpp"

void mempattern4(uint32_t patt, void *dst, size_t size)
{
	uint32_t *dst_mem = (uint32_t *)dst;
	for (size_t i = 0; i < size; i++, dst_mem += 1)
	{
		*dst_mem = patt;
	}
}
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