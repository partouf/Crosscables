
Groundfloor
-----------

Using Threads on Linux requires -lpthread, and optionally -lrt for higher resolution timing.

BUILD
-----
sh build.sh


Linking
-------

When used in conjunction with the other Crosscables libraries, -lGroundfloor usually needs to sit at the end of the Makefile linking options to make sure the compiler doesn't optimize out things that are used in the other needed libraries.
