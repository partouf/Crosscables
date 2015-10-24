@echo off

mkdir ..\include\Chalkboard
mkdir ..\include\Chalkboard\Win32
mkdir ..\include\Chalkboard\GTK
mkdir ..\include\Chalkboard\Cocoa

copy *.h ..\include\Chalkboard
copy Win32\*.h ..\include\Chalkboard\Win32
copy GTK\*.h ..\include\Chalkboard\GTK
copy Cocoa\*.h ..\include\Chalkboard\Cocoa
