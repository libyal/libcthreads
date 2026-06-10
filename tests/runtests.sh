#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libcthreads/.libs/libcthreads.1.dylib && test -f ./pycthreads/.libs/pycthreads.so
then
	install_name_tool -change /usr/local/lib/libcthreads.1.dylib ${PWD}/libcthreads/.libs/libcthreads.1.dylib ./pycthreads/.libs/pycthreads.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

