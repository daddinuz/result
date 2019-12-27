/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Davide Di Carlo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <panic/panic.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#if !defined(__GNUC__)
#define __attribute__(...)
#endif

// internal use only, must not be used directly.
#define __RESULT_OK_TAG    0xA5
#define __RESULT_ERR_TAG   0x5A

/**
 * Result holds a returned value or an error providing a way of handling errors, without resorting to exception
 * handling; when a function that may fail returns a result type, the programmer is forced to consider success or failure
 * paths, before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.
 */

/**
 * Macro used to generate declarations of the result type (usually used in .h files).
 *
 * @param NewType is the name of the generated result type.
 * @param Err is the type of the error variant.
 * @param Ok is the type of the ok variant.
 * @attention the struct must be treated as opaque therefore its members must not be accessed directly, use the generated functions instead.
 */
#define ResultDeclare(NewType, Err, Ok)                                                                                 \
    struct NewType { union { Err __err; Ok __ok; }; unsigned char __tag; };                                             \
    \
    extern struct NewType NewType##_ok(Ok ok)                                                                           \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern struct NewType NewType##_err(Err err)                                                                        \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool NewType##_isOk(struct NewType self)                                                                     \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool NewType##_isErr(struct NewType self)                                                                    \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern Ok NewType##_unwrap(struct NewType self)                                                                     \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern Ok NewType##_expect(struct NewType self, const char *fmt, ...)                                               \
    __attribute__((__warn_unused_result__, __nonnull__(2), __format__(__printf__, 2, 3)));                              \
    \
    extern Err NewType##_unwrapErr(struct NewType self)                                                                 \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern Err NewType##_expectErr(struct NewType self, const char *fmt, ...)                                           \
    __attribute__((__warn_unused_result__, __nonnull__(2), __format__(__printf__, 2, 3)));

/**
 * Macro used to generate definitions of the result type (usually used in .c files).
 *
 * @param NewType is the name of the generated result type.
 * @param Err is the type of the error variant.
 * @param Ok is the type of the ok variant.
 */
#define ResultDefine(NewType, Err, Ok)                                                                                  \
    struct NewType NewType##_ok(Ok ok) {                                                                                \
        return (struct NewType) { .__ok = ok, .__tag = __RESULT_OK_TAG };                                               \
    }                                                                                                                   \
    \
    struct NewType NewType##_err(Err err) {                                                                             \
        return (struct NewType) { .__err = err, .__tag = __RESULT_ERR_TAG };                                            \
    }                                                                                                                   \
    \
    bool NewType##_isOk(const struct NewType self) {                                                                    \
        return __RESULT_OK_TAG == self.__tag;                                                                           \
    }                                                                                                                   \
    \
    bool NewType##_isErr(const struct NewType self) {                                                                   \
        return __RESULT_ERR_TAG == self.__tag;                                                                          \
    }                                                                                                                   \
    \
    Ok NewType##_unwrap(const struct NewType self) {                                                                    \
        if (__RESULT_OK_TAG == self.__tag) { return self.__ok; }                                                        \
        else                               { panic("unable to unwrap value"); }                                         \
    }                                                                                                                   \
    \
    Ok NewType##_expect(const struct NewType self, const char *const fmt, ...) {                                        \
        if (__RESULT_OK_TAG == self.__tag) { return self.__ok; }                                                        \
        else                               { va_list args; va_start(args, fmt); __vpanic(__TRACE__, fmt, args); }       \
    }                                                                                                                   \
    \
    Err NewType##_unwrapErr(const struct NewType self) {                                                                \
        if (__RESULT_ERR_TAG == self.__tag) { return self.__err; }                                                      \
        else                                { panic("unable to unwrap error"); };                                       \
    }                                                                                                                   \
    \
    Err NewType##_expectErr(const struct NewType self, const char *const fmt, ...) {                                    \
        if (__RESULT_ERR_TAG == self.__tag) { return self.__err; }                                                      \
        else                                { va_list args; va_start(args, fmt); __vpanic(__TRACE__, fmt, args); }      \
    }

#ifdef __cplusplus
}
#endif
