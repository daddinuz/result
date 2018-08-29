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
    assert(Ok != error);
    return (Result) {.__error=error, .__value=NULL};
}

Result Result_ok(void *const value) {
    return (Result) {.__error=Ok, .__value=value};
}

bool Result_isError(const Result self) {
    return Ok != self.__error;
}

bool Result_isOk(const Result self) {
    return Ok == self.__error;
}

Result Result_map(const Result self, Result (*const f)(void *)) {
    assert(NULL != f);
    return Result_isError(self) ? self : f(Result_unwrap(self));
}

Result Result_alt(const Result self, const Result a) {
    return Result_isError(self) ? a : self;
}

Result Result_chain(const Result self, Result (*const f)(void *)) {
    assert(NULL != f);
    return Result_isError(self) ? self : f(Result_unwrap(self));
}

void *Result_fold(const Result self, void *(*const whenError)(Error), void *(*const whenOk)(void *)) {
    assert(NULL != whenError);
    assert(NULL != whenOk);
    return Result_isError(self) ? whenError(self.__error) : whenOk(Result_unwrap(self));
}

void *Result_getOr(const Result self, void *const defaultValue) {
    return Result_isError(self) ? defaultValue : Result_unwrap(self);
}

void *Result_getOrElse(const Result self, void *(*const f)(void)) {
    assert(NULL != f);
    return Result_isError(self) ? f() : Result_unwrap(self);
}

Error Result_inspect(const Result self) {
    return self.__error;
}

const char *Result_explain(const Result self) {
    return self.__error->__message;
}

void *__Result_unwrap(const char *const file, const int line, const Result self) {
    assert(NULL != file);
    assert(line > 0);
    if (Result_isError(self)) {
        __Panic_terminate(file, line, "%s", self.__error->__message);
    }
    return self.__value;
}

void *__Result_expect(const char *const file, const int line, const Result self, const char *const format, ...) {
    assert(NULL != file);
    assert(line > 0);
    assert(NULL != format);
    if (Result_isError(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return self.__value;
}
