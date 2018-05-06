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

#define _STR(x)     #x
#define STR(x)      _STR(x)

const char *Result_version(void) {
    return STR(RESULT_VERSION_MAJOR) "." STR(RESULT_VERSION_MINOR) "." STR(RESULT_VERSION_PATCH) RESULT_VERSION_SUFFIX;
}

Result Result_ok(void *const value) {
    assert(value);
    return (Result) {.__value=value, .__error=Ok};
}

Result Result_error(Error error) {
    assert(error);
    assert(Ok != error);
    return (Result) {.__value=NULL, .__error=error};
}

bool Result_isOk(const Result self) {
    return Ok == self.__error;
}

bool Result_isError(const Result self) {
    return Ok != self.__error;
}

void *__Result_expect(const char *const file, const int line, const Result self, const char *const format, ...) {
    assert(file);
    assert(format);
    if (Result_isError(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return self.__value;
}

void *__Result_unwrap(const char *const file, const int line, const Result self) {
    assert(file);
    return __Result_expect(file, line, self, "Error: %s.\n", self.__error->__message);
}

Error Result_inspect(const Result self) {
    return self.__error;
}

const char *Result_explain(const Result self) {
    return self.__error->__message;
}
