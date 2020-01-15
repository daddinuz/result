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

#include <assert.h>
#include <stdbool.h>

#if !defined(__GNUC__)
#define __attribute__(...)
#endif

// internal use only, must not be used directly.
typedef unsigned char __ResultTag;

#define __RESULT_OK_TAG      0xA5u
#define __RESULT_ERR_TAG     0x5Au

/**
 * Result holds a returned value or an error providing a way of handling errors, without resorting to exception
 * handling; when a function that may fail returns a result type, the programmer is forced to consider success or failure
 * paths, before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.
 */

/**
 * Macro used to generate declarations of the result type (usually used in .h files).
 *
 * @param Struct is the name of the generated result type.
 * @param Err is the type of the error variant.
 * @param Ok is the type of the ok variant.
 * @attention the struct must be treated as opaque therefore its members must not be accessed directly, use the generated functions instead.
 */
#define ResultDeclare(Struct, Err, Ok)                                                                                  \
    struct Struct { union { Err __err; Ok __ok; }; __ResultTag __tag; };                                                \
    \
    extern struct Struct Struct##_ok(Ok ok)                                                                             \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern struct Struct Struct##_err(Err err)                                                                          \
    __attribute__((__warn_unused_result__));                                                                            \
    \
    extern bool Struct##_isOk(const struct Struct *self)                                                                \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern bool Struct##_isErr(const struct Struct *self)                                                               \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Ok Struct##_unwrap(const struct Struct *self)                                                                \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Ok Struct##_expect(const struct Struct *self, const char *fmt, ...)                                          \
    __attribute__((__warn_unused_result__, __nonnull__(1, 2), __format__(__printf__, 2, 3)));                           \
    \
    extern Err Struct##_unwrapErr(const struct Struct *self)                                                            \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                                            \
    \
    extern Err Struct##_expectErr(const struct Struct *self, const char *fmt, ...)                                      \
    __attribute__((__warn_unused_result__, __nonnull__(1, 2), __format__(__printf__, 2, 3))) /* semi-colon */

/**
 * Macro used to generate definitions of the result type (usually used in .c files).
 *
 * @param Struct is the name of the generated result type.
 * @param Err is the type of the error variant.
 * @param Ok is the type of the ok variant.
 */
#define ResultDefine(Struct, Err, Ok)                                                                                   \
    struct Struct Struct##_ok(Ok ok) {                                                                                  \
        return (struct Struct) { .__ok = ok, .__tag = __RESULT_OK_TAG };                                                \
    }                                                                                                                   \
    \
    struct Struct Struct##_err(Err err) {                                                                               \
        return (struct Struct) { .__err = err, .__tag = __RESULT_ERR_TAG };                                             \
    }                                                                                                                   \
    \
    bool Struct##_isOk(const struct Struct *const self) {                                                               \
        assert(NULL != self);                                                                                           \
        return __RESULT_OK_TAG == self->__tag;                                                                          \
    }                                                                                                                   \
    \
    bool Struct##_isErr(const struct Struct *const self) {                                                              \
        assert(NULL != self);                                                                                           \
        return __RESULT_ERR_TAG == self->__tag;                                                                         \
    }                                                                                                                   \
    \
    Ok Struct##_unwrap(const struct Struct *const self) {                                                               \
        assert(NULL != self);                                                                                           \
        if (__RESULT_OK_TAG == self->__tag) { return self->__ok; }                                                      \
        else                                { panic("unable to unwrap value"); }                                        \
    }                                                                                                                   \
    \
    Ok Struct##_expect(const struct Struct *const self, const char *const fmt, ...) {                                   \
        assert(NULL != self);                                                                                           \
        assert(NULL != fmt);                                                                                            \
        if (__RESULT_OK_TAG == self->__tag) { return self->__ok; }                                                      \
        else                                { va_list args; va_start(args, fmt); __vpanic(__TRACE__, fmt, args); }      \
    }                                                                                                                   \
    \
    Err Struct##_unwrapErr(const struct Struct *const self) {                                                           \
        assert(NULL != self);                                                                                           \
        if (__RESULT_ERR_TAG == self->__tag) { return self->__err; }                                                    \
        else                                 { panic("unable to unwrap error"); };                                      \
    }                                                                                                                   \
    \
    Err Struct##_expectErr(const struct Struct *const self, const char *const fmt, ...) {                               \
        assert(NULL != self);                                                                                           \
        assert(NULL != fmt);                                                                                            \
        if (__RESULT_ERR_TAG == self->__tag) { return self->__err; }                                                    \
        else                                 { va_list args; va_start(args, fmt); __vpanic(__TRACE__, fmt, args); }     \
    }                                                                                                                   \
    \
    static_assert(1, "") /* semi-colon */

#ifdef __cplusplus
}
#endif
