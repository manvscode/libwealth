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
#include <stdint.h>
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



static financial_item_t* __financial_profile_item_add( financial_profile_t* profile, financial_item_type_t type );

struct financial_profile {

	financial_asset_t* assets;
	financial_liability_t* liabilities;
	financial_monthly_expense_t* monthly_expenses; /* monthly */

	value_t  total_assets;
	value_t  total_liabilities;
	value_t  total_monthly_expenses;
	value_t  monthly_income;
	value_t  disposable_income;
	value_t  net_worth;
	value_t  goal;
	uint8_t  flags;
	uint16_t credit_score;
	uint32_t credit_score_updated;
	uint32_t last_updated;
};


financial_profile_t* financial_profile_create( void )
{
	financial_profile_t* profile = malloc( sizeof(financial_profile_t) );

	if( profile )
	{
		vector_create( profile->assets, 1 );
		vector_create( profile->liabilities, 1 );
		vector_create( profile->monthly_expenses, 1 );

		time_t now = time( NULL );

		profile->total_assets           = 0.0;
		profile->total_liabilities      = 0.0;
		profile->total_monthly_expenses = 0.0;
		profile->monthly_income         = 0.0;
		profile->disposable_income      = 0.0;
		profile->net_worth              = 0.0;
		profile->goal                   = 0.0;
		profile->flags                  = 0;
		profile->credit_score           = 0;
		profile->credit_score_updated   = now;

		profile->last_updated           = now;
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

uint8_t financial_profile_flags( const financial_profile_t* profile )
{
	assert( profile );
	return profile->flags;
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

#define check_read(X, Y) \
	if( Y != X ) \
	{ \
		if( profile ) \
	   	{ \
			financial_profile_destroy( &profile ); \
			profile = NULL; \
		} \
		goto done; \
	}

	if( file )
	{
		financial_profile_header_t header;

		size_t objs_read = fread( &header, sizeof(header), 1, file );
		check_read( objs_read, 1 );

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
			vector_reserve( profile->assets, header.asset_count );
			vector_reserve( profile->liabilities, header.liability_count );
			vector_reserve( profile->monthly_expenses, header.monthly_expense_count );

			for( size_t i = 0; i < header.asset_count; i++ )
			{
				financial_asset_t* item = (financial_asset_t*) __financial_profile_item_add( profile, FI_ASSET );
				objs_read = fread( item, sizeof(*item), 1, file );
				check_read( objs_read, 1 );
			}

			for( size_t i = 0; i < header.liability_count; i++ )
			{
				financial_liability_t* item = (financial_liability_t*) __financial_profile_item_add( profile, FI_LIABILITY );
				objs_read = fread( item, sizeof(*item), 1, file );
				check_read( objs_read, 1 );
			}

			for( size_t i = 0; i < header.monthly_expense_count; i++ )
			{
				financial_monthly_expense_t* item = (financial_monthly_expense_t*) __financial_profile_item_add( profile, FI_MONTHLY_EXPENSE );
				objs_read = fread( item, sizeof(*item), 1, file );
				check_read( objs_read, 1 );
			}


			objs_read = fread( &profile->total_assets, sizeof(profile->total_assets), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->total_liabilities, sizeof(profile->total_liabilities), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->total_monthly_expenses, sizeof(profile->total_monthly_expenses), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->monthly_income, sizeof(profile->monthly_income), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->disposable_income, sizeof(profile->disposable_income), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->net_worth, sizeof(profile->net_worth), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->goal, sizeof(profile->goal), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->flags, sizeof(profile->flags), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->credit_score, sizeof(profile->credit_score), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->credit_score_updated, sizeof(profile->credit_score_updated), 1, file );
			check_read( objs_read, 1 );
			objs_read = fread( &profile->last_updated, sizeof(profile->last_updated), 1, file );
			check_read( objs_read, 1 );
		}
	}

done:
	if( file ) fclose( file );
	return profile;
}

bool financial_profile_save( const financial_profile_t* profile, const char* filename )
{
	bool result = false;
	FILE* file = fopen( filename, "wb" );

#define check_write(X, Y) \
	if( Y != X ) \
	{ \
		goto done; \
	}

	if( file && profile )
	{
		financial_profile_header_t header = {
			.asset_count           = vector_size(profile->assets),
			.liability_count       = vector_size(profile->liabilities),
			.monthly_expense_count = vector_size(profile->monthly_expenses)
		};

		memcpy( &header.identifier, IDENTIFIER, sizeof(IDENTIFIER) );

		size_t objs_written = fwrite( &header, sizeof(header), 1, file );
		check_write( objs_written, 1 );

		for( size_t i = 0; i < header.asset_count; i++ )
		{
			const financial_asset_t* item = (const financial_asset_t*) financial_profile_item_get( profile, FI_ASSET, i );
			objs_written = fwrite( item, sizeof(financial_asset_t), 1, file );
			check_write( objs_written, 1 );
		}

		for( size_t i = 0; i < header.liability_count; i++ )
		{
			const financial_liability_t* item = (const financial_liability_t*) financial_profile_item_get( profile, FI_LIABILITY, i );
			objs_written = fwrite( item, sizeof(financial_liability_t), 1, file );
			check_write( objs_written, 1 );
		}

		for( size_t i = 0; i < header.monthly_expense_count; i++ )
		{
			const financial_monthly_expense_t* item = (const financial_monthly_expense_t*) financial_profile_item_get( profile, FI_MONTHLY_EXPENSE, i );
			objs_written = fwrite( item, sizeof(financial_monthly_expense_t), 1, file );
			check_write( objs_written, 1 );
		}

		objs_written = fwrite( &profile->total_assets, sizeof(profile->total_assets), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->total_liabilities, sizeof(profile->total_liabilities), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->total_monthly_expenses, sizeof(profile->total_monthly_expenses), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->monthly_income, sizeof(profile->monthly_income), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->disposable_income, sizeof(profile->disposable_income), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->net_worth, sizeof(profile->net_worth), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->goal, sizeof(profile->goal), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->flags, sizeof(profile->flags), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->credit_score, sizeof(profile->credit_score), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->credit_score_updated, sizeof(profile->credit_score_updated), 1, file );
		check_write( objs_written, 1 );
		objs_written = fwrite( &profile->last_updated, sizeof(profile->last_updated), 1, file );
		check_write( objs_written, 1 );

		result = true;
	}


done:
	if( file ) fclose( file );
	return result;
}





financial_item_t* financial_profile_item_add( financial_profile_t* profile, financial_item_type_t type, const char* description, value_t amount )
{
	assert( profile );

	financial_item_t* item = __financial_profile_item_add( profile, type );
	financial_item_set_description( item, description );
	financial_item_set_amount( item, amount );

	return item;
}

financial_item_t* __financial_profile_item_add( financial_profile_t* profile, financial_item_type_t type )
{
	assert( profile );
	financial_item_t* result = NULL;

	switch( type )
	{
		case FI_ASSET:
			vector_push_emplace( profile->assets );
			result = (financial_item_t*) &vector_last( profile->assets );
			break;
		case FI_LIABILITY:
			vector_push_emplace( profile->liabilities );
			result = (financial_item_t*) &vector_last( profile->liabilities );
			break;
		case FI_MONTHLY_EXPENSE:
			vector_push_emplace( profile->monthly_expenses );
			result = (financial_item_t*) &vector_last( profile->monthly_expenses );
			break;
		default:
			break;
	}

	return result;
}

bool financial_profile_item_remove( financial_profile_t* profile, financial_item_type_t type, size_t index )
{
	bool result = false;
	switch( type )
	{
		case FI_ASSET:
		{
			size_t count = vector_size( profile->assets );
			if( index > 0 && index < count )
			{
				financial_asset_t last_item = vector_last( profile->assets );
				profile->assets[ index ] = last_item;
				vector_pop( profile->assets );
				result = true;
			}
		}
		case FI_LIABILITY:
		{
			size_t count = vector_size( profile->liabilities );
			if( index > 0 && index < count )
			{
				financial_liability_t last_item = vector_last( profile->liabilities );
				profile->liabilities[ index ] = last_item;
				vector_pop( profile->liabilities );
				result = true;
			}
		}
		case FI_MONTHLY_EXPENSE:
		{
			size_t count = vector_size( profile->monthly_expenses );
			if( index > 0 && index < count )
			{
				financial_monthly_expense_t last_item = vector_last( profile->monthly_expenses );
				profile->monthly_expenses[ index ] = last_item;
				vector_pop( profile->monthly_expenses );
				result = true;
			}
		}
		default:
			break;
	}

	return result;
}

size_t financial_profile_item_index( const financial_profile_t* profile, financial_item_type_t type, const financial_item_t* item )
{
	assert( profile );
	switch( type )
	{
		case FI_ASSET:
			return (financial_asset_t*)item - profile->assets;
		case FI_LIABILITY:
			return (financial_liability_t*)item - profile->liabilities;
		case FI_MONTHLY_EXPENSE:
			return (financial_monthly_expense_t*)item - profile->monthly_expenses;
		default:
			return 0;
	}
}

financial_item_t* financial_profile_item_get( const financial_profile_t* profile, financial_item_type_t type, size_t index )
{
	assert( profile );
	financial_item_t* result = NULL;

	switch( type )
	{
		case FI_ASSET:
		{
			size_t count = vector_size( profile->assets );
			if( index < count )
			{
				result = (financial_item_t*) &profile->assets[ index ];
			}
			break;
		}
		case FI_LIABILITY:
		{
			size_t count = vector_size( profile->liabilities );
			if( index < count )
			{
				result = (financial_item_t*) &profile->liabilities[ index ];
			}
			break;
		}
		case FI_MONTHLY_EXPENSE:
		{
			size_t count = vector_size( profile->monthly_expenses );
			if( index < count )
			{
				result = (financial_item_t*) &profile->monthly_expenses[ index ];
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
			result = vector_size( profile->assets );
			break;
		case FI_LIABILITY:
			result = vector_size( profile->liabilities );
			break;
		case FI_MONTHLY_EXPENSE:
			result = vector_size( profile->monthly_expenses );
			break;
		default:
			break;
	}

	return result;
}



void financial_profile_sort( financial_profile_t* profile, financial_item_sort_method_t method )
{
	financial_item_collection_sort( profile->assets, sizeof(*profile->assets), method );
	financial_item_collection_sort( profile->liabilities, sizeof(*profile->liabilities), method );
	financial_item_collection_sort( profile->monthly_expenses, sizeof(*profile->monthly_expenses), method );
}

void financial_profile_sort_items( financial_profile_t* profile, financial_item_type_t type, financial_item_sort_method_t method )
{
	assert( profile );

	switch( type )
	{
		case FI_ASSET:
		{
			financial_item_collection_sort( profile->assets, sizeof(*profile->assets), method );
			break;
		}
		case FI_LIABILITY:
		{
			financial_item_collection_sort( profile->liabilities, sizeof(*profile->liabilities), method );
			break;
		}
		case FI_MONTHLY_EXPENSE:
		{
			financial_item_collection_sort( profile->monthly_expenses, sizeof(*profile->monthly_expenses), method );
			break;
		}
		default:
		{
			break;
		}
	}
}

void financial_profile_refresh( financial_profile_t* profile )
{
	assert( profile );

	profile->total_assets           = financial_asset_collection_sum( profile->assets );
	profile->total_liabilities      = financial_liability_collection_sum( profile->liabilities );
	profile->total_monthly_expenses = financial_monthly_expense_collection_sum( profile->monthly_expenses );
	profile->disposable_income      = profile->monthly_income - profile->total_monthly_expenses;
	profile->net_worth              = profile->total_assets - profile->total_liabilities;

	profile->last_updated = time( NULL );
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

int16_t financial_profile_credit_score( const financial_profile_t* profile )
{
	assert( profile );
	return profile->credit_score;
}

void financial_profile_set_credit_score( financial_profile_t* profile, int16_t credit_score )
{
	assert( profile );
	profile->credit_score = credit_score;
	profile->credit_score_updated = time( NULL );
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

