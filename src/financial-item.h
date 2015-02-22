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
#ifndef _FINANCIAL_ITEM_H_
#define _FINANCIAL_ITEM_H_

struct financial_item {
	desc_short_t          description;
	value_t               amount;
};

struct financial_asset {
	financial_item_t base;
	financial_asset_class_t asset_class;
};

struct financial_liability {
	financial_item_t base;
	financial_liability_class_t liability_class;
};

struct financial_monthly_expense {
	financial_item_t base;
};

void    financial_item_collection_sort ( void* collection, size_t item_size, financial_item_sort_method_t method );

value_t financial_asset_collection_sum           ( const financial_asset_t* collection );
value_t financial_liability_collection_sum       ( const financial_liability_t* collection );
value_t financial_monthly_expense_collection_sum ( const financial_monthly_expense_t* collection );


#endif /* _FINANCIAL_ITEM_H_ */
