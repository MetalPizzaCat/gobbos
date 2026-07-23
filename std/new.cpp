#include "new.hpp"

void operator delete(void *ptr) noexcept
{
	//TODO: Add freeing
	//std::free(ptr);
}