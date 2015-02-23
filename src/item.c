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
#include "item.h"


const char* financial_item_description( const financial_item_t* item )
{
	assert( item );
	return item->description;
}

void financial_item_set_description( financial_item_t* item, const char* description )
{
	assert( item );
	strncpy( item->description, description, sizeof(item->description) );
	item->description[ sizeof(item->description) - 1 ] = '\0';
}

value_t financial_item_amount( const financial_item_t* item )
{
	assert( item );
	return item->amount;
}

void financial_item_set_amount( financial_item_t* item, value_t amount )
{
	assert( item );
	item->amount = amount;
}



value_t financial_item_collection_sum( financial_item_t* collection )
{
	value_t sum = 0.0;
	const size_t count = vector_size( collection );

	for( size_t i = 0; i < count; i++ )
	{
		sum += collection[ i ].amount;
	}

	return sum;
}


static inline int financial_item_description_asc_compare( const void* l, const void* r )
{
	const financial_item_t* left  = l;
	const financial_item_t* right = r;
	return strcmp(left->description, right->description);
}

static inline int financial_item_description_des_compare( const void* l, const void* r )
{
	return -1 * financial_item_description_asc_compare( l, r );
}

static inline int financial_item_amount_asc_compare( const void* l, const void* r )
{
	const financial_item_t* left  = l;
	const financial_item_t* right = r;
	return (int) (left->amount - right->amount);
}

static inline int financial_item_amount_des_compare( const void* l, const void* r )
{
	return -1 * financial_item_amount_asc_compare( l, r );
}


static int (*financial_item_sort_methods[])( const void*, const void* ) = {
	financial_item_description_asc_compare,
	financial_item_description_des_compare,
	financial_item_amount_asc_compare,
	financial_item_amount_des_compare,
};

//void financial_item_collection_sort( financial_item_t* collection, financial_item_sort_method_t method )
void financial_item_collection_sort( void* collection, size_t item_size, financial_item_sort_method_t method )
{
	const size_t count = vector_size( collection );
	int (*compare)( const void*, const void* ) = financial_item_sort_methods[ method ];
	qsort( collection, count, item_size, compare );
}
