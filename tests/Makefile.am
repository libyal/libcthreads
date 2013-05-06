AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@

TESTS = \
	test_thread.sh \
	test_mutex.sh \
	test_read_write_lock.sh \
	test_condition.sh \
	test_repeating_thread.sh

check_SCRIPTS = \
	test_condition.sh \
	test_mutex.sh \
	test_read_write_lock.sh \
	test_repeating_thread.sh \
	test_thread.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	cthreads_test_condition \
	cthreads_test_mutex \
	cthreads_test_read_write_lock \
	cthreads_test_repeating_thread \
	cthreads_test_thread

cthreads_test_condition_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_libcstring.h \
	cthreads_test_condition.c

cthreads_test_condition_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libcthreads/libcthreads.la

cthreads_test_mutex_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_libcstring.h \
	cthreads_test_mutex.c

cthreads_test_mutex_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libcthreads/libcthreads.la

cthreads_test_read_write_lock_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_libcstring.h \
	cthreads_test_read_write_lock.c

cthreads_test_read_write_lock_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libcthreads/libcthreads.la

cthreads_test_repeating_thread_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_libcstring.h \
	cthreads_test_repeating_thread.c

cthreads_test_repeating_thread_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libcthreads/libcthreads.la

cthreads_test_thread_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_libcstring.h \
	cthreads_test_thread.c

cthreads_test_thread_LDADD = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	../libcthreads/libcthreads.la

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

