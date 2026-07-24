#include "heap.hpp"
#include <os/os.hpp>
#define LIMINE_MEMMAP_REQUEST_ID {LIMINE_COMMON_MAGIC, 0x67cf3d9d378a806f, 0xe304acdfc50c3c62}

/*

LIMINE_MEMMAP_USABLE entries represent regions of the address space that are usable RAM, and do not contain other data, the executable, bootloader information, or anything valuable, and are therefore free for use.

LIMINE_MEMMAP_RESERVED entries represent regions of the address space that are reserved for unspecified purposes by the firmware, hardware, or otherwise, and should not be touched by the executable.

LIMINE_MEMMAP_ACPI_RECLAIMABLE entries represent regions of the address space containing ACPI related data, such as ACPI tables and AML code. The executable should make absolutely sure that no data contained in these regions is still needed before deciding to reclaim these memory regions for itself. Refer to the ACPI specification for further information.

LIMINE_MEMMAP_ACPI_NVS entries represent regions of the address space used for ACPI non-volatile data storage. Refer to the ACPI specification for further information.

LIMINE_MEMMAP_BAD_MEMORY entries represent regions of the address space that contain bad RAM, which may be unreliable, and therefore these regions should be treated the same as reserved regions.

LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE entries represent regions of the address space containing RAM used to store bootloader or firmware information that should be available to the executable (or, in some cases, hardware, such as for MP trampolines). The executable should make absolutely sure that no data contained in these regions is still needed before deciding to reclaim these memory regions for itself.

LIMINE_MEMMAP_EXECUTABLE_AND_MODULES entries are meant to have an illustrative purpose only, and are not authoritative sources to be used as a means to find the addresses of the executable or modules. One must use the specific Limine features (Executable Address and Module features) to do that.

LIMINE_MEMMAP_FRAMEBUFFER entries represent regions of the address space containing memory-mapped framebuffers. These entries exist for illustrative purposes only, and are not to be used to acquire the address of any framebuffer. One must use the Framebuffer feature for that.

LIMINE_MEMMAP_RESERVED_MAPPED (base revision 4 or greater) entries represent regions of the address space containing the ACPI tables as described by the Memory Layout at Entry section, if the firmware did not already map them within either an ACPI reclaimable or an ACPI NVS region. For base revision 5 or greater, these entries additionally contain SMBIOS tables, EFI Runtime Services code and data, and the EFI system table along with the data it references (see Base Revision 5 for details).


*/
#define LIMINE_MEMMAP_USABLE 0
#define LIMINE_MEMMAP_RESERVED 1
#define LIMINE_MEMMAP_ACPI_RECLAIMABLE 2
#define LIMINE_MEMMAP_ACPI_NVS 3
#define LIMINE_MEMMAP_BAD_MEMORY 4
#define LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define LIMINE_MEMMAP_EXECUTABLE_AND_MODULES 6
#define LIMINE_MEMMAP_FRAMEBUFFER 7
#define LIMINE_MEMMAP_RESERVED_MAPPED 8

__attribute__((section(".limine_requests"))) struct limine_memmap_request memory_map_request = {
	.id = LIMINE_MEMMAP_REQUEST_ID,
};

os::memory::HeapInfo::HeapInfo() : m_response(memory_map_request.response)
{
}

void os::memory::HeapInfo::init(uint8_t *heapBuffer, uint64_t heapSize)
{
	m_heap = heapBuffer;
	m_heapSize = heapSize;
	m_root = (BlockHeader *)(heapBuffer);
	m_root->size = heapSize - sizeof(BlockHeader);
	m_root->free = true;
	m_root->next = nullptr;
	m_root->prev = nullptr;
}

void os::memory::HeapInfo::initLargestHeap()
{
	uint64_t maxMem = 0;
	uint64_t bestBase = -1;
	for (uint64_t i = 0; i < m_response->entry_count; i++)
	{
		if (m_response->entries[i]->type == LIMINE_MEMMAP_USABLE && m_response->entries[i]->length > maxMem)
		{
			maxMem = m_response->entries[i]->length;
			bestBase = m_response->entries[i]->base;
		}
	}
	if (maxMem > 0)
	{
		init((uint8_t *)bestBase, maxMem);
	}
	else
	{
		panic_with_message("No suitable memory location found to create heap");
		// do some error handling if no memory found
	}
}

os::memory::BlockHeader *os::memory::merge(BlockHeader *entry, BlockMergeDirection direction)
{
    BlockHeader *entry2 = NULL;
    switch (direction)
    {
    case BlockMergeDirection::Previous:
        entry2 = entry->prev;
        entry2->size += entry->size + sizeof(BlockHeader);
        entry2->next = entry->next;
        if (entry2->prev)
            entry2->next->prev = entry2;
        return entry2;
        break;
    case BlockMergeDirection::Next:
        entry2 = entry->next;
        entry->size += entry2->size + sizeof(BlockHeader);
        entry->next = entry2->next;
        if (entry->next)
            entry->next->prev = entry;
        return entry;
        break;
    default:
        return NULL;
    }
}


void os::memory::split(BlockHeader *entry, uint64_t size)
{
	uint64_t total_size = entry->size;
	uint64_t size2 = total_size - size - sizeof(BlockHeader);
	BlockHeader *old_next = entry->next;

	entry->size = size;
	entry->next = (BlockHeader *)((uint8_t *)(entry + 1) + size);

	BlockHeader *next = entry->next;
	next->size = size2;
	next->free = 1;
	next->prev = entry;
	next->next = old_next;
}

// void *os::memory::BlockHeader::merge(BlockMergeDirection direction)
// {
// 	BlockHeader *entry2 = nullptr;
// 	switch (direction)
// 	{
// 	case BlockMergeDirection::Previous:
// 		entry2 = prev;
// 		entry2->size += this->size + sizeof(BlockHeader);
// 		entry2->next = this->next;
// 		entry2->next->prev = entry2;
// 		return entry2;
// 	case BlockMergeDirection::Next:
// 		entry2 = next;
// 		this->size += entry2->size + sizeof(BlockHeader);
// 		this->next = entry2->next;
// 		this->next->prev = this;
// 		return this;
// 	default:
// 		return nullptr;
// 	}
// }
