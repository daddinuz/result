# Result

[![Build Status](https://travis-ci.org/daddinuz/result.svg?branch=master)](https://travis-ci.org/daddinuz/result)

Result holds a returned value or an error providing a way of handling errors, without resorting to exception handling;
when a function that may fail returns a result type, the programmer is forced to consider success or failure paths,
before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.

```c
/*
 * .h
 */
#include <stdio.h>
#include <result.h>

ResultDeclare(DivisionResult, const char *, double)

struct DivisionResult division(double dividend, double divisor);

int main() {
    struct DivisionResult result = division(18, 0);

    if (DivisionResult_isOk(result)) {
        printf("%f\n", DivisionResult_expect(result, "At '%s': expected a number", __TRACE__));
    } else {
        printf("At '%s': %s\n", __TRACE__, DivisionResult_expectErr(result, "At '%s': expected an error", __TRACE__));
    }

    return 0;
}

/*
 * .c
 */
ResultDefine(DivisionResult, const char *, double)

struct DivisionResult division(double dividend, double divisor) {
    return -0.0001 <= divisor && divisor <= 0.0001 ? DivisionResult_err("division by zero") : DivisionResult_ok(dividend / divisor);
}
```
