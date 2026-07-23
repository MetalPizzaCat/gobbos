#include "typeinfo.hpp"

std::type_info::~type_info()
{
}

bool std::type_info::operator==(const type_info &a) const
{
	// TODO: Implement strcmp
	return __type_name == a.__type_name;
}

bool std::type_info::operator!=(const type_info &a) const
{
	return __type_name != a.__type_name;
}

bool std::type_info::before(const std::type_info &a) const
{
	return __type_name < a.__type_name;
}

std::type_info &std::type_info::operator=(const type_info &rhs)
{
	// TODO: insert return statement here
}

__cxxabiv1::__class_type_info::~__class_type_info()
{
}

__cxxabiv1::__si_class_type_info::~__si_class_type_info()
{
}