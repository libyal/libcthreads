AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBCTHREADS_DLL_EXPORT@

lib_LTLIBRARIES = libcthreads.la

libcthreads_la_SOURCES = \
	libcthreads.c \
	libcthreads_condition.c libcthreads_condition.h \
	libcthreads_definitions.h \
	libcthreads_error.c libcthreads_error.h \
	libcthreads_extern.h \
	libcthreads_libcerror.h \
	libcthreads_lock.c libcthreads_lock.h \
	libcthreads_mutex.c libcthreads_mutex.h \
	libcthreads_queue.c libcthreads_queue.h \
	libcthreads_read_write_lock.c libcthreads_read_write_lock.h \
	libcthreads_repeating_thread.c libcthreads_repeating_thread.h \
	libcthreads_support.c libcthreads_support.h \
	libcthreads_thread.c libcthreads_thread.h \
	libcthreads_thread_attributes.c libcthreads_thread_attributes.h \
	libcthreads_thread_pool.c libcthreads_thread_pool.h \
	libcthreads_types.h \
	libcthreads_unused.h

libcthreads_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

libcthreads_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libcthreads_definitions.h.in \
	libcthreads.rc \
	libcthreads.rc.in

DISTCLEANFILES = \
	libcthreads_definitions.h \
	libcthreads.rc \
	Makefile \
	Makefile.in

sources-local: $(BUILT_SOURCES)

splint-local:
	@echo "Running splint on libcthreads ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcthreads_la_SOURCES)

