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
#include <math.h>
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
# include <stdbool.h>
# include <stdint.h>
#else
  typedef int bool;
  typedef unsigned char  uint8_t;
  typedef unsigned short uint16_t;
  typedef unsigned int   uint32_t;
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef double value_t;
typedef uint16_t flags_t;
typedef char desc_short_t[64];
typedef char desc_medium_t[256];
typedef char desc_long_t[1024];

#define FP_FLAG_ASSETS_DIRTY                (1 << 0)
#define FP_FLAG_LIABILITIES_DIRTY           (1 << 1)
#define FP_FLAG_MONTHLY_EXPENSES_DIRTY      (1 << 2)
#define FP_FLAG_INCOME_DIRTY                (1 << 3)


struct financial_item;
typedef struct financial_item financial_item_t;

struct financial_asset;
typedef struct financial_asset financial_asset_t;
struct financial_liability;
typedef struct financial_liability financial_liability_t;
struct financial_expense;
typedef struct financial_expense financial_expense_t;

const char* financial_item_description     ( const financial_item_t* item );
void        financial_item_set_description ( financial_item_t* item, const char* description );
value_t     financial_item_amount          ( const financial_item_t* item );
void        financial_item_set_amount      ( financial_item_t* item, value_t amount );


typedef enum financial_asset_class {
	FA_UNSPECIFIED = 0,
	FA_CASH,
	FA_EQUITY, /* Stocks, options, futures, et cetera */
	FA_FIXED_INCOME, /* Bonds */
	FA_MONEY_MARKEY,
	FA_REAL_ESTATE,
	FA_GUARANTEED,
	FA_COMMODITIES
} financial_asset_class_t;

financial_asset_class_t financial_asset_class     ( const financial_asset_t* asset );
void                    financial_asset_set_class ( financial_asset_t* asset, financial_asset_class_t cls );

typedef enum financial_liability_class {
	FL_UNSPECIFIED = 0,
	FL_CURRENT,
	FL_LONG_TERM
} financial_liability_class_t;

financial_liability_class_t financial_liability_class     ( const financial_liability_t* liability );
void                        financial_liability_set_class ( financial_liability_t* liability, financial_liability_class_t cls );

typedef enum financial_expense_class {
	FE_UNSPECIFIED = 0,
	FE_AUTOMOBILE,
	FE_BANK_CHARGES,
	FE_CHARITY,
	FE_CHILDCARE,
	FE_CLOTHING,
	FE_CREDIT_CARD_FEES,
	FE_EDUCATION,
	FE_EVENTS,
	FE_FOOD,
	FE_GIFTS,
	FE_HEALTHCARE,
	FE_HOUSEHOLD,
	FE_INSURANCE,
	FE_JOB_EXPENSES,
	FE_LEISURE,
	FE_HOBBIES,
	FE_LOANS,
	FE_PET_CARE,
	FE_SAVINGS,
	FE_TAXES,
	FE_UTILITIES,
	FE_VACATION
} financial_expense_class_t;

///financial_expense_class_t financial_expense_class     ( const financial_expense_t* expense );
///void                      financial_expense_set_class ( financial_expense_t* expense, financial_expense_class_t cls );

struct financial_profile;
typedef struct financial_profile financial_profile_t;

financial_profile_t* financial_profile_create( void );
void                 financial_profile_destroy( financial_profile_t** profile );
financial_profile_t* financial_profile_load( const char* filename );
bool                 financial_profile_save( const financial_profile_t* profile, const char* filename );


typedef enum financial_item_type {
	FI_ASSET = 0,
	FI_LIABILITY,
	FI_MONTHLY_EXPENSE
} financial_item_type_t;

financial_item_t*  financial_profile_item_add    ( financial_profile_t* profile, financial_item_type_t type, const char* description, value_t amount );
bool               financial_profile_item_remove ( financial_profile_t* profile, financial_item_type_t type, size_t index );
size_t             financial_profile_item_index  ( const financial_profile_t* profile, financial_item_type_t type, const financial_item_t* item );
financial_item_t*  financial_profile_item_get    ( const financial_profile_t* profile, financial_item_type_t type, size_t index );
size_t             financial_profile_item_count  ( const financial_profile_t* profile, financial_item_type_t type );
void               financial_profile_item_clear  ( financial_profile_t* profile, financial_item_type_t type );


typedef enum financial_item_sort_method {
	FI_SORT_DESCRIPTION_ASC = 0,
	FI_SORT_DESCRIPTION_DES,
	FI_SORT_AMOUNT_ASC,
	FI_SORT_AMOUNT_DES
} financial_item_sort_method_t;

void     financial_profile_sort        ( financial_profile_t* profile, financial_item_sort_method_t method );
void     financial_profile_sort_items  ( financial_profile_t* profile, financial_item_type_t type, financial_item_sort_method_t method );

/*
 * Callbacks
 */
typedef void (*financial_profile_updated_fxn_t)( const financial_profile_t* profile, flags_t flags );

void     financial_profile_set_updated_callback    ( financial_profile_t* profile, const financial_profile_updated_fxn_t callback );
flags_t  financial_profile_flags                   ( const financial_profile_t* profile );
void     financial_profile_clear                   ( financial_profile_t* profile );
void     financial_profile_refresh                 ( financial_profile_t* profile );
value_t  financial_profile_goal                    ( const financial_profile_t* profile );
void     financial_profile_set_goal                ( financial_profile_t* profile, value_t goal );
uint16_t financial_profile_credit_score            ( const financial_profile_t* profile );
void     financial_profile_set_credit_score        ( financial_profile_t* profile, int16_t credit_score );
uint32_t financial_profile_credit_score_last_update( const financial_profile_t* profile );
value_t  financial_profile_salary                  ( const financial_profile_t* profile );
void     financial_profile_set_salary              ( financial_profile_t* profile, value_t salary );
void     financial_profile_set_monthly_income      ( financial_profile_t* profile, value_t income );
value_t  financial_profile_monthly_income          ( const financial_profile_t* profile );
value_t  financial_profile_total_assets            ( const financial_profile_t* profile );
value_t  financial_profile_total_liabilities       ( const financial_profile_t* profile );
value_t  financial_profile_total_expenses          ( const financial_profile_t* profile );
value_t  financial_profile_disposable_income       ( const financial_profile_t* profile );
value_t  financial_profile_debt_to_income_ratio    ( const financial_profile_t* profile );
value_t  financial_profile_net_worth               ( const financial_profile_t* profile );
float    financial_profile_progress                ( const financial_profile_t* profile );

void financial_profile_print( FILE* stream, const financial_profile_t* profile );

static inline value_t simple_interest( value_t principle, value_t rate, value_t time )
{
	return principle * (1 + rate * time );
}

static inline value_t compound_interest( value_t principle, value_t rate, value_t time )
{
	return principle * pow(1 + rate, time );
}

static inline value_t annuity_present_value( value_t amount, value_t rate, value_t time )
{
	value_t d = pow(1 + rate, time );
	return (amount / rate) * (1 - (1/d));
}

static inline value_t annuity_due_present_value( value_t amount, value_t rate, value_t time )
{
	value_t d = pow(1 + rate, time );
	return (amount / rate) * (1 - (1/d)) * (1 + rate);
}

static inline value_t annuity_future_value( value_t amount, value_t rate, value_t time )
{
	return (amount / rate) * (pow(1 + rate, time) - 1);
}

static inline value_t annuity_due_future_value( value_t amount, value_t rate, value_t time )
{
	return (amount / rate) * (pow(1 + rate, time) - 1) * (1 + rate);
}

#ifdef __cplusplus
} /* extern C Linkage */
namespace wealth {
} /* namespace wealth */
#endif 
#endif /* _WEALTH_H_ */
