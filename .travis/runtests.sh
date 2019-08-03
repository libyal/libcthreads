#!/bin/bash
# Script to run tests on Travis-CI
#
# Version: 20190802

./configure ${CONFIGURE_OPTIONS};
RESULT=$?;

if test ${RESULT} -eq 0;
then
	make > /dev/null;
	RESULT=$?;
fi
if test ${RESULT} -eq 0;
then
	if test ${TARGET} = "macos-gcc-python" || test ${TARGET} = "macos-gcc-pkgbuild";
	then
		install_name_tool -change /usr/local/lib/libcthreads.1.dylib ${PWD}/libcthreads/.libs/libcthreads.1.dylib ./pycthreads/.libs/pycthreads.so;
	fi
	make check CHECK_WITH_STDERR=1;
	RESULT=$?;
fi
if test ${RESULT} -ne 0;
then
        if test -f tests/test-suite.log;
	then
		cat tests/test-suite.log;
	fi
fi
exit ${RESULT};

