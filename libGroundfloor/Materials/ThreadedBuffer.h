
#ifndef _GFThreadedBuffer_H
#define _GFThreadedBuffer_H

#include "Thread.h"
#include "FifoVector.h"

namespace Groundfloor {

   /** TGFThreadedBuffer implements a Thread that will process objects one-by-one
    *  which are stored into a GFFifoVector.
    */
   class ThreadedBuffer : public Thread {
   protected:
      Lockable lockBuffer;
      FifoVector aBuffer;
      bool bDeleteAfterProcess;
      bool bStopWhenEmpty;
   public:
      ThreadedBuffer();
      ~ThreadedBuffer();

      /// To add a new object to the vector that should be processed.
      void add(Freeable *obj);
      /// Thread execute function, this is internally used and should not be called.
      void execute();

      /// Checks whether or not the Fifovector is empty.
      bool isEmpty();
      /// Checks whether or not the Fifovector is full.
      /** If the fifovector is full, it is wise to stop adding objects or start processing objects - in order to prevent a deadlock. */
      bool isFull();

      /// If you want the execute function to delete the object that was processed succesfully, set this to true. However it is recommended you delete the object yourself.
      void setDeleteAfterProcess(bool bDelete);

      /// By Default this is fault, set it to true when you have filled the buffer so the thread will stop when the buffer is emptied
      /** Use this if you want to be able to use thread functions like join() or others without having to check if the buffer is empty yourself */
      void setStopWhenEmpty(bool bSet);

      /// Function to be implemented. It is recommended to always return True.
      virtual bool processObject(Freeable *obj) = 0;
   };


}

#endif // _GFThreadedSender_H

