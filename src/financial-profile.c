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
#include <stdio.h>
#include <time.h>
#include <math.h>
#define VECTOR_GROW_AMOUNT(array)      (1)
#ifdef __ANDROID__
#include <vector.h>
#else
#include <libcollections/vector.h>
#endif
#include "wealth.h"
#include "financial-item.h"

static financial_item_t* __financial_profile_item_add( financial_profile_t* profile, financial_item_type_t type, const financial_item_t* item );

struct financial_profile {
	financial_item_t* assets;
	financial_item_t* liabilities;
	financial_item_t* monthly_expenses; /* monthly */

	value_t total_assets;
	value_t total_liabilities;
	value_t total_monthly_expenses;
	value_t monthly_income;
	value_t disposable_income;
	value_t net_worth;
	value_t goal;

	time_t last_updated;
};


financial_profile_t* financial_profile_create( void )
{
	financial_profile_t* profile = malloc( sizeof(financial_profile_t) );

	if( profile )
	{
		vector_create( profile->assets, 1 );
		vector_create( profile->liabilities, 1 );
		vector_create( profile->monthly_expenses, 1 );

		profile->total_assets           = 0.0;
		profile->total_liabilities      = 0.0;
		profile->total_monthly_expenses = 0.0;
		profile->monthly_income         = 0.0;
		profile->disposable_income      = 0.0;
		profile->net_worth              = 0.0;
		profile->goal                   = 0.0;
	}

	return profile;
}

void financial_profile_destroy( financial_profile_t** p_profile )
{
	if( p_profile && *p_profile )
	{
		financial_profile_t* profile = *p_profile;

		vector_destroy( profile->assets );
		vector_destroy( profile->liabilities );
		vector_destroy( profile->monthly_expenses );

		free( profile );
		*p_profile = NULL;
	}
}


static const uint8_t IDENTIFIER[] = { 'F', 'P', '\0', '\0' };


typedef struct financial_profile_header {
	uint8_t identifier[ 4 ];
	uint32_t asset_count;
	uint32_t liability_count;
	uint32_t monthly_expense_count;
} financial_profile_header_t;


financial_profile_t* financial_profile_load( const char* filename )
{
	financial_profile_t* profile = NULL;
	FILE* file = fopen( filename, "rb" );

	if( file )
	{
		financial_profile_header_t header;

		fread( &header, sizeof(header), 1, file );

		if( memcmp(header.identifier, IDENTIFIER, sizeof(header.identifier) ) != 0 )
		{
			goto done;
		}
		else
		{
			profile = financial_profile_create( );
		}

		if( profile )
		{
			vector_resize( profile->assets, header.asset_count );
			vector_resize( profile->liabilities, header.liability_count );
			vector_resize( profile->monthly_expenses, header.monthly_expense_count );

			for( size_t i = 0; i < header.asset_count; i++ )
			{
				financial_item_t item;
				fread( &item, sizeof(financial_item_t), 1, file );
				__financial_profile_item_add( profile, FI_ASSET, &item );
			}

			for( size_t i = 0; i < header.liability_count; i++ )
			{
				financial_item_t item;
				fread( &item, sizeof(financial_item_t), 1, file );
				__financial_profile_item_add( profile, FI_LIABILITY, &item );
			}

			for( size_t i = 0; i < header.monthly_expense_count; i++ )
			{
				financial_item_t item;
				fread( &item, sizeof(financial_item_t), 1, file );
				__financial_profile_item_add( profile, FI_MONTHLY_EXPENSE, &item );
			}


			fread( &profile->total_assets, sizeof(profile->total_assets), 1, file );
			fread( &profile->total_liabilities, sizeof(profile->total_liabilities), 1, file );
			fread( &profile->total_monthly_expenses, sizeof(profile->total_monthly_expenses), 1, file );
			fread( &profile->monthly_income, sizeof(profile->monthly_income), 1, file );
			fread( &profile->disposable_income, sizeof(profile->disposable_income), 1, file );
			fread( &profile->net_worth, sizeof(profile->net_worth), 1, file );
			fread( &profile->goal, sizeof(profile->goal), 1, file );

			fread( &profile->last_updated, sizeof(profile->last_updated), 1, file );
		}
	}

done:
	fclose( file );
	return profile;
}

void financial_profile_save( const financial_profile_t* profile, const char* filename )
{
	FILE* file = fopen( filename, "wb" );

	if( file && profile )
	{
		financial_profile_header_t header = {
			.asset_count           = vector_length(profile->assets),
			.liability_count       = vector_length(profile->liabilities),
			.monthly_expense_count = vector_length(profile->monthly_expenses)
		};
		memcpy( &header.identifier, IDENTIFIER, sizeof(IDENTIFIER) );

		fwrite( &header, sizeof(header), 1, file );

		for( size_t i = 0; i < header.asset_count; i++ )
		{
			const financial_item_t* item = financial_profile_item_get( profile, FI_ASSET, i );
			fwrite( item, sizeof(financial_item_t), 1, file );
		}

		for( size_t i = 0; i < header.liability_count; i++ )
		{
			const financial_item_t* item = financial_profile_item_get( profile, FI_LIABILITY, i );
			fwrite( item, sizeof(financial_item_t), 1, file );
		}

		for( size_t i = 0; i < header.monthly_expense_count; i++ )
		{
			const financial_item_t* item = financial_profile_item_get( profile, FI_MONTHLY_EXPENSE, i );
			fwrite( item, sizeof(financial_item_t), 1, file );
		}


		fwrite( &profile->total_assets, sizeof(profile->total_assets), 1, file );
		fwrite( &profile->total_liabilities, sizeof(profile->total_liabilities), 1, file );
		fwrite( &profile->total_monthly_expenses, sizeof(profile->total_monthly_expenses), 1, file );
		fwrite( &profile->monthly_income, sizeof(profile->monthly_income), 1, file );
		fwrite( &profile->disposable_income, sizeof(profile->disposable_income), 1, file );
		fwrite( &profile->net_worth, sizeof(profile->net_worth), 1, file );
		fwrite( &profile->goal, sizeof(profile->goal), 1, file );

		fwrite( &profile->last_updated, sizeof(profile->last_updated), 1, file );
	}

	fclose( file );
}





financial_item_t* financial_profile_item_add( financial_profile_t* profile, financial_item_type_t type, const char* description, value_t amount )
{
	assert( profile );

	financial_item_t item;
	financial_item_set_description( &item, description );
	financial_item_set_amount( &item, amount );

	return __financial_profile_item_add( profile, type, &item );
}

financial_item_t* __financial_profile_item_add( financial_profile_t* profile, financial_item_type_t type, const financial_item_t* item )
{
	assert( profile );
	assert( item );
	financial_item_t* result = NULL;

	switch( type )
	{
		case FI_ASSET:
			vector_push( profile->assets, *item );
			result = &vector_last( profile->assets );
			break;
		case FI_LIABILITY:
			vector_push( profile->liabilities, *item );
			result = &vector_last( profile->liabilities );
			break;
		case FI_MONTHLY_EXPENSE:
			vector_push( profile->monthly_expenses, *item );
			result = &vector_last( profile->monthly_expenses );
			break;
		default:
			break;
	}

	return result;
}

bool financial_profile_item_remove( financial_profile_t* profile, financial_item_type_t type, size_t id )
{
	bool result = false;
	switch( type )
	{
		case FI_ASSET:
		{
			size_t count = vector_length( profile->assets );
			if( id > 0 && id < count )
			{
				financial_item_t last_item = vector_last( profile->assets );
				profile->assets[ id ] = last_item;
				vector_pop( profile->assets );
				result = true;
			}
		}
		case FI_LIABILITY:
		{
			size_t count = vector_length( profile->liabilities );
			if( id > 0 && id < count )
			{
				financial_item_t last_item = vector_last( profile->liabilities );
				profile->liabilities[ id ] = last_item;
				vector_pop( profile->liabilities );
				result = true;
			}
		}
		case FI_MONTHLY_EXPENSE:
		{
			size_t count = vector_length( profile->monthly_expenses );
			if( id > 0 && id < count )
			{
				financial_item_t last_item = vector_last( profile->monthly_expenses );
				profile->monthly_expenses[ id ] = last_item;
				vector_pop( profile->monthly_expenses );
				result = true;
			}
		}
		default:
			break;
	}

	return result;
}

size_t financial_profile_item_id( const financial_profile_t* profile, financial_item_type_t type, const financial_item_t* item )
{
	assert( profile );
	switch( type )
	{
		case FI_ASSET:
			return item - profile->assets;
		case FI_LIABILITY:
			return item - profile->liabilities;
		case FI_MONTHLY_EXPENSE:
			return item - profile->monthly_expenses;
		default:
			return 0;
	}
}

financial_item_t* financial_profile_item_get( const financial_profile_t* profile, financial_item_type_t type, size_t id )
{
	assert( profile );
	financial_item_t* result = NULL;

	switch( type )
	{
		case FI_ASSET:
		{
			size_t count = vector_length( profile->assets );
			if( id < count )
			{
				result = &profile->assets[ id ];
			}
			break;
		}
		case FI_LIABILITY:
		{
			size_t count = vector_length( profile->liabilities );
			if( id < count )
			{
				result = &profile->liabilities[ id ];
			}
			break;
		}
		case FI_MONTHLY_EXPENSE:
		{
			size_t count = vector_length( profile->monthly_expenses );
			if( id < count )
			{
				result = &profile->monthly_expenses[ id ];
			}
			break;
		}
		default:
			break;
	}

	return result;
}

size_t financial_profile_item_count( const financial_profile_t* profile, financial_item_type_t type )
{
	assert( profile );
	size_t result = 0;

	switch( type )
	{
		case FI_ASSET:
			result = vector_length( profile->assets );
			break;
		case FI_LIABILITY:
			result = vector_length( profile->liabilities );
			break;
		case FI_MONTHLY_EXPENSE:
			result = vector_length( profile->monthly_expenses );
			break;
		default:
			break;
	}

	return result;
}


static inline value_t financial_item_collection_sum( financial_item_t* collection )
{
	value_t sum = 0.0;
	const size_t count = vector_length( collection );

	for( size_t i = 0; i < count; i++ )
	{
		sum += collection[ i ].amount;
	}

	return sum;
}

static int financial_item_compare( const void* l, const void* r )
{
	const financial_item_t* left  = l;
	const financial_item_t* right = r;
	return strcmp(left->description, right->description);
}

static inline void financial_item_collection_sort( financial_item_t* collection )
{
	const size_t count = vector_length( collection );
#if 0
	for( int i = 2; i < count; i++ )
	{
		for( int j = i; j > 1 && strcmp(collection[ j ].description, collection[ j - 1 ].description) < 0; j-- )
		{
			financial_item_t temp = collection[ j ];
			collection[ j ]       = collection[ j - 1 ];
			collection[ j - 1 ]   = temp;
		}
	}
#else
	qsort( collection, count, sizeof(financial_item_t), financial_item_compare );
#endif
}


void financial_profile_refresh( financial_profile_t* profile )
{
	assert( profile );

	profile->total_assets           = financial_item_collection_sum( profile->assets );
	profile->total_liabilities      = financial_item_collection_sum( profile->liabilities );
	profile->total_monthly_expenses = financial_item_collection_sum( profile->monthly_expenses );
	profile->disposable_income      = profile->monthly_income - profile->total_monthly_expenses;
	profile->net_worth              = profile->total_assets - profile->total_liabilities;

	profile->last_updated = time( NULL );
}

void financial_profile_sort( financial_profile_t* profile )
{
	assert( profile );

	financial_item_collection_sort( profile->assets );
	financial_item_collection_sort( profile->liabilities );
	financial_item_collection_sort( profile->monthly_expenses );
}

value_t financial_profile_goal( const financial_profile_t* profile )
{
	assert( profile );
	return profile->goal;
}

void financial_profile_set_goal( financial_profile_t* profile, value_t goal )
{
	assert( profile );
	profile->goal = goal;
}

value_t financial_profile_salary( const financial_profile_t* profile )
{
	assert( profile );
	return profile->monthly_income * 12;
}

void financial_profile_set_salary( financial_profile_t* profile, value_t salary )
{
	assert( profile );
	profile->monthly_income = salary / 12.0;
}

value_t financial_profile_monthly_income( const financial_profile_t* profile )
{
	assert( profile );
	return profile->monthly_income;
}

void financial_profile_set_monthly_income( financial_profile_t* profile, value_t income )
{
	assert( profile );
	profile->monthly_income = income;
}

value_t financial_profile_total_assets( const financial_profile_t* profile )
{
	assert( profile );
	return profile->total_assets;
}

value_t financial_profile_total_liabilities( const financial_profile_t* profile )
{
	assert( profile );
	return profile->total_liabilities;
}

value_t financial_profile_total_monthly_expenses( const financial_profile_t* profile )
{
	assert( profile );
	return profile->total_monthly_expenses;
}

value_t financial_profile_disposable_income( const financial_profile_t* profile )
{
	assert( profile );
	return profile->disposable_income;
}

value_t financial_profile_debt_to_income_ratio( const financial_profile_t* profile )
{
	assert( profile );
	value_t result = 0.0;

	if( profile->monthly_income > 0.0 )
	{
		result = profile->total_liabilities / (profile->monthly_income * 12);
	}

	return result;
}

value_t financial_profile_net_worth( const financial_profile_t* profile )
{
	assert( profile );
	return profile->net_worth;
}

float financial_profile_progress( const financial_profile_t* profile )
{
	float result = 0.0f;

	if( profile->goal > profile->net_worth )
	{
		result = (float) (profile->net_worth / profile->goal);
	}

	return result;
}

static const char* percent_format( float p )
{
	static char buffer[ 32 ];
	snprintf( buffer, sizeof(buffer), "%.1f%%", 100 * p );
	buffer[ sizeof(buffer) - 1 ] = '\0';
	return buffer;
}

void financial_profile_print( FILE* stream, const financial_profile_t* profile )
{
	size_t max_lines = financial_profile_item_count( profile, FI_ASSET );
	max_lines = financial_profile_item_count( profile, FI_LIABILITY ) > max_lines ? financial_profile_item_count( profile, FI_LIABILITY ) : max_lines;
	max_lines = financial_profile_item_count( profile, FI_MONTHLY_EXPENSE ) > max_lines ? financial_profile_item_count( profile, FI_MONTHLY_EXPENSE ) : max_lines;


	fprintf( stream, "+-------------------------------------------------"
                     "+-------------------------------------------------"
                     "+-------------------------------------------------+\n" );
	fprintf( stream, "| %-47s | %-47s | %-47s |\n", "                    ASSETS", "                  LIABILITIES", "                   EXPENSES" );
	fprintf( stream, "+-------------------------------------------------"
                     "+-------------------------------------------------"
                     "+-------------------------------------------------+\n" );

	for( size_t i = 0; i < max_lines; i++ )
	{
		financial_item_t* ass = financial_profile_item_get( profile, FI_ASSET, i );
		financial_item_t* lia = financial_profile_item_get( profile, FI_LIABILITY, i );
		financial_item_t* exp = financial_profile_item_get( profile, FI_MONTHLY_EXPENSE, i );


		if( ass )
		{
			fprintf( stream, "| %34s: $%'-10.2f ", financial_item_description(ass), financial_item_amount(ass) );
		}
		else
		{
			fprintf( stream, "| %35s %11s ", "", "" );
		}

		if( lia )
		{
			fprintf( stream, "| %34s: $%'-10.2f ", financial_item_description(lia), financial_item_amount(lia) );
		}
		else
		{
			fprintf( stream, "| %35s %11s ", "", "" );
		}

		if( exp )
		{
			fprintf( stream, "| %34s: $%'-10.2f |", financial_item_description(exp), financial_item_amount(exp) );
		}
		else
		{
			fprintf( stream, "| %35s %11s |", "", "" );
		}

		fprintf( stream, "\n" );
	}

	fprintf( stream, "+-------------------------------------------------"
			"+-------------------------------------------------"
			"+-------------------------------------------------+\n" );
	fprintf( stream, "| %28s $%'-17d | %47s | %28s -$%'-16.2f |\n", "NET WORTH:", (int) round(financial_profile_net_worth(profile)), "", "TOTAL EXPENSES:", financial_profile_total_monthly_expenses(profile) );
	fprintf( stream, "| %28s $%'-17d | %47s | %28s $%'-17.2f |\n", "GOAL:", (int) round(financial_profile_goal(profile)), "", "MONTHLY INCOME:", financial_profile_monthly_income(profile) );
	fprintf( stream, "| %28s %-18s | %47s | %28s $%'-17.2f |\n", "PROGRESS:", percent_format(financial_profile_progress(profile)), "", "DISPOSABLE INCOME:", financial_profile_disposable_income(profile) );
	fprintf( stream, "| %28s %-18s | %47s | %47s |\n", "DEBT TO INCOME RATIO:", percent_format(financial_profile_debt_to_income_ratio(profile)), "", "" );
	fprintf( stream, "+-------------------------------------------------+ %47s +-------------------------------------------------+\n", "" );
}

