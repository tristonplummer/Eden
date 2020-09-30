#pragma once

#include <cstdint>

/**
 * Define an alignment macro depending on the compiler platform
 */
#if defined(_MSC_VER)
#define PACKED __declspec(align(1))
#endif
#if defined(__GNUC__)
#define PACKED __attribute__((aligned(1), packed))
#endif