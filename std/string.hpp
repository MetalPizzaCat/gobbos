#pragma once
#include <stdint.h>
#include <stddef.h>
namespace std
{
	class string
	{
	public:
		/// @brief Create a string that holds a copy of a given string
		/// @param str
		string(const char *str);

		/// @brief Create a string that consists of `character` being copied `count` times
		/// @param character Character to copy
		/// @param count Amount of times to copy the character
		/// @return
		string(char character, size_t count);
		const char *c_str() const { return m_data; }

		/// @brief Get character at index i without any checks
		/// @param i
		/// @return
		char &operator[](size_t i);

		/// @brief Get character at index i without any checks
		/// @param i
		/// @return
		const char &operator[](size_t i) const;

		/// @brief Get size of the string
		/// @return
		size_t len() const { return m_size; }

		/// @brief Get size of the string. Wrapper around len for consistency sake
		/// @return
		size_t size() const;

		/// @brief Add new character to the end of string
		/// @param ch Character to add
		void push_back(char ch);

		size_t capacity() const { return m_allocatedSize; }

		~string();

	private:
		/// @brief Pointer to the start of data
		char *m_data;
		size_t m_size;
		size_t m_allocatedSize;
	};
}