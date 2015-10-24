
#include "Lockable.h"

#include "Defines.h"

#ifdef GF_OS_WIN32
   #include <windows.h>
#else
   #include <string.h>
   #include <time.h>
#endif

#include <cstdio>

#ifndef CLOCK_REALTIME
   #define rtmin(a,b)            (((a) < (b)) ? (a) : (b))
   #define rtmax(a,b)            (((a) > (b)) ? (a) : (b))

	#ifndef GF_CP_MVC
		#include <sys/time.h>
	#endif
#endif


unsigned long Groundfloor::getCurrentThreadID() {
   #ifdef GF_OS_WIN32
   return reinterpret_cast<unsigned long>( ::GetCurrentThread() );
   #else
      #ifdef GF_OS_BSD
         return reinterpret_cast<unsigned long>( pthread_self() );
      #else
         return pthread_self();
      #endif
   #endif
}

/*
WIN32
-----

HANDLE WINAPI CreateMutex( LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName );
BOOL CloseHandle( HANDLE hObject );
BOOL WINAPI ReleaseMutex( HANDLE hMutex );
DWORD WINAPI WaitForSingleObject( HANDLE hHandle, DWORD dwMilliseconds );

//------------------------------------------------------------------------------

LINUX
-----

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int pthread_mutex_init (pthread_mutex_t *mutex, pthread_mutexattr_t *attr);
int pthread_mutex_destroy (pthread_mutex_t *mutex);
int pthread_mutex_lock (pthread_mutex_t *mutex);
int pthread_mutex_trylock (pthread_mutex_t *mutex);
int pthread_mutex_unlock (pthread_mutex_t *mutex);
*/

Groundfloor::Lockable::Lockable() : Groundfloor::Freeable() {
   #ifdef GF_OS_WIN32
      hndMutex = NULL;
      hndMutex = CreateMutex( NULL, false, NULL );

      if ( hndMutex == NULL ) {
         printf( "CreateMutex error: %d\n", GetLastError() );
      }
   #else
      memset( &hndMutex, 0, sizeof(pthread_mutex_t) );
      memset( &pAttr, 0, sizeof(pthread_mutexattr_t) );

      if ( pthread_mutexattr_init( &pAttr ) ) {
         printf( "MutexAttrInit error\n" );
      }

      pthread_mutexattr_settype( &pAttr, PTHREAD_MUTEX_RECURSIVE );

      if ( pthread_mutex_init( &hndMutex, &pAttr ) ) {
         printf( "MutexInit error\n" );
      }

		#ifndef CLOCK_REALTIME
			iFunctionTime = 0;

			struct timeval start, stop;
			gettimeofday( &start, NULL );
			pthread_mutex_trylock( &hndMutex );
			if ( iFunctionTime >= 0) {
				iFunctionTime = 1;
			}
			pthread_mutex_unlock( &hndMutex );
			gettimeofday( &stop, NULL );

			iFunctionTime--;
			iFunctionTime += (stop.tv_sec - start.tv_sec) * 1000 + ((stop.tv_usec - start.tv_usec) / 1000);
			iFunctionTime = rtmax( 0, iFunctionTime );
			iFunctionTime = rtmin( 1000, iFunctionTime );
		#endif
   #endif

   iNestingLevel = 0;
}

Groundfloor::Lockable::~Lockable() {
   #ifdef GF_OS_WIN32
      CloseHandle( hndMutex );
   #else
      pthread_mutex_destroy( &hndMutex );
      pthread_mutexattr_destroy( &pAttr );
   #endif
}

bool Groundfloor::Lockable::lock() {
   return lockWhenAvailable( GFLOCK_NOWAIT );
}

bool Groundfloor::Lockable::lockWhenAvailable( int iTimeout ) {
   #ifdef GF_OS_WIN32
      bool b = ( WaitForSingleObject( hndMutex, iTimeout ) == 0 );
      if ( b ) {
         iNestingLevel++;
      }
      return b;
   #else
      bool b = false;

      if ( iTimeout == GFLOCK_NOWAIT ) {
         b = ( !pthread_mutex_trylock( &hndMutex ) );
         if ( b ) {
            iNestingLevel++;
         }
         return b;
      }

      if ( iTimeout == GFLOCK_INFINITEWAIT ) {
         b = ( !pthread_mutex_lock( &hndMutex ) );
         if ( b ) {
            iNestingLevel++;
         }
         return b;
      }


      struct timespec lockTimeout;

      #if defined(CLOCK_REALTIME)
         clock_gettime( CLOCK_REALTIME, &lockTimeout );

         long iSec      = (iTimeout / 1000);
         long iNanoSec  = (iTimeout - (iSec * 1000)) * 1000000;

         lockTimeout.tv_sec   += iSec;
         lockTimeout.tv_nsec  += iNanoSec;
         if ( lockTimeout.tv_nsec > 1000000000 ) {
            iSec++;
            lockTimeout.tv_nsec -= 1000000000;
         }

         b = ( !pthread_mutex_timedlock( &hndMutex, &lockTimeout ) );
         if ( b ) {
            iNestingLevel++;
         }
         return b;

      #else
			int icur = iFunctionTime;

			while ( pthread_mutex_trylock( &hndMutex ) != 0 ) {
				if ( icur >= iTimeout ) {
					return false;
				}
				GFMillisleep(1);
				icur++;
			}

			iNestingLevel++;
			return true;
      #endif

      // with wait

   #endif
}

bool Groundfloor::Lockable::unlock() {
   #ifdef GF_OS_WIN32
      ReleaseMutex( hndMutex );
   #else
      pthread_mutex_unlock( &hndMutex );
   #endif
   iNestingLevel--;

   return true;
}


Groundfloor::LockableScope::LockableScope(Lockable *lock) {
   this->lock = lock;
   this->lock->lockWhenAvailable(GFLOCK_INFINITEWAIT);
}

Groundfloor::LockableScope::~LockableScope() {
   this->lock->unlock();
}
