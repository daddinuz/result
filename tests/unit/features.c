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

#include <result.h>
#include <traits/traits.h>
#include "features.h"

Feature(Result_error) {
    const size_t counter = traitsUnit_getWrappedSignalsCounter();

    traits_unit_wraps(SIGABRT) {
        Error error = NULL;
        Result _ = Result_error(error);
        (void) _;
    }
    assert_equal(traitsUnit_getWrappedSignalsCounter(), counter + 1);

    traits_unit_wraps(SIGABRT) {
        Result _ = Result_error(Ok);
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 2);

    Result sut = Result_error(DomainError);
    assert_true(Result_isError(sut));
    assert_false(Result_isOk(sut));
    assert_equal(DomainError, Result_inspect(sut));
    assert_string_equal(Error_explain(DomainError), Result_explain(sut));
}

Feature(Result_ok) {
    const size_t counter = traits_unit_get_wrapped_signals_counter();

    traits_unit_wraps(SIGABRT) {
        Result _ = Result_ok(NULL);
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);

    const char value[] = "A";
    Result sut = Result_ok(value);
    assert_false(Result_isError(sut));
    assert_true(Result_isOk(sut));
    assert_equal(Ok, Result_inspect(sut));
    assert_string_equal(Error_explain(Ok), Result_explain(sut));
    assert_equal(Result_unwrap(sut), value);
}

Feature(Result_fromNullable) {
    {
        Result sut = Result_fromNullable(NULL);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(NullReferenceError, Result_inspect(sut));
        assert_string_equal(Error_explain(NullReferenceError), Result_explain(sut));
    }

    {
        const char value[] = "A";
        Result sut = Result_fromNullable(value);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_equal(Result_unwrap(sut), value);
    }
}

const void *mapFromError(const void *_) {
    (void) _;
    assert_true(false);
    return NULL;
}

const void *mapFromOkToNull(const void *_) {
    (void) _;
    return NULL;
}

const void *mapOk(const void *_) {
    (void) _;
    return "B";
}

Feature(Result_map) {
    {
        const Result sut = Result_map(Result_error(DomainError), mapFromError);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(DomainError, Result_inspect(sut));
        assert_string_equal(Error_explain(DomainError), Result_explain(sut));
    }

    {
        const Result sut = Result_map(Result_ok("A"), mapFromOkToNull);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(NullReferenceError, Result_inspect(sut));
        assert_string_equal(Error_explain(NullReferenceError), Result_explain(sut));
    }

    {
        const Result sut = Result_map(Result_ok("A"), mapOk);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_string_equal(Result_unwrap(sut), "B");
    }
}

Result chainFromError(const void *_) {
    (void) _;
    assert_true(false);
    return Result_error(IllegalState);
}

Result chainFromOkToError(const void *_) {
    (void) _;
    return Result_error(DomainError);
}

Result chainOk(const void *_) {
    (void) _;
    return Result_ok("B");
}

Feature(Result_chain) {
    {
        const Result sut = Result_chain(Result_error(DomainError), chainFromError);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(DomainError, Result_inspect(sut));
        assert_string_equal(Error_explain(DomainError), Result_explain(sut));
    }

    {
        const Result sut = Result_chain(Result_ok("A"), chainFromOkToError);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(DomainError, Result_inspect(sut));
        assert_string_equal(Error_explain(DomainError), Result_explain(sut));
    }

    {
        const Result sut = Result_chain(Result_ok("A"), chainOk);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_string_equal(Result_unwrap(sut), "B");
    }
}

Feature(Result_alt) {
    const Result errorAlternative = Result_error(MathError);
    const Result okAlternative = Result_ok("X");

    {
        const Result sut = Result_alt(Result_error(DomainError), errorAlternative);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(MathError, Result_inspect(sut));
        assert_string_equal(Error_explain(MathError), Result_explain(sut));
    }

    {
        const Result sut = Result_alt(Result_error(DomainError), okAlternative);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_string_equal(Result_unwrap(sut), "X");
    }

    {
        const Result sut = Result_alt(Result_ok("A"), okAlternative);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_string_equal(Result_unwrap(sut), "A");
    }
}

Result orElseError(void) {
    return Result_error(MathError);
}

Result orElseOk(void) {
    return Result_ok("X");
}

Feature(Result_orElse) {
    {
        const Result sut = Result_orElse(Result_error(DomainError), orElseError);
        assert_true(Result_isError(sut));
        assert_false(Result_isOk(sut));
        assert_equal(MathError, Result_inspect(sut));
        assert_string_equal(Error_explain(MathError), Result_explain(sut));
    }

    {
        const Result sut = Result_orElse(Result_error(DomainError), orElseOk);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_string_equal(Result_unwrap(sut), "X");
    }

    {
        const Result sut = Result_orElse(Result_ok("A"), orElseOk);
        assert_false(Result_isError(sut));
        assert_true(Result_isOk(sut));
        assert_equal(Ok, Result_inspect(sut));
        assert_string_equal(Error_explain(Ok), Result_explain(sut));
        assert_string_equal(Result_unwrap(sut), "A");
    }
}

Feature(Result_unwrap) {
    Result sut = Result_ok("A");

    const char *value = Result_unwrap(sut);
    assert_string_equal(value, "A");

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Result_unwrap(Result_error(DomainError));
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Result_unwrapAsMutable) {
    char value[] = "A";
    Result sut = Result_ok(value);

    assert_equal(value, Result_unwrapAsMutable(sut));

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Result_unwrap(Result_error(DomainError));
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Result_expect) {
    Result sut = Result_ok("A");

    const char *value = Result_expect(sut, "%s", "Expected a value");
    assert_string_equal(value, "A");

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Result_expect(Result_error(DomainError), "%s", "Expected a value");
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Result_expectAsMutable) {
    char value[] = "A";
    Result sut = Result_ok(value);

    assert_equal(value, Result_expectAsMutable(sut, "%s", "Expected a value"));

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Result_expect(Result_error(DomainError), "%s", "Expected a value");
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}
