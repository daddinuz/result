/*
Author: daddinuz
email:  daddinuz@gmail.com

Copyright (c) 2018 Davide Di Carlo

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <stdbool.h>
#include <error/error.h>

#if !(defined(__GNUC__) || defined(__clang__))
__attribute__(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define RESULT_VERSION_MAJOR        0
#define RESULT_VERSION_MINOR        4
#define RESULT_VERSION_PATCH        0
#define RESULT_VERSION_SUFFIX       ""
#define RESULT_VERSION_IS_RELEASE   0
#define RESULT_VERSION_HEX          0x000400

/**
 * Result holds a returned value or an error code providing a way of handling errors, without resorting to exception
 * handling; when a function that may fail returns a result type, the programmer is forced to consider success or failure
 * paths, before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.
 *
 * @attention this struct must be treated as opaque therefore its members must not be accessed directly.
 */
typedef struct {
    Error __error;
    void *const __value;
} Result;

/**
 * An helper macro used for type hinting, useful when writing interfaces.
 * By convention the annotated type is the wrapped value type and the following are the `Error` types that may be returned.
 */
#define ResultOf(type, errors...) \
    Result

/**
 * Creates a `Result` variant wrapping an `Error`.
 *
 * @attention error must not be Ok.
 */
extern Result Result_error(Error error)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Creates a `Result` variant wrapping a value.
 */
extern Result Result_ok(void *value)
__attribute__((__warn_unused_result__));

/**
 * Returns `true` if this `Result` is wrapping an `Error`, `false` otherwise.
 */
extern bool Result_isError(Result self)
__attribute__((__warn_unused_result__));

/**
 * Returns `true` if this `Result` is wrapping a value, `false` otherwise.
 */
extern bool Result_isOk(Result self)
__attribute__((__warn_unused_result__));

/**
 * If this `Result` is an `Ok` variant, apply `f` on this value else return this.
 *
 * @attention f must not be `NULL`.
 */
extern Result Result_map(Result self, Result f(void *))
__attribute__((__warn_unused_result__, __nonnull__(2)));

/**
 * If this `Result` is an `Ok` variant then this will be returned, if it is an `Error`  variant the next `Result will be returned`.
 */
extern Result Result_alt(Result self, Result a)
__attribute__((__warn_unused_result__));

/**
 * Chains several possibly failing computations.
 *
 * @attention f must not be `NULL`.
 */
extern Result Result_chain(Result self, Result f(void *))
__attribute__((__warn_unused_result__, __nonnull__(2)));

/**
 * Applies a function to each case in this `Result`.
 *
 * @attention whenError must not be `NULL`.
 * @attention whenOk must not be `NULL`.
 */
extern void *Result_fold(Result self, void *whenError(Error), void *whenOk(void *))
__attribute__((__warn_unused_result__, __nonnull__(2, 3)));

/**
 * Returns the value from this `Result` if it's an `Ok` variant or a default value if this is an `Error`.
 */
extern void *Result_getOr(Result self, void *defaultValue)
__attribute__((__warn_unused_result__));

/**
 * Returns the value from this `Result` if it's an `Ok` variant or compute a value if this is an `Error`.
 *
 * @attention f must not be `NULL`.
 */
extern void *Result_getOrElse(Result self, void *f(void))
__attribute__((__warn_unused_result__, __nonnull__(2)));

/**
 * Returns the error associated to this `Result`.
 */
extern Error Result_inspect(Result self)
__attribute__((__warn_unused_result__));

/**
 * Returns the explanations of the error associated to this `Result`.
 */
extern const char *Result_explain(Result self)
__attribute__((__warn_unused_result__));

/**
 * Unwraps the value of this `Result` if it's an `Ok` variant or panics if this is an `Error` variant.
 */
#define Result_unwrap(self) \
    __Result_unwrap((__FILE__), (__LINE__), (self))

/**
 * Unwraps the value of this `Result` if it's an `Ok` variant or panics if this is an `Error` variant with a custom message.
 */
#define Result_expect(self, ...) \
    __Result_expect((__FILE__), (__LINE__), (self), __VA_ARGS__)

/**
* @attention this function must be treated as opaque therefore must not be called directly.
*/
extern void *__Result_unwrap(const char *file, int line, Result self)
__attribute__((__nonnull__(1)));

/**
 * @attention this function must be treated as opaque therefore must not be called directly.
 */
extern void *__Result_expect(const char *file, int line, Result self, const char *format, ...)
__attribute__((__nonnull__(1, 4), __format__(__printf__, 4, 5)));

#ifdef __cplusplus
}
#endif
