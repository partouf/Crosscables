
#ifndef __GFThread_H_
#define __GFThread_H_
/***************************************************************************/

#include "../Atoms/Callable.h"
#include "../Atoms/Defines.h"

#ifdef GF_OS_LINUX
   #include <pthread.h>
#endif

namespace Groundfloor {

   /** Thread creates a new native thread where the execution code is called from.
    *  Add your own code by overriding the execute() function, this function gets
    *  called every given interval during the timespan of the thread.
    *  Warning: do not call stopAndWait() within the execute() running code,
    *   the thread will deadlock on this call.
    */
   class Thread : public Callable {
   protected:
      bool     bRunning;
      bool     bStartedExecutionMethod;
      bool     bShouldTerminate;
      bool     bExceptionOccurred;

#ifdef GF_OS_LINUX
      pthread_t aPThreadStruct;
#endif

      void    *pHandle;
      bool     bThreadIsCreated;

      unsigned int iCurrentInterval;
      unsigned int iDefaultSleepValue;

      void cleanup();

   public:
      /// Constructor. Initializes the interval at 100ms. The thread will not be started yet.
      Thread();
      /// Destructor. Stops the thread and exits.
      ~Thread();

      /// Returns whether or not the executionloop() is still running.
      bool isRunning();

      /// Spawns a new thread to work in.
      void start();
      /// Signals executionloop() to stop looping, and exits.
      /** Do not delete this object right after calling stop(), check for its running state first. Or better: use stopAndWait() */
      void stop();
      /// Signals executionloop() to stop looping, and waits until the executionloop has exited as well.
      void stopAndWait();

      /// Default waiting loop until the thread has stopped, do not run while in execute
      void waitWhileRunning();

      /// Waits for both threads until they've stopped, do not run while in execute
      void join(Thread *anOtherThread);

      /// Sets the interval at which the execute() function is called.
      void setInterval(unsigned int iInterval);

      /// This function must stay public, yet should not be called.
      /** It is the entrypoint of the native thread and will loop every set interval until the thread is stopped. */
      void executionloop();
   };

}

/***************************************************************************/
#endif // __GFThread_H_

