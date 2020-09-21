# Result

[![Build Status](https://travis-ci.org/daddinuz/result.svg?branch=master)](https://travis-ci.org/daddinuz/result)

Result holds a returned value or an error providing a way of handling errors, without resorting to exception handling;
when a function that may fail returns a result type, the programmer is forced to consider success or failure paths,
before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.

```c
#include <stdio.h>
#include <result.h>

/*
 * .h
 */
enum MathError {
    DivisionByZero,
};

result_declare(NumericResult, enum MathError, double);

NumericResult divide(double numerator, double denominator);

/*
 * main.c
 */
int main() {
    NumericResult result = divide(18, 0);
    printf("%f\n", NumericResult_expect(&result, "'%s'\nError: expected a number", TRACE));
    return 0;
}

/*
 * .c
 */
result_define(NumericResult, enum MathError, double);

NumericResult divide(const double numerator, const double denominator) {
    return -0.000001 <= denominator && denominator <= 0.000001
            ? NumericResult_err(DivisionByZero)
            : NumericResult_ok(numerator / denominator);
}
```
