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
