/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Davide Di Carlo
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
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdalign.h>

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

/**
 * Result holds a returned value or an error providing a way of handling errors, without resorting to exception
 * handling; when a function that may fail returns a result type, the programmer is forced to consider success or failure
 * paths, before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.
 */

/**
 * Macro used to generate declarations of the result type (must be used in .h files).
 *
 * @param Ident is the name of the generated result type.
 * @param ErrType is the type of the error variant.
 * @param OkType is the type of the ok variant.
 * @attention the struct must be treated as opaque therefore its members must not be accessed directly, use the generated functions instead.
 */
#define result_declare(Ident, ErrType, OkType)                                                          \
    typedef struct Ident {                                                                              \
        alignas(alignof(OkType) > alignof(ErrType) ? alignof(OkType) : alignof(ErrType))                \
        char _[(sizeof(OkType) > sizeof(ErrType) ? sizeof(OkType) : sizeof(ErrType)) + 1];              \
    } Ident;                                                                                            \
    \
    extern struct Ident Ident##_ok(OkType ok)                                                           \
    __attribute__((__warn_unused_result__));                                                            \
    \
    extern struct Ident Ident##_err(ErrType err)                                                        \
    __attribute__((__warn_unused_result__));                                                            \
    \
    extern bool Ident##_isOk(const struct Ident *self)                                                  \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                            \
    \
    extern bool Ident##_isErr(const struct Ident *self)                                                 \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                            \
    \
    extern OkType Ident##_unwrap(const struct Ident *self)                                              \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                            \
    \
    extern OkType Ident##_expect(const struct Ident *self, const char *fmt, ...)                        \
    __attribute__((__warn_unused_result__, __nonnull__(1, 2), __format__(__printf__, 2, 3)));           \
    \
    extern ErrType Ident##_unwrapErr(const struct Ident *self)                                          \
    __attribute__((__warn_unused_result__, __nonnull__(1)));                                            \
    \
    extern ErrType Ident##_expectErr(const struct Ident *self, const char *fmt, ...)                    \
    __attribute__((__warn_unused_result__, __nonnull__(1, 2), __format__(__printf__, 2, 3)));           \
    \
    static_assert(1, "") /* semi-colon */

/**
 * Macro used to generate definitions of the result type (must be used in .c files).
 *
 * @param Ident is the name of the generated result type.
 * @param ErrType is the type of the error variant.
 * @param OkType is the type of the ok variant.
 */
#define result_define(Ident, ErrType, OkType)                                                           \
    struct Ident Ident##_ok(OkType ok) {                                                                \
        struct Ident self;                                                                              \
        memcpy(self._, &ok, sizeof(OkType));                                                            \
        self._[sizeof(self._) - 1] = +42;                                                               \
        return self;                                                                                    \
    }                                                                                                   \
    \
    struct Ident Ident##_err(ErrType err) {                                                             \
        struct Ident self;                                                                              \
        memcpy(self._, &err, sizeof(ErrType));                                                          \
        self._[sizeof(self._) - 1] = -42;                                                               \
        return self;                                                                                    \
    }                                                                                                   \
    \
    bool Ident##_isOk(const struct Ident *const self) {                                                 \
        assert(NULL != self);                                                                           \
        return self->_[sizeof(self->_) - 1] == +42;                                                     \
    }                                                                                                   \
    \
    bool Ident##_isErr(const struct Ident *const self) {                                                \
        assert(NULL != self);                                                                           \
        return self->_[sizeof(self->_) - 1] == -42;                                                     \
    }                                                                                                   \
    \
    OkType Ident##_unwrap(const struct Ident *const self) {                                             \
        assert(NULL != self);                                                                           \
        panic_assert(Ident##_isOk(self));                                                               \
        OkType out;                                                                                     \
        memcpy(&out, self->_, sizeof(OkType));                                                          \
        return out;                                                                                     \
    }                                                                                                   \
    \
    OkType Ident##_expect(const struct Ident *const self, const char *const fmt, ...) {                 \
        assert(NULL != self);                                                                           \
        assert(NULL != fmt);                                                                            \
        if (Ident##_isOk(self)) { return Ident##_unwrap(self); }                                        \
        else                    { va_list args; va_start(args, fmt); panic_vabort(TRACE, fmt, args); }  \
    }                                                                                                   \
    \
    ErrType Ident##_unwrapErr(const struct Ident *const self) {                                         \
        assert(NULL != self);                                                                           \
        panic_assert(Ident##_isErr(self));                                                              \
        ErrType out;                                                                                    \
        memcpy(&out, self->_, sizeof(ErrType));                                                         \
        return out;                                                                                     \
    }                                                                                                   \
    \
    ErrType Ident##_expectErr(const struct Ident *const self, const char *const fmt, ...) {             \
        assert(NULL != self);                                                                           \
        assert(NULL != fmt);                                                                            \
        if (Ident##_isErr(self)) { return Ident##_unwrapErr(self); }                                    \
        else                     { va_list args; va_start(args, fmt); panic_vabort(TRACE, fmt, args); } \
    }                                                                                                   \
    \
    static_assert(1, "") /* semi-colon */

#ifdef __cplusplus
}
#endif
