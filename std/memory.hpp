#pragma once
#include <stdint.h>
#include <stddef.h>

/// @brief Copy value `patt` `n` times in memory
/// @tparam T
/// @param patt Value to repeat in memory. While any type can be used the target type is intergers
/// @param dst Destination array in which to put the memory. Array is assumed to be at least `sizeof(T) * n` bytes long.
/// @param n
template <typename T>
void mempattern(T patt, void *dst, size_t n)
{
	T *dst_mem = (T *)dst;
	for (size_t i = 0; i < n; i++, dst_mem += 1)
	{
		*dst_mem = patt;
	}
}

extern "C" void *malloc(size_t size);

extern "C" void *memset(void *dst, uint8_t v, size_t c);

extern "C" void free(void *ptr);

void *mallocAligned(uint64_t size, uint64_t alignment);
