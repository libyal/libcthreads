include_HEADERS = \
	libcthreads.h

pkginclude_HEADERS = \
	libcthreads/definitions.h \
	libcthreads/error.h \
	libcthreads/extern.h \
	libcthreads/features.h \
	libcthreads/types.h

EXTRA_DIST = \
	libcthreads.h.in \
	libcthreads/definitions.h.in \
	libcthreads/features.h.in \
	libcthreads/types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	-rm -f libcthreads.h
	-rm -f libcthreads/definitions.h
	-rm -f libcthreads/features.h
	-rm -f libcthreads/types.h
	-rm -f Makefile

