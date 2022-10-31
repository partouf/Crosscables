
#ifndef __RemoteSquirrel_H
#define __RemoteSquirrel_H

#include "AbstractSquirrel.h"
#include "../Materials/StringLookup.h"
#include "BValue.h"

namespace Groundfloor {
   /// specialized Squirrel implementation for Remote Databases
   class RemoteSquirrelConnection : public AbstractSquirrelConnection {
   public:
      RemoteSquirrelConnection();
      ~RemoteSquirrelConnection();

      /// hostname (implementation should automatically resolve this into an IP if needed)
      StringProperty host;
      /// port to connect to on the host
      Property<int> port;
      /// username to use when logging in to the server
      StringProperty username;
      /// password to use
      StringProperty password;
   };

   /// for Local Databases
   class LocalSquirrelConnection : public AbstractSquirrelConnection {
   public:
      LocalSquirrelConnection();
      ~LocalSquirrelConnection();

      StringProperty path;
   };


   /// specialized Squirrel implementation for Remote Databases
   class RemoteSQL : public AbstractSquirrel {
   protected:
      SimpleStringLookup params;
      String sCurrentQuery;

      virtual void prepareQuery();

      /// replaces occurences of sToken (parameters) with the given value
      /** depending on the lastsettype of the value the function should escape the value according to the specific database's rules, setting bIncludeQuotation to false is not recommended, as it could negate portability */
      virtual void replaceInQuery(const char *sToken, BValue *aValue, bool bIncludeQuotations = true) = 0;
   public:
      RemoteSQL(RemoteSquirrelConnection *pConnection);
      ~RemoteSQL();

      /// sets the current query to sQuery
      virtual void setQuery(const String *sQuery, bool bReplaceBackticksWithDoubleQuotes = true);

      /// returns ptr to current query
      virtual String *getCurrentQuery();

      virtual BValue *findOrAddParam(const char *sToken);
      virtual BValue *findOrAddParam(const String *sName);

      /// return (if possible) the last id that was inserted or updated by use of an autonumbering type or sequence
      /** this function should only return the id that was used last in this exact session/connection */
      virtual __int64 getLastID(const char *sTableName, SquirrelReturnData *errData = NULL) = 0;
   };
}

#endif // __RemoteSquirrel_H

