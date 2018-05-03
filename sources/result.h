/*
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 *
 * Copyright (c) 2018 Davide Di Carlo
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <error/error.h>

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

#define RESULT_VERSION_MAJOR       0
#define RESULT_VERSION_MINOR       1
#define RESULT_VERSION_PATCH       1
#define RESULT_VERSION_SUFFIX      ""
#define RESULT_VERSION_IS_RELEASE  0
#define RESULT_VERSION_HEX         0x000101

/**
 * @return The semantic versioning string of the package.
 */
extern const char *
Result_version(void)
__attribute__((__warn_unused_result__));

/**
 * Result holds a returned value or an error code providing a way of handling errors, without resorting to exception
 * handling; when a function that may fail returns a result type, the programmer is forced to consider success or failure
 * paths, before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.
 *
 * @attention
 *  This type must be treated as opaque therefore its members should never be accessed directly.
 */
typedef struct __Result {
    void *__value;
    Error __error;
} Result;

/**
 * An helper macro used for type hinting, useful when writing interfaces.
 * By convention every symbol following the first one are Error types.
 */
#define ResultOf(xType, ...) \
    Result

/**
 * Creates a Result notifying a successful execution.
 *
 * @param value The result value <b>must not be NULL</b>.
 * @return A new Result instance wrapping value.
 */
extern Result
Result_ok(void *value)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Creates a Result notifying a unsuccessful execution.
 *
 * @param error The Error instance <b>must not be Ok</b>.
 * @return A new Result instance wrapping an error.
 */
extern Result
Result_error(Error error)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Tests if Result is Ok
 *
 * @param self The Result instance.
 * @return true if Result is Ok else false.
 */
extern bool
Result_isOk(Result self)
__attribute__((__warn_unused_result__));

/**
 * Tests if Result is Error
 *
 * @param self The Result instance.
 * @return true if Result is Error else false.
 */
extern bool
Result_isError(Result self)
__attribute__((__warn_unused_result__));

/**
 * Unwraps a Result, yielding its wrapped value if it's Ok.
 * Aborts execution if Result is Error..
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param file The file name.
 * @param line The line number.
 * @param self The Result instance.
 * @param format The custom panic message (overrides the associated Error panic message).
 * @param ... The format params.
 * @return The unwrapped value or terminates the execution.
 */
extern void *
__Result_expect(const char *file, int line, Result self, const char *format, ...)
__attribute__((__warn_unused_result__, __nonnull__, __format__(__printf__, 4, 5)));

/**
 * @see __Result_expect(const char *file, int line, Result self, const char *format, ...)
 */
#define Result_expect(xSelf, ...) \
  __Result_expect(__FILE__, __LINE__, (xSelf), __VA_ARGS__)

/**
 * Unwraps a Result, yielding its wrapped value if it's Ok.
 * Aborts execution if Result is wrapping an Error.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param file The file name.
 * @param line The line number.
 * @param self The Result instance.
 * @return The unwrapped value or terminates the execution.
 */
extern void *
__Result_unwrap(const char *file, int line, Result self)
__attribute__((__warn_unused_result__, __nonnull__(1)));

/**
 * @see __Result_unwrap(const char *file, int line, Result self)
 */
#define Result_unwrap(xSelf) \
  __Result_unwrap(__FILE__, __LINE__, (xSelf))

/**
 * Inspects the result error.
 *
 * @param self The Result instance.
 * @return The Error associated to the result.
 */
extern Error
Result_inspect(Result self)
__attribute__((__warn_unused_result__));

/**
 * Gets the result error explanation.
 * Same as: Error_message(Result_inspect(result))
 *
 * @param self The Result instance.
 * @return The error explanation.
 */
extern const char *
Result_explain(Result self)
__attribute__((__warn_unused_result__));

#ifdef __cplusplus
}
#endif
