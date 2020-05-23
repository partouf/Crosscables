# Crosscables

A collection of cross platform libraries aimed to ease development.

The libraries are based on C++98 and was primarily developed in the years 2008-2011, but also in later years.

The suite remained on a private Subversion repository for a very long time and really only benefiting my own primarily "written on Windows, but running on Linux" - applications. Since C++ is very much growing as a language, I decided to release as an open source library for whoever wanted to learn from it.

The libraries were intended to be used modular as static libraries linked only when needed.


Currently included:
 * libGroundfloor - a base collection of things to help the other libraries (strings, encoding, containers, threading)
 * libJumpropes - a Networking library
 * libJumpknots - an SSL extension to libJumpropes (Requires OpenSSL)
 * libMySQLBooks - a MySQL database connectivity library (Requires the MySQL C API)
 * libChalkboard - GUI library for all platforms using native Windows and GTK controls (MacOSX Cocoa bindings are broken)

Included, but not yet uploaded:
 * libPostgreSQLBooks - PostgreSQL, Not yet uploaded

Supported/tested platforms:
 * Win32 (compiler VS2015-2019)
 * MacOSX Clang
 * Linux [![Build Status](https://img.shields.io/endpoint.svg?url=https%3A%2F%2Factions-badge.atrox.dev%2Fpartouf%2FCrosscables%2Fbadge&style=flat)](https://actions-badge.atrox.dev/partouf/Crosscables/goto)
 
Supported/tested:
 * MacOSX (only libGroundfloor and libJumpropes will build)
 * FreeBSD
 * Solaris (no GUI)

