
#include <cstdio>
#include "GarbageCollector.h"

#include "../Materials/Functions.h"
#include <cstring>

// global var
static Groundfloor::GarbageCollector *GFGlobalVar_GFGarbageCollector = 0;

Groundfloor::GarbageStamp::GarbageStamp( Groundfloor::Freeable *anObject, __int64 iTimeToLive ) {
   this->anObject       = anObject;
   this->iCreationTime  = GetTimestamp();
   if ( iTimeToLive == GC_LIVEFOREVER ) {
      this->iDeletionTime  = GC_LIVEFOREVER;
   } else {
      this->iDeletionTime  = this->iCreationTime + iTimeToLive;
   }
}

Groundfloor::GarbageStamp::~GarbageStamp() {
   if ( this->anObject != 0 ) {
      delete anObject;
   }
}

__int64 Groundfloor::GarbageStamp::getCreationTime() {
   return this->iCreationTime;
}

__int64 Groundfloor::GarbageStamp::getDeletionTime() {
   return this->iDeletionTime;
}

// ----------------------------------------------------------------------------

Groundfloor::GarbageCollector::GarbageCollector() {
   iObjectsAdded     = 0;
   iObjectsDeleted   = 0;

   aGarbageCollection.autoClear = true;

   this->iDefaultTimeToLive = 60;   // a minute
   this->setInterval( 5000 );       // 5 second interval, we don't wanna be too much trouble
}

Groundfloor::GarbageCollector::~GarbageCollector() {
   stopAndWait();
}

void Groundfloor::GarbageCollector::setDefaultTimeToLive( unsigned int iDefaultTimeToLive ) {
   this->iDefaultTimeToLive = iDefaultTimeToLive;
}

void Groundfloor::GarbageCollector::addToQueue( Groundfloor::Freeable *anObject, unsigned int iTimeToLive ) {
   Groundfloor::GarbageStamp *stampedObject = 0;

   if ( iTimeToLive != 0 ) {
      stampedObject = new Groundfloor::GarbageStamp( anObject, iTimeToLive );
   } else {
      stampedObject = new Groundfloor::GarbageStamp( anObject, iDefaultTimeToLive );
   }

   aGarbageCollection.addElement( stampedObject );

   iObjectsAdded++;   
}

void Groundfloor::GarbageCollector::execute() {
   unsigned int iDeleted = 0;

   unsigned int c = aGarbageCollection.size();

   for ( unsigned int i = 0; i < c; i++ ) {
      Groundfloor::GarbageStamp *aStamp = static_cast<Groundfloor::GarbageStamp *>( aGarbageCollection.elementAt( i ) );

      if ( aStamp != 0 ) {
         if ( aStamp->getDeletionTime() != GC_LIVEFOREVER ) {
            if ( GetTimestamp() >= aStamp->getDeletionTime() ) {
               delete aGarbageCollection.removeElement( i );

               iDeleted++;
            }
         }
      }

      GFMillisleep( 10 );
   }

   if ( iDeleted > 0 ) {
      aGarbageCollection.compress();
   }

   iObjectsDeleted += iDeleted;
}

unsigned int Groundfloor::GarbageCollector::getItemsAdded() {
   return iObjectsAdded;
}

unsigned int Groundfloor::GarbageCollector::getItemsDeleted() {
   return iObjectsDeleted;
}


// ----------------------------------------------------------------------------

void GFGarbage_InitialisationError() {
   printf( "Warning: Garbage Collector not yet initialized!\n" );
}

bool Groundfloor::initGlobalGarbageCollector( unsigned int iInterval ) {
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      Groundfloor::finiGlobalGarbageCollector();
   }

   GFGlobalVar_GFGarbageCollector = new Groundfloor::GarbageCollector();

   GFGlobalVar_GFGarbageCollector->setDefaultTimeToLive( 60 );
   GFGlobalVar_GFGarbageCollector->setInterval( iInterval );

   GFGlobalVar_GFGarbageCollector->start();
   
   return true;
}

void Groundfloor::finiGlobalGarbageCollector() {
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      delete GFGlobalVar_GFGarbageCollector;
   } else {
      GFGarbage_InitialisationError();
   }
}

Groundfloor::Freeable *Groundfloor::Disposable( Groundfloor::Freeable *object, unsigned int iTTL ) {
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      GFGlobalVar_GFGarbageCollector->addToQueue( object, iTTL );
   } else {
      GFGarbage_InitialisationError();
   }

   return object;
}

Groundfloor::String *Groundfloor::Disposable( Groundfloor::String *object, unsigned int iTTL ) {
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      GFGlobalVar_GFGarbageCollector->addToQueue( object, iTTL );
   } else {
      GFGarbage_InitialisationError();
   }

   return object;
}

Groundfloor::String *Groundfloor::DisposableStr( const char *str, unsigned int iTTL ) {
   Groundfloor::String *object = new Groundfloor::String( str, strlen(str) );
   
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      GFGlobalVar_GFGarbageCollector->addToQueue( object, iTTL );
   } else {
      GFGarbage_InitialisationError();
   }

   return object;
}

Groundfloor::BValue *Groundfloor::DisposableStrValue( const char *str, unsigned int iTTL ) {
   Groundfloor::BValue *object = new Groundfloor::BValue();
   object->setString( str );

   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      GFGlobalVar_GFGarbageCollector->addToQueue( object, iTTL );
   } else {
      GFGarbage_InitialisationError();
   }

   return object;
}

unsigned int Groundfloor::GarbageStats_itemsAdded() {
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      return GFGlobalVar_GFGarbageCollector->getItemsAdded();
   } else {
      GFGarbage_InitialisationError();
   }

   return 0;
}

unsigned int Groundfloor::GarbageStats_itemsDeleted() {
   if ( GFGlobalVar_GFGarbageCollector != 0 ) {
      return GFGlobalVar_GFGarbageCollector->getItemsDeleted();
   } else {
      GFGarbage_InitialisationError();
   }

   return 0;
}

