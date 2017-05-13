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
#include <locale.h>
#include <wealth.h>

static bool is_leap( int year );
static int get_last_day( int month, int year );

static const char* months[] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec"
};

static const double rates[] = {
	0.01,
	0.02,
	0.04,
	0.06,
	0.10,
	0.13,
	0.16,
	0.19
};

#define arr_len(arr) (sizeof(arr) / sizeof(arr[0]))

void pretty_table( double monthly_deposit, int years, int start_year, int start_month )
{
	printf( "Monthly Deposit =  $%'.2f\n", monthly_deposit );
	printf( "Number of Years =  %d\n", years );
	printf( "\n" );

	printf( "Month   " );
	for( int i = 0; i < arr_len(rates); i++ )
	{
		double r = rates[ i ];
		printf( "%8s (%2d%%) ", "Total", (int) (100 * r) );
	}

	printf( "\n------------------------------------------------------------------------------------------------------------------------------\n" );

	int end_month = start_month + 12 * years;
	int year = start_year;
	int period = 1;

	for( int p = start_month; p < end_month; p++ )
	{
		int month = p % 12;

		if( p != start_month && month == 0 )
		{
			year += 1;
		}

		for(int q = 0; q < 2; q++ )
		{
			printf( "%-3s %d %4d   ", months[month], q == 0 ? 15 : get_last_day(month, year), year );
			for( int i = 0; i < arr_len(rates); i++ )
			{
				double r = rates[ i ];
				double t = annuity_future_value( monthly_deposit, r / 24, period );
				printf( "$%'-13.2f ", t );
			}

			period += 1;
			printf( "\n" );
		}
	}
}

void csv_table( double monthly_deposit, int years, int start_year, int start_month )
{
	printf( "Month" );
	for( int i = 0; i < arr_len(rates); i++ )
	{
		double r = rates[ i ];
		printf( ",%s (%d%%)", "Total", (int) (100 * r) );
	}
	printf( "\n" );

	int end_month = start_month + 12 * years;
	int year = start_year;
	int period = 1;

	for( int p = start_month; p < end_month; p++ )
	{
		int month = p % 12;

		if( p != start_month && month == 0 )
		{
			year += 1;
		}

		printf( "%s %d", months[p % 12], year );
		for( int i = 0; i < arr_len(rates); i++ )
		{
			double r = rates[ i ];
			double t = annuity_future_value( monthly_deposit, r / 24, period );
			printf( ",%.2f", t );
		}

		period += 1;
		printf( "\n" );
	}
}


int main( int argc, char *argv[] )
{
	double monthly_deposit = 2000.0;
	int years = 4;
	int start_year = 2015;
	int start_month = 6;

	if( argc >= 2 )
	{
		monthly_deposit = atof( argv[1] );
	}

	if( argc >= 3 )
	{
		years = atoi( argv[2] );
	}

#if 0
		time_t now = time( NULL );

		setlocale( LC_NUMERIC, "" );
		struct tm* p_tm = localtime( &now );
		start_year = p_tm->tm_year + 1900;
		start_month = p_tm->tm_mon;
#endif


	if( argc >= 4 && strcmp("csv", argv[3]) == 0 )
	{
		csv_table( monthly_deposit, years, start_year, start_month );
	}
	else
	{
		pretty_table( monthly_deposit, years, start_year, start_month );
	}

	return 0;
}

bool is_leap( int year )
{
	return year % 4 == 0;
}

int get_last_day( int month, int year )
{
	const int ndays[]={ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int result = -1;

	if( month >= 0 && month < 12 )
	{
		result = ndays[ month ];

		if( is_leap(year) && month == 1 )
		{
			result += 1;
		}
	}

	return result;
}

