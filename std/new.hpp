#pragma once
#include <stddef.h>
#include <memory.hpp>
void operator delete(void *ptr) noexcept;

void *operator new(size_t size);

static void *operator_new_impl(size_t size);