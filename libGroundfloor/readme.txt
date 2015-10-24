
Groundfloor
-----------


Project is afhankelijk van basis C++ libraries.
Voor Threads in Linux is -lpthread nodig, en optioneel -lrt.

BUILD
-----
sh build.sh


Linking problemen
-----------------

Indien een andere static library naar Groundfloor verwijst moet
-lGroundfloor aan het eind van de linking compile regel staan.
