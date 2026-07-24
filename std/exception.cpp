#include "exception.hpp"
#include <os/os.hpp>

void std::terminate() noexcept
{
	panic_with_message("std::terminate was called");
}