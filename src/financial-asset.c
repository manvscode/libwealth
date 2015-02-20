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
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef __ANDROID__
#include <vector.h>
#else
#include <libcollections/vector.h>
#endif
#include "wealth.h"
#include "financial-item.h"

financial_asset_class_t financial_asset_class( const financial_asset_t* asset )
{
	assert( asset );
	return asset->asset_class;
}

void financial_asset_set_class( financial_asset_t* asset, financial_asset_class_t cls )
{
	assert( asset );
	asset->asset_class = cls;
}

value_t financial_asset_collection_sum( const financial_asset_t* collection )
{
	value_t sum = 0.0;
	const size_t count = vector_size( collection );

	for( size_t i = 0; i < count; i++ )
	{
		sum += collection[ i ].base.amount;
	}

	return sum;
}
