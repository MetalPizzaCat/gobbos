#pragma once
#include <stdint.h>
#include <stddef.h>

/// @brief Copy value `patt` `n` times in memory
/// @tparam T
/// @param patt Value to repeat in memory. While any type can be used the target type is intergers
/// @param dst Destination array in which to put the memory. Array is assumed to be at least `sizeof(T) * n` bytes long.
/// @param n
template <typename T>
void mempattern(T patt, void *dst, size_t n)
{
	T *dst_mem = (T *)dst;
	for (size_t i = 0; i < n; i++, dst_mem += 1)
	{
		*dst_mem = patt;
	}
}

/**
 * @brief Allocate block of memory in heap of given sie
 * @param size Size of the block
 * @return Pointer to newly allocated memory or nullptr on failure
 */
extern "C" void *malloc(size_t size);

/**
 * @brief Set all `c` bytes of destination memory block to given value
 * @param dst Buffer to set values in
 * @param v Value to set to
 * @param c How many bytes to set
 * @return Pointer to the updated block. Equal to `dst`
 *
 */
extern "C" void *memset(void *dst, uint8_t v, size_t c);

/**
 * @brief Free given block of memory
 * @param ptr Pointer to the block that needs to be freed
 *
 */
extern "C" void free(void *ptr);

/**
 * @brief Compare two strings stopping at `len`. Values beyond `len` are not accessed
 * @param p1 First string to compare
 * @param p2 Second string to compare
 * @param len Length of the comparison sequence
 * @return -1 if the first character that does not match has a lower value in `p1` than in `p2`, 0 if strings are equal and 1 the first character that does not match has a greater value in `p1` than in `p2`
 *
 */
extern "C" int strncmp(const char *p1, const char *p2, int len);

/**
 * @brief Compare two strings
 * @param p1 First string to compare
 * @param p2 Second string to compare
 * @return -1 if the first character that does not match has a lower value in `p1` than in `p2`, 0 if strings are equal and 1 the first character that does not match has a greater value in `p1` than in `p2`
 */
extern "C" int strcmp(const char *p1, const char *p2);

/**
 * @brief Get length of the string(the amount of bytes until first \0 is encountered)
 * @param str String to get length of
 * @return Length of stirng
 */
extern "C" size_t strlen(const char *str);

/**
 * @brief Get length of the string(the amount of bytes until first \0 is encountered or calculated length is equal to n)
 * @param str String to get length of
 * @param n Max length of string
 * @return Length of stirng or n if string is longer than n
 */
extern "C" size_t strnlen(const char *str, size_t n);

/**
 * @brief Copy characters of src into dest
 * @param dest String to copy into
 * @param src String to copy from
 * @return Pointer to `dest`
 */
extern "C" char *strcpy(char *dest, const char *src);

/**
 * @brief Allocate block of memory in heap of given size with memory alignment. This can help performance
 * @param size Size of the block
 * @param alignment Byte alignment
 * @return Pointer to newly allocated memory or nullptr on failure
 */
void *mallocAligned(uint64_t size, uint64_t alignment);

/**
 * @brief Resize given block of memory preserving the data. The original pointer ptr is invalidated and any access to it is undefined behavior (even if reallocation was in-place).
 * @param ptr Pointer to the block that needs to be resized
 * @param size Size to resize to
 * @return Pointer to the new block with preserved data
 *
 */
extern "C" void *realloc(void *ptr, uint32_t size);

/**
 * @brief Copy bytes from source to dest
 * @param dest Block of memory to copy bytes into
 * @param source Block of memory to copy bytes from
 * @param size Amount of bytes to copy
 * @return Pointer to `dest`
 * 
 */
extern "C" void* memcpy(void* dest, const void* source, uint32_t size);