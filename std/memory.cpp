#include "memory.hpp"
#include <os/heap.hpp>

void *malloc(size_t size)
{
	using namespace os::memory;
	if (size == 0)
	{
		return nullptr;
	}
	BlockHeader *current = HeapInfo::getInstance().getRoot();

	while (current != nullptr)
	{
		if (current->free == 0)
		{
			current = current->next;
			continue;
		}
		if (current->size < size)
		{
			current = current->next;
			continue;
		}

		if (current->size > (size + sizeof(BlockHeader)) * 2)
		{
			split(current, size);
			current->free = false;
			return (void *)(current + 1);
		}
		current->free = true;
		return (void *)(current + 1);
	}

	return NULL;
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

void free(void *ptr)
{
	using namespace os::memory;
	if (ptr == nullptr)
	{
		return;
	}

	BlockHeader *entry = (BlockHeader *)ptr - 1;
	entry->free = 1;
	while (entry->prev != NULL && entry->prev->free == 1)
	{
		entry = (BlockHeader *)merge(entry, BlockMergeDirection::Previous);
	}
	while (entry->next != NULL && entry->next->free == 1)
	{
		entry = (BlockHeader *)merge(entry, BlockMergeDirection::Next);
	}
}

void *mallocAligned(uint64_t size, uint64_t alignment)
{
	using namespace os::memory;
	BlockHeader *current = HeapInfo::getInstance().getRoot();

	if (size == 0)
	{
		return NULL;
	}

	while (current != NULL)
	{
		if (current->free == 0)
		{
			current = current->next;
			continue;
		}

		uint64_t block_start = (uint64_t)(current + 1);
		uint64_t aligned_start = (block_start + alignment - 1) & ~(alignment - 1);
		uint64_t padding = aligned_start - block_start;
		uint64_t total_size = size + padding;

		if (current->size < total_size)
		{
			current = current->next;
			continue;
		}

		if (padding > 0)
		{
			if (padding < sizeof(BlockHeader))
			{
				current = current->next;
				continue;
			}

			split(current, padding - sizeof(BlockHeader));
			continue;
		}

		if (current->size > (size + sizeof(BlockHeader)) * 2)
		{
			split(current, size);
		}

		current->free = 0;
		return (void *)(current + 1);
	}

	return NULL;
}

void *realloc(void *ptr, uint32_t size)
{
	using namespace os::memory;
	BlockHeader *entry = (BlockHeader *)ptr - 1;

	if ((int32_t)size <= (int32_t)entry->size - (int32_t)sizeof(BlockHeader) * 2)
	{
		split(entry, size);
		BlockHeader *next = entry->next;
		if (next != NULL && next->next != NULL)
		{
			if (next->free == 1 && next->next->free == 1)
			{
				merge(next, BlockMergeDirection::Next);
			}
		}
		return (void *)(entry + 1);
	}
	else if (size <= entry->size)
	{
		return (void *)(entry + 1);
	}

	if (entry->next != NULL && entry->next->free == 1)
	{
		if (size < entry->size + entry->next->size + sizeof(BlockHeader))
		{
			entry = merge(entry, BlockMergeDirection::Next);
			split(entry, size);
			return (void *)(entry + 1);
		}
		if (entry->prev != NULL && entry->prev->free == 1)
		{
			if (size < entry->size + entry->next->size + entry->prev->size + sizeof(BlockHeader) * 2)
			{
				uint32_t old_size = entry->size;
				entry = merge(entry, BlockMergeDirection::Next);
				entry = merge(entry, BlockMergeDirection::Previous);
				split(entry, size);
				entry->free = 0;
				memcpy((void *)(entry + 1), ptr, old_size);
				return (void *)(entry + 1);
			}
		}
	}
	if (entry->prev != NULL && entry->prev->free == 1)
	{
		if (size < entry->size + entry->prev->size + sizeof(BlockHeader))
		{
			uint32_t old_size = entry->size;
			entry = merge(entry, BlockMergeDirection::Previous);
			split(entry, size);
			entry->free = 0;
			memcpy((void *)(entry + 1), ptr, old_size);
			return (void *)(entry + 1);
		}
	}

	uint8_t *new_ptr = (uint8_t *)malloc(size);
	if (new_ptr == NULL)
	{
		return NULL;
	}

	memcpy(new_ptr, ptr, entry->size);

	free((void *)(entry + 1));

	return new_ptr;
}

void *memcpy(void *dest, const void *source, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		((uint8_t *)dest)[i] = ((uint8_t *)source)[i];
	}
	return dest;
}

int strncmp(const char *p1, const char *p2, int len)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;
	int counter = 0;
	do
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
		counter++;
	} while (c1 == c2 && counter < len);

	return c1 - c2;
}

int strcmp(const char *p1, const char *p2)
{
	const unsigned char *s1 = (const unsigned char *)p1;
	const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;

	do
	{
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1 - c2;
	} while (c1 == c2);

	return c1 - c2;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	for (; *str != '\0'; str++, len++)
		;
	return len;
}

size_t strnlen(const char *str, size_t n)
{
	size_t len = 0;
	for (; *str != '\0' && len < n; str++, len++)
		;
	return len;
}

char *strcpy(char *dest, const char *src)
{
	char *d = dest;
	for (; (*dest = *src) != '\0'; src++, dest++)
		;
	return d;
}
