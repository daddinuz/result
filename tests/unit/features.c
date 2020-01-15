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

#include <traits/traits.h>
#include "features.h"

ResultDefine(DivisionResult, const char *, double);

Feature(Result_ok) {
    const double value = 5.0;
    struct DivisionResult sut = DivisionResult_ok(value);

    assert_that(DivisionResult_isOk(&sut));
    assert_false(DivisionResult_isErr(&sut));

    {
        const double ok = DivisionResult_unwrap(&sut);
        assert_equal(value, ok);
    }

    {
        const double ok = DivisionResult_expect(&sut, "expected a value");
        assert_equal(value, ok);
    }

    const size_t sigcount = traitsUnit_getWrappedSignalsCounter();

    traitsUnit_wrap(SIGABRT) {
        const char *const err = DivisionResult_unwrapErr(&sut);
        (void) err;
    }

    assert_equal(sigcount + 1, traitsUnit_getWrappedSignalsCounter());

    traitsUnit_wrap(SIGABRT) {
        const char *const err = DivisionResult_expectErr(&sut, "expected an error");
        (void) err;
    }

    assert_equal(sigcount + 2, traitsUnit_getWrappedSignalsCounter());
}

Feature(Result_err) {
    const char *const value = "error";
    struct DivisionResult sut = DivisionResult_err(value);

    assert_that(DivisionResult_isErr(&sut));
    assert_false(DivisionResult_isOk(&sut));

    {
        const char *const err = DivisionResult_unwrapErr(&sut);
        assert_equal(value, err);
    }

    {
        const char *const err = DivisionResult_expectErr(&sut, "expected a value");
        assert_equal(value, err);
    }

    const size_t sigcount = traitsUnit_getWrappedSignalsCounter();

    traitsUnit_wrap(SIGABRT) {
        const double value = DivisionResult_unwrap(&sut);
        (void) value;
    }

    assert_equal(sigcount + 1, traitsUnit_getWrappedSignalsCounter());

    traitsUnit_wrap(SIGABRT) {
        const double value = DivisionResult_expect(&sut, "expected a value");
        (void) value;
    }

    assert_equal(sigcount + 2, traitsUnit_getWrappedSignalsCounter());
}
