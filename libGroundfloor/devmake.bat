@echo off

Set DESTDIR=..\include\Groundfloor

mkdir %DESTDIR%

Set DIR1=Atoms
Set DIR2=Molecules
Set DIR3=Materials
Set DIR4=Bookshelfs

mkdir %DESTDIR%\%DIR1%
mkdir %DESTDIR%\%DIR2%
mkdir %DESTDIR%\%DIR3%
mkdir %DESTDIR%\%DIR4%

copy %DIR1%\*.h %DESTDIR%\%DIR1%
copy %DIR2%\*.h %DESTDIR%\%DIR2%
copy %DIR3%\*.h %DESTDIR%\%DIR3%
copy %DIR4%\*.h %DESTDIR%\%DIR4%

REM PATH=%PATH%;"D:\Program Files (x86)\doxygen\bin"
REM doxygen Groundfloor.Doxyfile
