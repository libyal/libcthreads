AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBCTHREADS_DLL_IMPORT@

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages

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

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_library: package.m4 test_library.at test_macros.at

test_manpages: package.m4 test_macros.at test_manpages.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in

