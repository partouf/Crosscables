//---------------------------------------------------------------------------

#ifndef GFStringLookupH
#define GFStringLookupH
//---------------------------------------------------------------------------

#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"
#include "../Molecules/Vector.h"

namespace Groundfloor {

   class SimpleStringLookupObject : public Freeable {
   public:
      SimpleStringLookupObject(const char *sName, Freeable *anObject);
      SimpleStringLookupObject(const String *sName, Freeable *anObject);
      ~SimpleStringLookupObject();

      String name;
      Freeable *object;
   };

   class SimpleStringLookup : public Vector {
   public:
      SimpleStringLookup();
      ~SimpleStringLookup();

      void addObjectByString(const char *sName, Freeable *anObject);
      Freeable *getObjectByString(const String *sName, bool bCaseSensitive = true);
      Freeable *getObjectByString(const char *sName, bool bCaseSensitive = true);
      void removeObjectByString(const char *sName);

      void insertObjectSortedByLength(const char *sName, Freeable *anObject, bool bAscending = true);
      void insertObjectSortedByLength(const String *sName, Freeable *anObject, bool bAscending = true);

	  void deleteAndClear();
   };

}

//---------------------------------------------------------------------------
#endif
