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

void pretty_table( double monthly_deposit, int years )
{
	time_t now = time( NULL );

	setlocale( LC_NUMERIC, "" );
	struct tm* p_tm = localtime( &now );
	int year = p_tm->tm_year + 1900;

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
	int period = 1;
	for( int y = 0; y < years; y++ )
	{
		for( int p = p_tm->tm_mon; p < p_tm->tm_mon + 12; p++ )
		{
			printf( "%-3s %4d   ", months[p % 12], year + y );
			for( int i = 0; i < arr_len(rates); i++ )
			{
				double r = rates[ i ];
				double t = annuity_future_value( monthly_deposit, r / 12, period );
				printf( "$%'-13.2f ", t );
			}

			period += 1;
			printf( "\n" );
		}
	}
}

void csv_table( double monthly_deposit, int years )
{
	time_t now = time( NULL );

	setlocale( LC_NUMERIC, "" );
	struct tm* p_tm = localtime( &now );
	int year = p_tm->tm_year + 1900;

	printf( "Month" );
	for( int i = 0; i < arr_len(rates); i++ )
	{
		double r = rates[ i ];
		printf( ",%s (%d%%)", "Total", (int) (100 * r) );
	}
	printf( "\n" );

	int period = 1;
	for( int y = 0; y < years; y++ )
	{
		for( int p = p_tm->tm_mon; p < p_tm->tm_mon + 12; p++ )
		{
			printf( "%s %d", months[p % 12], year + y );
			for( int i = 0; i < arr_len(rates); i++ )
			{
				double r = rates[ i ];
				double t = annuity_future_value( monthly_deposit, r / 12, period );
				printf( ",%.2f", t );
			}

			period += 1;
			printf( "\n" );
		}
	}
}


int main( int argc, char *argv[] )
{
	double monthly_deposit = 2000.0;
	int years = 4;

	if( argc >= 2 )
	{
		monthly_deposit = atof( argv[1] );
	}

	if( argc >= 3 )
	{
		years = atoi( argv[2] );
	}

	if( argc >= 4 && strcmp("csv", argv[3]) == 0 )
	{
		csv_table( monthly_deposit, years );
	}
	else
	{
		pretty_table( monthly_deposit, years );
	}

	return 0;
}

