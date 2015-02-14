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
#ifndef _WEALTH_H_
#define _WEALTH_H_
#include <stddef.h>
#include <stdio.h>
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#include <stdbool.h>
#ifdef __restrict
#undef __restrict
#define __restrict restrict
#endif
#ifdef __inline
#undef __inline
#define __inline inline
#endif
#else
#define bool int
#define true 1
#define false 0
#ifdef __restrict
#undef __restrict
#define __restrict
#endif
#ifdef __inline
#undef __inline
#define __inline
#endif
#endif
#ifdef __cplusplus
extern "C" {
#endif 

typedef double value_t;
typedef char desc_short_t[64];
typedef char desc_medium_t[256];
typedef char desc_long_t[1024];


value_t simple_interest           ( value_t principle, value_t rate, value_t time );
value_t compound_interest         ( value_t principle, value_t rate, value_t time );
value_t annuity_present_value     ( value_t amount, value_t rate, value_t time );
value_t annuity_due_present_value ( value_t amount, value_t rate, value_t time );
value_t annuity_future_value      ( value_t amount, value_t rate, value_t time );
value_t annuity_due_future_value  ( value_t amount, value_t rate, value_t time );

struct financial_item;
typedef struct financial_item financial_item_t;

const char* financial_item_description     ( financial_item_t* item );
void        financial_item_set_description ( financial_item_t* item, const char* description );
value_t     financial_item_amount          ( financial_item_t* item );
void        financial_item_set_amount      ( financial_item_t* item, value_t amount );


struct financial_profile;
typedef struct financial_profile financial_profile_t;

financial_profile_t* financial_profile_create( void );
void                 financial_profile_destroy( financial_profile_t** profile );
financial_profile_t* financial_profile_load( const char* filename );
void                 financial_profile_save( const financial_profile_t* profile, const char* filename );


typedef enum financial_item_type {
	FI_ASSET,
	FI_LIABILITY,
	FI_MONTHLY_EXPENSE
} financial_item_type_t;

financial_item_t*  financial_profile_item_add    ( financial_profile_t* profile, financial_item_type_t type, const char* description, value_t amount );
bool               financial_profile_item_remove ( financial_profile_t* profile, financial_item_type_t type, size_t id );
size_t             financial_profile_item_id     ( const financial_profile_t* profile, financial_item_type_t type, const financial_item_t* item );
financial_item_t*  financial_profile_item_get    ( const financial_profile_t* profile, financial_item_type_t type, size_t id );
size_t             financial_profile_item_count  ( const financial_profile_t* profile, financial_item_type_t type );

void    financial_profile_refresh                ( financial_profile_t* profile );
value_t financial_profile_goal                   ( const financial_profile_t* profile );
void    financial_profile_set_goal               ( financial_profile_t* profile, value_t goal );
value_t financial_profile_salary                 ( const financial_profile_t* profile );
void    financial_profile_set_salary             ( financial_profile_t* profile, value_t salary );
void    financial_profile_set_monthly_income     ( financial_profile_t* profile, value_t income );
value_t financial_profile_monthly_income         ( const financial_profile_t* profile );
value_t financial_profile_total_assets           ( const financial_profile_t* profile );
value_t financial_profile_total_liabilities      ( const financial_profile_t* profile );
value_t financial_profile_total_monthly_expenses ( const financial_profile_t* profile );
value_t financial_profile_disposable_income      ( const financial_profile_t* profile );
value_t financial_profile_debt_to_income_ratio   ( const financial_profile_t* profile );
value_t financial_profile_net_worth              ( const financial_profile_t* profile );
float   financial_profile_progress               ( const financial_profile_t* profile );

#ifdef __ANDROID__
void financial_profile_print( FILE* stream, const financial_profile_t* profile );
#endif

#ifdef __cplusplus
} /* extern C Linkage */
namespace wealth {
} /* namespace wealth */
#endif 
#endif /* _WEALTH_H_ */
