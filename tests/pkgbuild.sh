#!/bin/sh
# Script to build a MacOS pkg
#
# Version: 20201121

set -e

make install DESTDIR=${PWD}/osx-pkg
mkdir -p ${PWD}/osx-pkg/usr/share/doc/libcthreads
cp AUTHORS COPYING COPYING.LESSER NEWS README ${PWD}/osx-pkg/usr/share/doc/libcthreads

VERSION=`sed '5!d; s/^ \[//;s/\],$//' configure.ac`
pkgbuild --root osx-pkg --identifier com.github.libyal.libcthreads --version ${VERSION} --ownership recommended ../libcthreads-${VERSION}.pkg

