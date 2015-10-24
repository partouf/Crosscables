
#ifndef __JRLookupCache_H
#define __JRLookupCache_H

namespace Jumpropes {
   class LookupBase;
   class LookupCache;
}

#include "Defines.h"
#include "Functions.h"
#include "NetAddress.h"
#include <Groundfloor/Atoms/Lockable.h>

using namespace Groundfloor;

namespace Jumpropes {

   class LookupBase : public Freeable {
   protected:
      String sNonValidHostName;

      virtual const char* classname() { return "LookupBase"; }
   public:
      LookupBase();
      ~LookupBase();

      virtual LookupObject *lookupHost(const String *sHost);
      virtual bool lookupHost(LookupObject *obj);

      virtual bool equalsNonValidHostResponse(const LookupObject *obj);
      virtual LookupObject *getNonValidHostResponseObject();
   };

   class LookupCache : public LookupBase {
   protected:
      Lockable lock;
      Vector list;
      LookupObject *findObj(const String *sHost);
   public:
      LookupCache(bool bInitNonvalidhostAdress = true);
      ~LookupCache();

      void clear(bool bInitNonvalidhostAdress = true);

      LookupObject *lookupHost(const String *sHost);
      bool lookupHost(LookupObject *obj);

      void addObject(const LookupObject *obj);
   };

}

#endif // __JRLookupCache_H
