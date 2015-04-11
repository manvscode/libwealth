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
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <locale.h>
#include "wealth.h"


#define arr_len(arr)             (sizeof(arr) / sizeof(arr[0]))
#define PROFILE_FILENAME         ("./joes-financial-profile.fp")

static financial_profile_t* create_profile( void );
static void profile_updated_event( const financial_profile_t* profile, flags_t flags );

int main( int argc, char *argv[] )
{
	setlocale( LC_NUMERIC, "" );

	financial_profile_t* profile = financial_profile_load( PROFILE_FILENAME );

	if( !profile )
	{
		printf( "Creating...\n" );
		profile = create_profile();
		financial_profile_save( profile, PROFILE_FILENAME );
	}
	else
	{
		//financial_profile_sort( profile, FI_SORT_DESCRIPTION_ASC );
		//financial_profile_sort( profile, FI_SORT_DESCRIPTION_DES );
		//financial_profile_sort( profile, FI_SORT_AMOUNT_ASC );
		financial_profile_sort( profile, FI_SORT_AMOUNT_DES );
		financial_profile_print( stdout, profile );
	}

	financial_profile_destroy( &profile );
	return 0;
}

financial_profile_t* create_profile( void )
{
	financial_profile_t* profile = financial_profile_create( );
	financial_profile_set_updated_callback( profile, profile_updated_event );

	//financial_profile_set_salary( profile, 114000.0 );
	financial_profile_set_monthly_income( profile, 6400.0 );
	financial_profile_set_goal( profile, 100000.0 );

	// Assets
	{
		financial_profile_item_add( profile, FI_ASSET, "Chase Checking (885715037)", 0.0 );
		financial_profile_item_add( profile, FI_ASSET, "Simple Account", 0.0 );
		financial_profile_item_add( profile, FI_ASSET, "2005 Jeep Wrangler", 9271.0 );
		financial_profile_item_add( profile, FI_ASSET, "Fidelity: Joe (X51799718)", 1000.1 );
		financial_profile_item_add( profile, FI_ASSET, "Fidelity: Simple IRA (414155055)", 3964.57 );
		financial_profile_item_add( profile, FI_ASSET, "TD Ameritrade", 0.0 );
		financial_profile_item_add( profile, FI_ASSET, "7 Silver Coins", 140.0 );
		financial_profile_item_add( profile, FI_ASSET, "Cash", 500.0 );
		financial_profile_item_add( profile, FI_ASSET, "Gun: Glock 19C", 350.0 );
		financial_profile_item_add( profile, FI_ASSET, "Gun: Sig Saur 250", 250.0 );
		financial_profile_item_add( profile, FI_ASSET, "Gun: Sig Saur P226 MK25", 600.0 );
		financial_profile_item_add( profile, FI_ASSET, "Gun: Mossberg 590A1", 350.0 );
		financial_profile_item_add( profile, FI_ASSET, "Gun: Remington 700", 600.0 );
		financial_profile_item_add( profile, FI_ASSET, "Gun: Smith & Wesson M&P15", 400.0 );
		financial_profile_item_add( profile, FI_ASSET, "PayPal", 0.0 );
		financial_profile_item_add( profile, FI_ASSET, "Fidelity: Roth IRA (211845892)", 968.70 );
		financial_profile_item_add( profile, FI_ASSET, "Fidelity: Rollover IRA (218515595)", 2584.17 );
	}

	// Liabilities
	{
		financial_profile_item_add( profile, FI_LIABILITY, "Capital One Card (...9648)", 0.0 );
		financial_profile_item_add( profile, FI_LIABILITY, "Chase Freedom Card (...4872)", 500.0 );
		financial_profile_item_add( profile, FI_LIABILITY, "Fidelity AmEx Card (...3366)", 0.0 );
		financial_profile_item_add( profile, FI_LIABILITY, "Home Mortgage", 1100.0 );
	}

	// Expenses
	{
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "FPL", 115.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Gym", 66.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Comcast", 65.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Progressive Auto Insurance", 160.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Rent", 995.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Amazon Prime", 8.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Amazon Web Services", 52.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Food (Groceries and fast food)", 400.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Entertainment", 400.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Tolls (Joe+Kat)", 150.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Gas/Fuel", 400.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Two Haircuts", 50.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Miscellaneous", 100.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Kat's + Joe T-Mobile Service", 100.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Kat's Gas/Fuel", 320.0 );
		financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, "Kat Stipend", 390.0 );
	}

	financial_profile_refresh( profile );

	return profile;
}

void profile_updated_event( const financial_profile_t* profile, flags_t flags )
{
	if( flags & FP_FLAG_ASSETS_DIRTY )
	{
		printf( "Assets Updated!\n" );
	}

	if( flags & FP_FLAG_LIABILITIES_DIRTY )
	{
		printf( "Liabilities Updated!\n" );
	}

	if( flags & FP_FLAG_MONTHLY_EXPENSES_DIRTY )
	{
		printf( "Monthly Expenses Updated!\n" );
	}

	if( flags & FP_FLAG_INCOME_DIRTY )
	{
		printf( "Income Updated!\n" );
	}
}
