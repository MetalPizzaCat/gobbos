#include <os/os.hpp>

namespace __cxxabiv1
{
	extern "C"
	{
		[[noreturn]] void __cxa_pure_virtual(void)
		{
			panic_with_message("Pure virtual function called!");
		}

		[[noreturn]] void __cxa_deleted_virtual(void)
		{
			panic_with_message("Deleted virtual function called!");
		}
	} // extern "C"
} // abi
