// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_FLOAT_PRINT_H
#define LIBMATHXCORE_FLOAT_PRINT_H

#include "mathx/core/float.h"

#include "mathx/core/integer.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API mx_size_t float_digits_for_prec(mx_size_t prec);
LIBMATHXCORE_API mx_size_t float_sprint(char *out, mx_size_t s, const mx_float_t *x);
LIBMATHXCORE_API void float_print(const mx_float_t *x);
LIBMATHXCORE_API mx_exp_t float_dec_digits(mx_int_t *digits, const mx_float_t *x);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_FLOAT_PRINT_H
