#pragma once
namespace std
{
	class type_info
	{
	public:
		virtual ~type_info();
		bool operator==(const type_info &) const;
		bool operator!=(const type_info &) const;
		bool before(const type_info &) const;
		const char *name() const { return __type_name; }

	private:
		type_info(const type_info &rhs);
		type_info &operator=(const type_info &rhs);
		const char *__type_name;
	};
}

namespace __cxxabiv1
{
	// taken from itanuim spec
	// https://itanium-cxx-abi.github.io/cxx-abi/abi.html#typeid

	class __class_type_info : public std::type_info
	{
	public:
		~__class_type_info();
	};

	class __si_class_type_info : public __class_type_info
	{
	public:
		~__si_class_type_info();
		const __class_type_info *__base_type;
	};

	struct __base_class_type_info
	{
	public:
		const __class_type_info *__base_type;
		long __offset_flags;

		enum __offset_flags_masks
		{
			__virtual_mask = 0x1,
			__public_mask = 0x2,
			__offset_shift = 8
		};
	};

	class __vmi_class_type_info : public __class_type_info
	{
	public:
		unsigned int __flags;
		unsigned int __base_count;
		__base_class_type_info __base_info[1];

		enum __flags_masks
		{
			__non_diamond_repeat_mask = 0x1,
			__diamond_shaped_mask = 0x2
		};
	};
}
