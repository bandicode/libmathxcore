// Copyright (C) 2018 Vincent Chambrin
// This file is part of the libmathxcore library
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef LIBMATHXCORE_VERSION_H
#define LIBMATHXCORE_VERSION_H

#include "mathx/core/defs.h"

#ifdef __cplusplus
extern "C" {
#endif

LIBMATHXCORE_API int mx_version_major();
LIBMATHXCORE_API int mx_version_minor();
LIBMATHXCORE_API int mx_version_patch();
LIBMATHXCORE_API const char* mx_version_prerelease_suffix();
LIBMATHXCORE_API const char* mx_version_string();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMATHXCORE_VERSION_H
