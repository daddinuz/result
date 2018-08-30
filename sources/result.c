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

ResultView ResultView_error(Error error) {
    assert(NULL != error);
    assert(Ok != error);
    return (ResultView) {.__error=error, .__value=NULL};
}

ResultView ResultView_ok(const ResultView_Value value) {
    return (ResultView) {.__error=Ok, .__value=value};
}

bool ResultView_isError(const ResultView self) {
    return Ok != self.__error;
}

bool ResultView_isOk(const ResultView self) {
    return Ok == self.__error;
}

ResultView ResultView_map(const ResultView self, ResultView (*const f)(ResultView_Value)) {
    assert(NULL != f);
    return ResultView_isError(self) ? self : f(ResultView_unwrap(self));
}

ResultView ResultView_alt(const ResultView self, const ResultView a) {
    return ResultView_isError(self) ? a : self;
}

ResultView ResultView_chain(const ResultView self, ResultView (*const f)(ResultView_Value)) {
    assert(NULL != f);
    return ResultView_isError(self) ? self : f(ResultView_unwrap(self));
}

ResultView_Value
ResultView_fold(const ResultView self, ResultView_Value (*const whenError)(Error),
                ResultView_Value (*const whenOk)(ResultView_Value)) {
    assert(NULL != whenError);
    assert(NULL != whenOk);
    return ResultView_isError(self) ? whenError(self.__error) : whenOk(ResultView_unwrap(self));
}

ResultView_Value ResultView_getOr(const ResultView self, const ResultView_Value defaultValue) {
    return ResultView_isError(self) ? defaultValue : ResultView_unwrap(self);
}

ResultView_Value ResultView_getOrElse(const ResultView self, ResultView_Value (*const f)(void)) {
    assert(NULL != f);
    return ResultView_isError(self) ? f() : ResultView_unwrap(self);
}

Error ResultView_inspect(const ResultView self) {
    return self.__error;
}

const char *ResultView_explain(const ResultView self) {
    return self.__error->__message;
}

ResultView_Value __ResultView_unwrap(const char *const file, const int line, const ResultView self) {
    assert(NULL != file);
    assert(line > 0);
    if (ResultView_isError(self)) {
        __Panic_terminate(file, line, "%s", self.__error->__message);
    }
    return self.__value;
}

ResultView_Value
__ResultView_expect(const char *const file, const int line, const ResultView self, const char *const format, ...) {
    assert(NULL != file);
    assert(line > 0);
    assert(NULL != format);
    if (ResultView_isError(self)) {
        va_list args;
        va_start(args, format);
        __Panic_vterminate(file, line, format, args);
    }
    return self.__value;
}

Result Result_error(Error error) {
    assert(NULL != error);
    assert(Ok != error);
    return (Result) {.__error=error, .__value=NULL};
}

Result Result_ok(const Result_Value value) {
    return (Result) {.__error=Ok, .__value=value};
}

ResultView Result_toView(Result self) {
    return (ResultView) {.__error=self.__error, .__value=self.__value};
}

bool Result_isError(const Result self) {
    return Ok != self.__error;
}

bool Result_isOk(const Result self) {
    return Ok == self.__error;
}

Result Result_map(const Result self, Result (*const f)(Result_Value)) {
    assert(NULL != f);
    return Result_isError(self) ? self : f(Result_unwrap(self));
}

Result Result_alt(const Result self, const Result a) {
    return Result_isError(self) ? a : self;
}

Result Result_chain(const Result self, Result (*const f)(Result_Value)) {
    assert(NULL != f);
    return Result_isError(self) ? self : f(Result_unwrap(self));
}

Result_Value
Result_fold(const Result self, Result_Value (*const whenError)(Error), Result_Value (*const whenOk)(Result_Value)) {
    assert(NULL != whenError);
    assert(NULL != whenOk);
    return Result_isError(self) ? whenError(self.__error) : whenOk(Result_unwrap(self));
}

Result_Value Result_getOr(const Result self, const Result_Value defaultValue) {
    return Result_isError(self) ? defaultValue : Result_unwrap(self);
}

Result_Value Result_getOrElse(const Result self, Result_Value (*const f)(void)) {
    assert(NULL != f);
    return Result_isError(self) ? f() : Result_unwrap(self);
}

Error Result_inspect(const Result self) {
    return self.__error;
}

const char *Result_explain(const Result self) {
    return self.__error->__message;
}

Result_Value __Result_unwrap(const char *const file, const int line, const Result self) {
    assert(NULL != file);
    assert(line > 0);
    if (Result_isError(self)) {
        __Panic_terminate(file, line, "%s", self.__error->__message);
    }
    return self.__value;
}

Result_Value __Result_expect(const char *const file, const int line, const Result self, const char *const format, ...) {
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
