#ifndef __GFGarbageCollector_H
#define __GFGarbageCollector_H

#include "../Atoms/Freeable.h"
#include "../Molecules/Vector.h"
#include "../Materials/Thread.h"

#include "../Molecules/String.h"
#include "../Bookshelfs/BValue.h"


#define GC_LIVEFOREVER 4294967295UL

namespace Groundfloor {

   /** GarbageStamp class
    */
   class GarbageStamp : public Freeable {
   protected:
      __int64 iCreationTime;
      __int64 iDeletionTime;
      Freeable *anObject;

   public:
      GarbageStamp(Freeable *anObject, __int64 iTimeToLive);
      ~GarbageStamp();

      __int64 getCreationTime();
      __int64 getDeletionTime();
   };

   /** GarbageCollector class.
     * The GarbageCollector is not a typical kind of garbage collection mechanism,
     *  does not depend on usage or references to the object, but on a predefined TTL (time-to-live).
     * Therefor the garbage collector should not be used on a regular basis, but only in situations
     *  where manually deleting an object is either not possible or too complicated.
     */
   class GarbageCollector : public Thread {
   protected:
      Vector aGarbageCollection;
      unsigned int iDefaultTimeToLive;

      unsigned int iObjectsAdded;
      unsigned int iObjectsDeleted;

   public:
      /// initializes vector, variables and sets the default time-to-live to 5 seconds
      GarbageCollector();
      /// automatically deletes all objects
      ~GarbageCollector();

      /// adds a new GarbageStamp with given object to the vector.
      /// If given iTimeToLive is set to GC_LIVEFOREVER, the object will be kept
      ///  alive untill the garbagecollector is deleted.
      void addToQueue(Freeable *anObject, unsigned int iTimeToLive = 0);

      /// the default ttl that is used when no custom ttl is given
      void setDefaultTimeToLive(unsigned int iDefaultTimeToLive);

      unsigned int getItemsAdded();
      unsigned int getItemsDeleted();

      /// implementation of thread-loop, disposing of garbage when timers have expired
      void execute();
   };


   /// instantiates the primary GarbageCollector object
   bool initGlobalGarbageCollector(unsigned int iInterval = 5000);
   /// deletes the garbagecollector, freeing all remaining garbage
   void finiGlobalGarbageCollector();

   /// returns the number of objects added to the garbagecollector
   unsigned int GarbageStats_itemsAdded();
   /// returns the number of objects that have been disposed
   unsigned int GarbageStats_itemsDeleted();


   /// adds the given object to the garbagecollector, return the same object
   Freeable *Disposable(Freeable *object, unsigned int iTTL = 0);
   /// adds the given String-object to the garbagecollector, return the same object
   String *Disposable(String *object, unsigned int iTTL = 0);

   /// creates a new String from given str, adds it to the garbagecollector, and returns the object
   String *DisposableStr(const char *str, unsigned int iTTL = 0);

   /// creates a new BValue from given str, adds it to the garbagecollector, and returns the object
   BValue *DisposableStrValue(const char *str, unsigned int iTTL = 0);

}

#endif // __GFGarbageCollector_H

