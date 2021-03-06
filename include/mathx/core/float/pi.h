// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_PI_H
#define LIBMATHXCORE_FLOAT_PI_H

#include "mathx/core/float.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API void float_assign_pi(mx_float_t *x);

LIBMATHXCORE_API void float_pi_gauss_legendre(mx_float_t *r);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_PI_H
