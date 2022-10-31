
#ifndef __GFBAttribute_H
#define __GFBAttribute_H

namespace Groundfloor {
   class FBAttribute;
}

#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"
#include "BValue.h"

namespace Groundfloor {
   /// attribute as used in BNode, following a key=value format.
   class BAttribute : public Freeable {
   public:
      BAttribute();
      BAttribute(const BAttribute *attrib);
      BAttribute(const char *sKey);
      BAttribute(const char *sKey, const char *sValue);
      BAttribute(const char *sKey, const String *sValue);
      BAttribute(const char *sKey, bool bValue);
      BAttribute(const char *sKey, long iValue);
      BAttribute(const char *sKey, double iValue, short iDecimal);
      ~BAttribute();

      String key;
      BValue value;
      bool hideFromXML;
   };
}

#endif // __GFBAttribute_H
