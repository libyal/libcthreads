AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBCTHREADS_DLL_IMPORT@

TESTS = \
	test_library.sh

check_SCRIPTS = \
	test_library.sh \
	test_manpage.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	cthreads_test_condition \
	cthreads_test_error \
	cthreads_test_lock \
	cthreads_test_mutex \
	cthreads_test_queue \
	cthreads_test_read_write_lock \
	cthreads_test_repeating_thread \
	cthreads_test_support \
	cthreads_test_thread \
	cthreads_test_thread_attributes \
	cthreads_test_thread_pool

cthreads_test_condition_SOURCES = \
	cthreads_test_condition.c \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_unused.h

cthreads_test_condition_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_error_SOURCES = \
	cthreads_test_error.c \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_unused.h

cthreads_test_error_LDADD = \
	../libcthreads/libcthreads.la

cthreads_test_lock_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_lock.c \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_unused.h

cthreads_test_lock_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_mutex_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_mutex.c \
	cthreads_test_unused.h

cthreads_test_mutex_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_queue_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_queue.c \
	cthreads_test_unused.h

cthreads_test_queue_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_read_write_lock_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_read_write_lock.c \
	cthreads_test_unused.h

cthreads_test_read_write_lock_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_repeating_thread_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_repeating_thread.c \
	cthreads_test_unused.h

cthreads_test_repeating_thread_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_support_SOURCES = \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_support.c \
	cthreads_test_unused.h

cthreads_test_support_LDADD = \
	../libcthreads/libcthreads.la

cthreads_test_thread_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_thread.c \
	cthreads_test_unused.h

cthreads_test_thread_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_thread_attributes_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_thread_attributes.c \
	cthreads_test_unused.h

cthreads_test_thread_attributes_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

cthreads_test_thread_pool_SOURCES = \
	cthreads_test_libcerror.h \
	cthreads_test_libcthreads.h \
	cthreads_test_macros.h \
	cthreads_test_memory.c cthreads_test_memory.h \
	cthreads_test_thread_pool.c \
	cthreads_test_unused.h

cthreads_test_thread_pool_LDADD = \
	../libcthreads/libcthreads.la \
	@LIBCERROR_LIBADD@

DISTCLEANFILES = \
	Makefile \
	Makefile.in

