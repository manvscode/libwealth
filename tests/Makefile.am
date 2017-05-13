if ENABLE_TESTS
AM_CFLAGS = -std=c11 -pg -g -ggdb -O0 -I$(top_builddir)/src/ -I/usr/local/include/ -L$(top_builddir)/lib/  -L/usr/local/lib

bin_PROGRAMS = \
$(top_builddir)/bin/savings \
$(top_builddir)/bin/savings2 \
$(top_builddir)/bin/report

__top_builddir__bin_savings_SOURCES         = savings.c
__top_builddir__bin_savings_LDADD           = $(top_builddir)/lib/.libs/libwealth.a -lcollections

__top_builddir__bin_savings2_SOURCES        = savings2.c
__top_builddir__bin_savings2_LDADD          = $(top_builddir)/lib/.libs/libwealth.a -lcollections

__top_builddir__bin_report_SOURCES         = report.c
__top_builddir__bin_report_LDADD           = $(top_builddir)/lib/.libs/libwealth.a -lcollections
endif
