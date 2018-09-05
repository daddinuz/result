# Result

[![Build Status](https://travis-ci.org/daddinuz/result.svg?branch=master)](https://travis-ci.org/daddinuz/result)

Result holds a returned value or an error providing a way of handling errors, without resorting to exception handling; 
when a function that may fail returns a result type, the programmer is forced to consider success or failure paths, 
before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.

```c

#include <stdio.h>

typedef const double *Number;

static Number Number_new(double number);

static Number zero(void);
static Number cube(Number number);
static ResultOf(Number, DomainError) division(Number dividend, Number divisor);
static ResultOf(Number, DomainError) squareRoot(Number number);

int main() {
    Number number = Result_unwrap(
            Result_alt(
                    Result_map(Result_chain(division(Number_new(36), Number_new(4)), squareRoot), cube),
                    Result_ok(zero())
            )
    );
    printf("Number is: %f\n", *number);
    return 0;
}

```
