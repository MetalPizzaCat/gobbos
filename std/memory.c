#include "memory.h"

void mempattern4(uint32_t patt, void *dst, size_t size)
{
	for (size_t i = 0; i < size; i++, dst += sizeof(uint32_t))
	{
		*(uint32_t *)dst = patt;
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