#!/bin/sh

DESTDIR=../include/Groundfloor

mkdir -p $DESTDIR
    
DIR1=Atoms
DIR2=Molecules
DIR3=Materials
DIR4=Bookshelfs

mkdir -p $DESTDIR/$DIR1
mkdir -p $DESTDIR/$DIR2
mkdir -p $DESTDIR/$DIR3
mkdir -p $DESTDIR/$DIR4

cp $DIR1/*.h $DESTDIR/$DIR1
cp $DIR2/*.h $DESTDIR/$DIR2
cp $DIR3/*.h $DESTDIR/$DIR3
cp $DIR4/*.h $DESTDIR/$DIR4

#doxygen Groundfloor.Doxyfile

