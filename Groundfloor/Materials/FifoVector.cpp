
#include "FifoVector.h"

#include <cstdio>


void Groundfloor::FifoVector::init() {
   this->autoClear = false;
   bWaitingForSpace = false;
   
   iCurrent    = -1;
   iLastAdded  = -1;
}

Groundfloor::FifoVector::FifoVector() : Groundfloor::Vector() {
   init();
}

Groundfloor::FifoVector::FifoVector( unsigned int iPrealloc ) : Groundfloor::Vector( iPrealloc ) {
   init();
} 

Groundfloor::FifoVector::~FifoVector() {
}

bool Groundfloor::FifoVector::isEmpty() {
   return ((iCurrent == -1) && (iLastAdded == -1));
}

bool Groundfloor::FifoVector::isFull() {
   return ( (iLastAdded + 1) == iCurrent );
}

void Groundfloor::FifoVector::push( Groundfloor::Freeable *obj ) {
   aWriteLock.lockWhenAvailable( GFLOCK_INFINITEWAIT );
	aReadLock.lockWhenAvailable( GFLOCK_INFINITEWAIT );
   try {
      if ( iLastAdded >= iCurrent ) {

         if ( (iCurrent > 0) && (iLastAdded == this->allocated() ) ) {
            this->replaceElement( 0, obj );
            iLastAdded = 0;
         } else if (iLastAdded + 1  < this->allocated()) {
            this->insertAt( iLastAdded + 1, obj );
            iLastAdded++;
         } else {
            this->addElement(obj);
            iLastAdded++;
         }
   
      } else { // iLastAdded < iCurrent
         while ( (iLastAdded + 1) == iCurrent ) {
				aReadLock.unlock();
				GFMillisleep(10);
				aReadLock.lockWhenAvailable( GFLOCK_INFINITEWAIT );
         }
         
         this->replaceElement( iLastAdded + 1, obj );
         iLastAdded++;
      }
   } catch ( ... ) {
   }
	aReadLock.unlock();
   aWriteLock.unlock();
}

Groundfloor::Freeable *Groundfloor::FifoVector::pop() {
   Groundfloor::Freeable *obj = 0;

   Groundfloor::LockableScope read(&aReadLock);
   try {
      if ( iLastAdded != -1 ) {
         obj = this->elementAt( iCurrent + 1 );
         this->replaceElement( iCurrent + 1, 0 );

         iCurrent++;
         
         if ( iCurrent == this->size() ) {
            iCurrent = -1;
         }

         if ( iCurrent == iLastAdded ) {
            iCurrent    = -1;
            iLastAdded  = -1;
         }
      }
   } catch ( ... ) {
   }
   
   return obj;
}


Groundfloor::Freeable *Groundfloor::FifoVector::trypop() {
   Groundfloor::Freeable *obj = NULL;

   Groundfloor::LockableScope read(&aReadLock);

   try {
      if ( iLastAdded != -1 ) {
         obj = this->elementAt( iCurrent + 1 );
      }
   } catch ( ... ) {
   }

   return obj;
}
