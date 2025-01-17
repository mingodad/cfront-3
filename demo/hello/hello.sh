#!/bin/sh

# manual steps to compile a program
# http://lists.nongnu.org/archive/html/tinycc-devel/2014-12/binCQHJXV7ywM.bin
#
# The CC script now work.
# Anyway, here is how to compile a C++ file manually


D=../..

if [ ! -x ${D}/cfront ]; then
    echo "error: cfront compiler not found"
    exit
fi

gcc -E ${MSIZE} -I${D}/incl hello.cpp > hello.i			# run preprocessor
${D}/cfront +a1 +L +fhello.cpp < hello.i > hello..c	# run cfront
gcc -fno-builtin-clog ${MSIZE} hello..c ${D}/libC.a -o hello.tmp			# compile and link plain C

# For static con/destructors, the nm/munch thingy is needed

nm hello.tmp | ${D}/munch > hello.cdts..c	# run mn against linked binary and filter
gcc -fno-builtin-clog ${MSIZE} hello..c hello.cdts..c ${D}/libC.a -o hello	# compile and link again

./hello
