#include "new.hpp"
#include <os/os.hpp>
void operator delete(void *ptr) noexcept
{
	// TODO: Add freeing
	free(ptr);
	// std::free(ptr);
}

void *operator new(size_t size)
{
	if (size == 0)
		size = 1;
	// if (p == nullptr)
	// 	__throw_bad_alloc_shim();
	void *r = malloc(size);
	if (r == nullptr)
	{
		panic_with_message("Bad memory allocation");
	}
	return r;
}

static void *operator_new_impl(size_t size)
{
	if (size == 0)
		size = 1;
	// void *p;
	// while ((p = malloc(size)) == nullptr)
	// {
	// 	// If malloc fails and there is a new_handler,
	// 	// call it to try free up memory.
	// 	std::new_handler nh = std::get_new_handler();
	// 	if (nh)
	// 		nh();
	// 	else
	// 		break;
	// }
	return malloc(size);
}
