
Win32
-----

Als je gebruik maakt van Windows Vista, installeer dingen Niet op C:, aangezien
daar het rechtensysteem altijd word toegepast.

prereq: Installeer MingW-utils in Dev-Cpp.

MySQL
-----
1. Download MySQL en installeer
2. cmd
3. d:
4. cd "D:\Program Files (x86)\MySQL\MySQL Server 5.1\lib\opt\"
5. d:\dev-cpp\bin\reimp -d libmysql.lib
6. d:\dev-cpp\bin\dlltool -k -A -C -d libmysql.def -l libmysql.a
7. creeer "D:\Program Files (x86)\MySQL\MySQL Server 5.1\include\mysql"
8. verplaats "D:\Program Files (x86)\MySQL\MySQL Server 5.1\include\*.*" naar include\mysql

Benodigde DLL's in Applicatie-map:
libmysql.dll


PostgreSQL
----------
1. Download PSQL en installeer
2. cmd
3. d:
4. cd "D:\Program Files (x86)\PostgreSQL\8.3\lib"
5. d:\dev-cpp\bin\pexports ..\libpq.dll > libpq.def
6. d:\dev-cpp\bin\dlltool -A -C -d libpq.def -l libpq.a

Benodigde DLL's in Applicatie-map:
libpq.dll
libintl3.dll
libiconv2.dll
libeay32.dll
krb5_32.dll
k5sprt32.dll
gssapi32.dll
comerr32.dll


mac os x - leopard
------------------

sudo sh mysqlinstall.sh
