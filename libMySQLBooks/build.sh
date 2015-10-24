#!/bin/sh

forcearch=$1

machine=`gcc -dumpmachine`
set `echo $machine | tr '-' ' '`

arch=$1
name1=$2
name2=$3

sysroot=""
# universals on PPC must be linked against 10.4u according to http://developer.apple.com/technotes/tn2005/tn2137.html
if `echo $arch | grep ppc > /dev/null`; then
   sysroot="/Developer/SDKs/MacOSX10.4u.sdk"
fi

makefile="Makefile.linux"
# the 'weakest link' of all the libraries...
libfile=$SYSROOT/usr/lib/libstdc++.6.dylib

mac=0
if test $name1 = linux; then
   makefile="Makefile.linux"
elif test $name2 = linux; then
   makefile="Makefile.linux"
elif test `echo $name2 | grep solaris`; then
   makefile="Makefile.solaris"
elif test $name1 = apple; then
   makefile="Makefile.maclinux"
   mac=1
elif test `echo $name2 | grep bsd`; then
   makefile="Makefile.linux"
else
   makefile="Makefile.maclinux"
   mac=1
fi

ARCHS=""

if test $forcearch; then
   mac=0
   
   if test $forcearch = "auto"; then
      ARCHS=""
   else
      ARCHS=" -arch $forcearch"
   fi
fi

if test $mac = 1; then
if `lipo $libfile -verify_arch i386`; then
ARCHS=$ARCHS" -arch i386"
fi
if `lipo $libfile -verify_arch x86_64`; then
ARCHS=$ARCHS" -arch x86_64"
fi
if `lipo $libfile -verify_arch ppc`; then
ARCHS=$ARCHS" -arch ppc"
fi
if `lipo $libfile -verify_arch ppc7400`; then
ARCHS=$ARCHS" -arch ppc7400"
fi
if `lipo $libfile -verify_arch ppc64`; then
ARCHS=$ARCHS" -arch ppc64"
fi
fi


make -f $makefile clean
sh devmake.sh
env ARCHS="$ARCHS" \
make -f $makefile
exit $?
