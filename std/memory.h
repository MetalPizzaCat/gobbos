#pragma once
#include <stdint.h>
#include <stddef.h>

/**
 * @brief Copy pattern `patt` `size` times into the dst. Total amount of bytes copied will be `4 * size`
 *
 * @param patt
 * @param dst
 * @param size
 */
void mempattern4(uint32_t patt, void *dst, size_t size);

void *memset(void *dst, uint8_t v, size_t c);