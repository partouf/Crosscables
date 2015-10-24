# Crosscables

A collection of cross platform libraries aimed to ease development.

The libraries are based on C++98 and was primarily developed in the years 2008-2011, but also in later years.

The suite remained on a private Subversion repository for a very long time and really only benefiting my own primarily "written on Windows, but running on Linux" - applications. Since C++ is very much growing as a language, I decided to release as an open source library for whoever wanted to learn from it.

The libraries were intended to be used modular as static libraries linked only when needed.

Note: Before uploading it to Github, I decided to rename most of the classes, filenames and just use namespaces, so it might be that some things are broken despite being able to compile. Also so far only the Win32 parts have been tested, the Makefiles still needed to be edited to use the new filenames and directory structure.


Currently included:
 * libGroundfloor - a base collection of things to help the other libraries (strings, encoding, containers, threading)
 * libJumpropes - a Networking library
 * libJumpknots - an SSL extension to libJumpropes (Requires OpenSSL)
 * libMySQLBooks - a MySQL database connectivity library (Requires the MySQL C API)
 
Included, but not yet uploaded:
 * libPostgreSQLBooks - PostgreSQL, Not yet uploaded
 * libChalkboard - GUI library for all platforms using native Windows, Cocoa and GTK controls

 
Supported/tested platforms:
 * Win32 (compiler VS2015)
 
Supported/tested, but Makefiles not yet fixed before uploading to GitHub:
 * Linux
 * MacOSX
 * FreeBSD
 * Solaris (no GUI)

