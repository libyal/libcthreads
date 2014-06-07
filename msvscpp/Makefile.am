MSVSCPP_FILES = \
	cthreads_test_condition/cthreads_test_condition.vcproj \
	cthreads_test_lock/cthreads_test_lock.vcproj \
	cthreads_test_mutex/cthreads_test_mutex.vcproj \
	cthreads_test_queue/cthreads_test_queue.vcproj \
	cthreads_test_read_write_lock/cthreads_test_read_write_lock.vcproj \
	cthreads_test_thread/cthreads_test_thread.vcproj \
	cthreads_test_thread_pool/cthreads_test_thread_pool.vcproj \
	libcerror/libcerror.vcproj \
	libcthreads/libcthreads.vcproj \
	libcstring/libcstring.vcproj \
	libcthreads.sln

SCRIPT_FILES = \
	scripts/vs2008_x64.sh \
	scripts/vs2008_x64_sln.sed \
	scripts/vs2008_x64_vcproj.sed \
	scripts/vs2010_x64.sh \
	scripts/vs2010_x64_sln.sed \
	scripts/vs2010_x64_vcxproj.sed

EXTRA_DIST = \
	$(MSVSCPP_FILES) \
	$(SCRIPT_FILES)

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

