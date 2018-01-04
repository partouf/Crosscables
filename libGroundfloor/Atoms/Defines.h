
#ifndef __GFDEFINES_H
#define __GFDEFINES_H

#include <cstddef>

#if defined(__linux) || defined(macintosh) || (defined(__MACH__) && defined(__APPLE__)) || defined(__FreeBSD__) || defined(__FreeBSD) || (defined (__SVR4) && defined (__sun))
#define GF_OS_LINUX
#define GF_CP_GCC

#if defined(macintosh) || (defined(__MACH__) && defined(__APPLE__))
#define GF_OS_MAC
#define GF_OS_BSD
#endif
#if defined(__FreeBSD__) || defined(__FreeBSD)
#define GF_OS_BSD
#endif
#if (defined (__SVR4) && defined (__sun))
#define GF_OS_SOLARIS
#endif
#else
#ifdef _WIN32
#define GF_OS_WIN32

#if defined(_MSC_VER)
#define GF_CP_MVC
#endif

#ifdef __MINGW32__
#define GF_CP_MINGW
#endif
#else
#define GF_OS_UNKNOWN
#define GF_CP_UNKNOWN
#endif
#endif


#ifdef GF_OS_LINUX
#include <unistd.h>
#define GFMillisleep(a) usleep(a*1000)

	typedef long long __int64;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
#else
	// we'll never want winsock1
#define _WINSOCKAPI_
#include <windows.h>
#define GFMillisleep(a) Sleep(a)
#endif

#include <cstdio>


#define MILLISLEEP(a) DoNotUseThisFunction(a)

#define GFWIN32NEXTLINE {13,10,0}
#define GFLINUXNEXTLINE {10,0}
#define GFMACOSNEXTLINE {10,0}

#ifdef GF_OS_WIN32
#define GFNATIVENEXTLINE GFWIN32NEXTLINE
#else
#ifdef GF_OS_MAC
#define GFNATIVENEXTLINE GFMACOSNEXTLINE
#else
#define GFNATIVENEXTLINE GFLINUXNEXTLINE
#endif
#endif


	const char S_ANSI_WIN32_SLASH[] = "\\";
	const char S_ANSI_LINUX_SLASH[] = "/";

	const wchar_t S_WIDE_WIN32_SLASH[] = L"\\";
	const wchar_t S_WIDE_LINUX_SLASH[] = L"/";

#ifdef GF_OS_WIN32
#define S_ANSI_NATIVE_SLASH S_ANSI_WIN32_SLASH
#define S_WIDE_NATIVE_SLASH S_WIDE_WIN32_SLASH
#else
#define S_ANSI_NATIVE_SLASH S_ANSI_LINUX_SLASH
#define S_WIDE_NATIVE_SLASH S_WIDE_LINUX_SLASH
#endif

	//#define GFTRACING

#ifdef GFTRACING
#define GFTRACE_ASSERT(b) if ( b ) { printf( "%d:<%s>:%s(ASSERT Failed)\n", __LINE__, __FILE__, __FUNCTION__ ); }
#define GFTRACE() printf( "%d:<%s>:%s\n", __LINE__, __FILE__, __FUNCTION__ )
#define GFTRACE_D(d) printf( "%d:<%s>::%s(%d)\n", __LINE__, __FILE__, __FUNCTION__, d )
#define GFTRACE_SD(s,d) printf( "%d:<%s>::%s(%d)\n", __LINE__, __FILE__, __FUNCTION__, s, d )
#define GFTRACE_X(d) printf( "%d:<%s>::%s(%x)\n", __LINE__, __FILE__, __FUNCTION__, d )
#define GFTRACE_S(s) printf( "%d:<%s>::%s(%s)\n", __LINE__, __FILE__, __FUNCTION__, s )
#else
#define GFTRACE_ASSERT(b) /* b */
#define GFTRACE() /*  */
#define GFTRACE_D(d) /* d */
#define GFTRACE_SD(s,d) /* s d */
#define GFTRACE_X(d) /* d */
#define GFTRACE_S(s) /* s */
#endif

#include "MemFuncs.h"

#endif // __GFDEFINES_H
