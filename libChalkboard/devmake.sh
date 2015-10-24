#!/bin/sh

mkdir -p ../include/Chalkboard
cp *.h ../include/Chalkboard

mkdir -p ../include/Chalkboard/GTK
cp GTK/*.h ../include/Chalkboard/GTK

mkdir -p ../include/Chalkboard/Cocoa
cp Cocoa/*.h ../include/Chalkboard/Cocoa

#doxygen Chalkboard.Doxyfile

