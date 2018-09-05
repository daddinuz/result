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

#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <panic/panic.h>
#include "result.h"

Result Result_error(Error error) {
    assert(NULL != error);
    Panic_when(Ok == error);
    return (Result) {.__error=error, .__value=NULL};
}

Result Result_ok(const void *const value) {
    Panic_when(NULL == value);
    return (Result) {.__error=Ok, .__value=value};
}

Result Result_fromNullable(const void *const value) {
    return (Result) {.__error=(NULL == value) ? NullReferenceError : Ok, .__value=value};
}

bool Result_isError(const Result self) {
    return Ok != self.__error;
}

bool Result_isOk(const Result self) {
    return Ok == self.__error;
}

Result Result_map(const Result self, const void *(*const f)(const void *)) {
    Panic_when(NULL == f);
    return Result_isError(self) ? self : Result_fromNullable(f(Result_unwrap(self)));
}

Result Result_chain(const Result self, Result (*const f)(const void *)) {
    Panic_when(NULL == f);
    return Result_isError(self) ? self : f(Result_unwrap(self));
}

Result Result_alt(const Result self, const Result other) {
    return Result_isOk(self) ? self : other;
}

Result Result_orElse(const Result self, Result f(void)) {
    Panic_when(NULL == f);
    return Result_isOk(self) ? self : f();
}

Error Result_inspect(const Result self) {
    return self.__error;
}

const char *Result_explain(const Result self) {
    return Error_explain(self.__error);
}

const void *__Result_unwrap(const char *const file, const int line, const Result self) {
    assert(NULL != file);
    if (Result_isError(self)) {
        __Panic_terminate(file, line, "%s", "Unable to unwrap value");
    }
    return self.__value;
}

void *__Result_unwrapAsMutable(const char *const file, const int line, const Result self) {
    assert(NULL != file);
    if (Result_isError(self)) {
        __Panic_terminate(file, line, "%s", "Unable to unwrap value");
    }
    return (void *) self.__value;
}

const void *__Result_expect(const char *const file, const int line, const Result self, const char *const format, ...) {
    assert(NULL != file);
    assert(NULL != format);
    if (Result_isError(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return self.__value;
}

void *
__Result_expectAsMutable(const char *const file, const int line, const Result self, const char *const format, ...) {
    assert(NULL != file);
    assert(NULL != format);
    if (Result_isError(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return (void *) self.__value;
}
