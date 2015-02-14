/*
 * Copyright (C) 2015 Joseph A. Marrero.  http://www.manvscode.com/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "wealth.h"



value_t simple_interest( value_t principle, value_t rate, value_t time )
{
	return principle * (1 + rate * time );
}

value_t compound_interest( value_t principle, value_t rate, value_t time )
{
	return principle * pow(1 + rate, time );
}

value_t annuity_present_value( value_t amount, value_t rate, value_t time )
{
	value_t d = pow(1 + rate, time );
	return (amount / rate) * (1 - (1/d));
}
value_t annuity_due_present_value( value_t amount, value_t rate, value_t time )
{
	value_t d = pow(1 + rate, time );
	return (amount / rate) * (1 - (1/d)) * (1 + rate);
}

value_t annuity_future_value( value_t amount, value_t rate, value_t time )
{
	return (amount / rate) * (pow(1 + rate, time) - 1);
}

value_t annuity_due_future_value( value_t amount, value_t rate, value_t time )
{
	return (amount / rate) * (pow(1 + rate, time) - 1) * (1 + rate);
}
