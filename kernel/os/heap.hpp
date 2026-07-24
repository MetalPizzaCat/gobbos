#pragma once
#include <stdint.h>
#include <os/limine.h>

namespace os::memory
{

	enum class BlockMergeDirection
	{
		None,
		Previous,
		Next,
	};
	struct BlockHeader
	{
		uint64_t size;
		bool free;
		BlockHeader *prev;
		BlockHeader *next;

		//void split(uint64_t size);

		// void *merge(BlockMergeDirection direction);
	};

	BlockHeader *merge(BlockHeader *entry, BlockMergeDirection direction);

	void split(BlockHeader *entry, uint64_t size);
	/**
	 * @brief Class containing info for the current state of the heap
	 *
	 */
	class HeapInfo
	{
	public:
		HeapInfo();
		static HeapInfo &getInstance()
		{
			static HeapInfo info;
			return info;
		}

		void init(uint8_t *heapBuffer, uint64_t heapSize);

		/// @brief Init heap of largest possible size in available memory blocks
		void initLargestHeap();

		BlockHeader *getRoot() const { return m_root; }

	private:
		uint8_t *m_heap;
		uint64_t m_heapSize;
		BlockHeader *m_root;

		limine_memmap_response *m_response;
	};

	void initHeap(uint8_t *heap_buffer, uint64_t heap_size);
} // namespace os
