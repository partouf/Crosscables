
#ifndef __GFLOCKABLE_H
#define __GFLOCKABLE_H

#include "Freeable.h"
#include "Defines.h"

#ifdef GF_OS_LINUX
#include <pthread.h>
#endif

#ifdef GF_OS_WIN32
#define GFLOCK_NOWAIT 0
#define GFLOCK_INFINITEWAIT INFINITE
#else
#define GFLOCK_NOWAIT 0
#define GFLOCK_INFINITEWAIT -1
#endif

namespace Groundfloor {

	unsigned long getCurrentThreadID();

	/** TGFLockable is a simple wrapper around a native mutex.
	 *  When compiled for the Linux platform, it uses the PThread library.
	 *  The mutex is thread-orientated, so calling lock() functions within the same
	 *  thread will not cause the function to fail the second time.
	 *  If a second thread attempts to lock it, it will fail until it is unlocked by the first thread.
	 */
	class Lockable : public Freeable
	{
	protected:

#ifdef GF_OS_WIN32
		HANDLE hndMutex;
#else
		pthread_mutex_t hndMutex;
		pthread_mutexattr_t pAttr;

#if !defined(CLOCK_REALTIME) || defined(GF_OS_MAC)
		int iFunctionTime;
#endif
#endif
		int iNestingLevel;

	public:
		/// Creates a native mutex without initial ownership.
		Lockable();
		/// Frees the mutex.
		~Lockable();

		/// Tries to obtain ownership over the mutex or just fails. (same as lockWhenAvailable(GFLOCK_NOWAIT))
		bool lock();
		/// Tries to obtain ownership over the mutex, when the specified timeout (in milliseconds) has elapsed or the ownership is obtained, the function exits.
		/// The defines GFLOCK_NOWAIT or GFLOCK_INFINITEWAIT may also be used to specify iTimeout.
		bool lockWhenAvailable(int iTimeout = GFLOCK_INFINITEWAIT);
		/// Releases the mutex for other threads to access.
		bool unlock();
	};

	class LockableScope : public Freeable
	{
	protected:
		Lockable *lock;
	public:
		LockableScope(Lockable *lock);
		~LockableScope();
	};

}

#endif // __GFMUTEXABLE_H
