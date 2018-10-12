
#ifndef LIBMATHXCORE_MEMORY_H
#define LIBMATHXCORE_MEMORY_H

#include "mathx/core/defs.h"

LIBMATHXCORE_API mx_limb_t* mx_malloc(mx_size_t size, mx_size_t *alloc);
LIBMATHXCORE_API mx_limb_t* mx_malloc_zero(mx_size_t size, mx_size_t *alloc);

LIBMATHXCORE_API void mx_free(mx_limb_t *mem);

#endif // LIBMATHXCORE_MEMORY_H