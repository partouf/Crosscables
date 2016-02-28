
#include "ThreadedBuffer.h"

Groundfloor::ThreadedBuffer::ThreadedBuffer() : Groundfloor::Thread() {
   bDeleteAfterProcess = false;
   bStopWhenEmpty = false;
   aBuffer.setPreAlloc(1024);
}

Groundfloor::ThreadedBuffer::~ThreadedBuffer() {
   stop();

   while ( bRunning ) {
      GFMillisleep( 10 );
   }
}

void Groundfloor::ThreadedBuffer::setStopWhenEmpty( bool bSet ) {
    bStopWhenEmpty = bSet;
}

void Groundfloor::ThreadedBuffer::add( Groundfloor::Freeable *obj ) {
   lockBuffer.lockWhenAvailable();
   aBuffer.push( obj );
   lockBuffer.unlock();
}

void Groundfloor::ThreadedBuffer::setDeleteAfterProcess( bool bDelete ) {
   bDeleteAfterProcess = bDelete;
}

void Groundfloor::ThreadedBuffer::execute() {
   lockBuffer.lockWhenAvailable();
   try {
      if ( !aBuffer.isEmpty() ) {
         // in order for this to work properly, no other object should access aBuffer!
         Groundfloor::Freeable *obj = aBuffer.trypop();

         // if your self-implemented function returns false, the object is not removed from the vector
         //  and will be processed Again next time the execute() function is called.
         // yet: if you don't want your program to deadlock at the add() function, it is recommended
         //  to occasionally pop something off of the vector.
         if ( processObject( obj ) ) {
            aBuffer.pop();

            if ( bDeleteAfterProcess ) {
               delete obj;
            }
         }
      }

      if ( bStopWhenEmpty && aBuffer.isEmpty() ) {
          stop();
      }

   } catch ( ... ) {
   }
   lockBuffer.unlock();
}

bool Groundfloor::ThreadedBuffer::isEmpty() {
   return aBuffer.isEmpty();
}

bool Groundfloor::ThreadedBuffer::isFull() {
   return aBuffer.isFull();
}

