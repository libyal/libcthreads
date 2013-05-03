include_HEADERS = \
	libcthreads.h

pkginclude_HEADERS = \
	libcthreads/codepage.h \
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
	/bin/rm -f libcthreads.h
	/bin/rm -f libcthreads/definitions.h
	/bin/rm -f libcthreads/features.h
	/bin/rm -f libcthreads/types.h
	/bin/rm -f Makefile

