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

#include <math.h>
#include <stdio.h>
#include <result.h>
#include <assert.h>

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

/*
 *
 */
static double numbers[4] = {};
static double *numbersCursor = numbers;
static const double *numbersEnd = numbers + sizeof(numbers) / sizeof(numbers[0]);

Number Number_new(const double number) {
    assert(numbersCursor <= numbersEnd);
    return (*zero() == number) ? zero() : (*numbersCursor = number, numbersCursor++);
}

Number zero(void) {
    static const double instance = 0;
    return &instance;
}

Number cube(Number number) {
    return Number_new(pow(*number, 3));
}

Result division(Number dividend, Number divisor) {
    return *divisor == 0.0 ? Result_error(DomainError) : Result_ok(Number_new(*dividend / *divisor));
}

Result squareRoot(Number number) {
    return *number < 0.0 ? Result_error(DomainError) : Result_ok(Number_new(sqrt(*number)));
}
