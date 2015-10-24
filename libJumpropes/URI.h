
#ifndef __JRURI_H
#define __JRURI_H

#include <Groundfloor/Atoms/Defines.h>

#include <Groundfloor/Atoms/Freeable.h>
#include <Groundfloor/Molecules/String.h>
#include <Groundfloor/Molecules/Property.h>

#include "NetAddress.h"
#include "LookupCache.h"

using namespace Groundfloor;

namespace Jumpropes {

   class URI : public Freeable {
   protected:
      LookupBase *pLookupBase;
      void useDefaultPort();
   public:
      URI(LookupBase *pLookupBase);
      ~URI();

      void parseUri(const String *sUri, bool bClearVars = true);
      void parseUri(const char *sUri, bool bClearVars = true);
      String *getUri();

      String protocol;
      LookupObject host;
      Property<int> port;
      String path;
      String query;

      String user;
      String pass;
   };

}

#endif // __JRURI_H
