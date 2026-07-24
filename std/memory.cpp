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
		if (!current->free)
		{
			current = current->next;
			continue;
		}
		if (current->size < size)
		{
			current = current->next;
			continue;
		}

		uint64_t block_start = (uint64_t)(current + 1);
		uint64_t aligned_start = (block_start + alignment - 1) & ~(alignment - 1);
		uint64_t padding = aligned_start - block_start;

		if (current->size < size + padding)
		{
			current = current->next;
			continue;
		}

		if (padding < sizeof(BlockHeader))
		{
			current = current->next;
			continue;
		}

		if (padding > sizeof(BlockHeader))
		{
			split(current, padding - sizeof(BlockHeader));
			current = current->next;
		}

		if (current->size > (size + sizeof(BlockHeader)) * 2)
		{
			split(current, size);
			current->free = 0;
			return (void *)(current + 1);
		}

		current->free = 0;
		return (void *)(current + 1);
	}

	return NULL;
}