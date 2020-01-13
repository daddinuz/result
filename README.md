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

ResultDeclare(NumericResult, const char *, double);

struct NumericResult divide(double numerator, double denominator);

int main() {
    struct NumericResult number = divide(18, 0);
    printf("%f\n", NumericResult_expect(number, "'%s': expected a number", __TRACE__));
    return 0;
}

/*
 * .c
 */
ResultDefine(NumericResult, const char *, double);

struct NumericResult divide(const double numerator, const double denominator) {
    return -0.0001 <= denominator && denominator <= 0.0001 ? NumericResult_err("division by zero") : NumericResult_ok(numerator / denominator);
}
```
