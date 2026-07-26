#include "string.hpp"
#include <memory.hpp>
#define STD_STRING_CAPACITY_STEP 16

std::string::string(const char *str)
{
	m_size = strlen(str);
	m_allocatedSize = ((m_size / STD_STRING_CAPACITY_STEP) + 1) * STD_STRING_CAPACITY_STEP;
	m_data = new char[m_allocatedSize];
	strcpy(m_data, str);
}

std::string::string(char character, size_t count)
{
	m_size = count + 1;
	m_allocatedSize = ((m_size / STD_STRING_CAPACITY_STEP) + 1) * STD_STRING_CAPACITY_STEP;
	m_data = new char[m_allocatedSize];
	m_data[count] = '\0';

	memset(m_data, character, count);
}

char &std::string::operator[](size_t i)
{
	return m_data[i];
}

const char &std::string::operator[](size_t i) const
{
	return m_data[i];
}

void std::string::push_back(char ch)
{
	if (m_size + 2 > m_allocatedSize)
	{
		m_allocatedSize += STD_STRING_CAPACITY_STEP;
		m_data = (char *)realloc(m_data, m_allocatedSize);
	}
	m_data[m_size++] = ch;
	m_data[m_size++] = '\0';
}

std::string::~string()
{
	delete[] m_data;
}
