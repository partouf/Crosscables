
#include "Thread.h"
#include "../Atoms/Defines.h"

#ifdef GF_OS_LINUX
    #include <pthread.h>
    #include <unistd.h>

    #define THREADRETURN void *
#else
    #include <windows.h>

    #define THREADRETURN DWORD
#endif



// non-member function to be spawned as a thread by the OS, executes our own thread-loop
THREADRETURN runExecute( void *pParam ) {
   static_cast<Groundfloor::Thread *>(pParam)->executionloop();

   return 0;
}

// Groundfloor::Thread functions
Groundfloor::Thread::Thread() {
   bStartedExecutionMethod = false;
   bRunning             = false;
   bShouldTerminate     = false;

   bThreadIsCreated     = false;
   bExceptionOccurred   = false;

   iDefaultSleepValue   = 10;

   setInterval( 1 );
}

Groundfloor::Thread::~Thread() {
   stopAndWait();

   cleanup();
}

void Groundfloor::Thread::cleanup() {
   if ( bThreadIsCreated ) {
   #ifdef GF_OS_WIN32
      CloseHandle( pHandle );
   #else
      pthread_detach( aPThreadStruct );
   #endif
   }

   bThreadIsCreated = false;
}


void Groundfloor::Thread::start() {
   // do nothing if we're already running
   if ( !bRunning ) {

      bStartedExecutionMethod = false;
      bShouldTerminate     = false;
      bExceptionOccurred   = false;

      // free any previous thread handles
      cleanup();

      #ifdef GF_OS_WIN32
         // Windows
         // WINBASEAPI HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES,DWORD,LPTHREAD_START_ROUTINE,PVOID,DWORD,PDWORD);
         pHandle = CreateThread( NULL, 0, (THREADRETURN (__stdcall *)(void *))(runExecute), static_cast<void *>(this), 0, NULL );
      #else
         // Linux PThread
         // extern int pthread_create (pthread_t *__restrict __newthread,__const pthread_attr_t *__restrict __attr,void *(*__start_routine) (void *),void *__restrict __arg) __THROW __nonnull ((1, 3));
         pHandle = reinterpret_cast<void *>( pthread_create( &aPThreadStruct, NULL, runExecute, static_cast<void *>(this) ) );
      #endif

      bThreadIsCreated = true;

      // wait till the thread-loop is really running
      while ( !bStartedExecutionMethod ) {
         GFMillisleep( iDefaultSleepValue );
      }

   }
}

void Groundfloor::Thread::stop() {
   // flag thread for termination
   bShouldTerminate = true;
}

void Groundfloor::Thread::stopAndWait() {
   stop();

   // wait till the thread-loop is at a full halt
   while ( bRunning ) {
      GFMillisleep( iCurrentInterval );
   }
}

bool Groundfloor::Thread::isRunning() {
   return bRunning;
}

void Groundfloor::Thread::executionloop() {
   bRunning = true;
   bStartedExecutionMethod = true;

   try {

      while ( !bShouldTerminate ) {
         execute();

         if ( !bShouldTerminate ) {
            GFMillisleep( iCurrentInterval );
         }
      }

   } catch ( ... ) {
      // the try-catch makes sure we catch any errors
      //  and just stop the thread
      //  instead of raising it to the OS handling of this thread
      bExceptionOccurred = true;
   }

   bRunning = false;
}

void Groundfloor::Thread::setInterval( unsigned int iInterval ) {
   iCurrentInterval = iInterval;
}

void Groundfloor::Thread::waitWhileRunning() {
    while ( bRunning ) {
        GFMillisleep( iDefaultSleepValue );
    }
}

void Groundfloor::Thread::join( Groundfloor::Thread *anOtherThread ) {
    while ( bRunning || anOtherThread->bRunning ) {
        GFMillisleep( iDefaultSleepValue );
    }
}
