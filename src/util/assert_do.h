#ifndef UTIL_ASSERT_DO_H
#define UTIL_ASSERT_DO_H

#include <assert.h>

/*
 * This file defines the "assert_do" macro
 *
 * "assert_do" works like assert with the exception that the condition will be
 * evaluated event when NDEBUG is defined, though it will no be checked in that
 * case.
 */

#ifdef NDEBUG
#  define assert_do(X) ((void) (X))
#else
#  define assert_do(X) assert(X)
#endif

#endif
