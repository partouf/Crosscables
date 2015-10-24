
#ifndef __MySQLSquirrel_H
#define __MySQLSquirrel_H


#include <Groundfloor/Bookshelfs/RemoteSquirrel.h>
#include <Groundfloor/Bookshelfs/BTable.h>
#include <Groundfloor/Molecules/String.h>

#include <Groundfloor/Atoms/Defines.h>

#ifdef GF_OS_WIN32
   #include <winsock2.h>
#endif

#include <mysql/mysql.h>
#include <mysql/errmsg.h>


bool initMySQLBooks();
bool finiMySQLBooks();

using namespace Groundfloor;

class TMySQLSquirrelConnection: public RemoteSquirrelConnection {
   protected:
      MYSQL aMysqlObj;
      MYSQL *pMysqlObj;

      bool bCompression;

      void debug( const char *sMsg );

   public:
      StringProperty LastError;

      TMySQLSquirrelConnection();
      ~TMySQLSquirrelConnection();

      /// Select database to use. No default.
      void selectDatabase( const char *sDatabase );

      bool connect();
      bool disconnect();

      MYSQL *getMySQLObj();
};

class TMySQLSquirrel: public RemoteSQL {
   protected:
      MYSQL_RES *pCurrentResult;
      MYSQL_ROW aCurrentRow;

      String sCurrentQuery;
      BFields aCurrentFields;

      __int64 iCurrentRecord;
      __int64 iRecordCount;
      bool bAnsiMode;

      void retreiveFields( BFields *aFields );

      void replaceInQuery( const char *sToken, BValue *aValue, bool bIncludeQuotations = true );
   public:
      Property<bool> autoreconnect;

      TMySQLSquirrel( RemoteSquirrelConnection *pConnection );
      ~TMySQLSquirrel();

      /// Set the query to send to the server when calling open().
      void setQuery( const String *sQuery, bool bReplaceBackticksWithDoubleQuotes = true );
	  void setQuery( const char *sQuery, bool bReplaceBackticksWithDoubleQuotes = true );

      __int64 getLastID( const char *sTableName, SquirrelReturnData *errData = NULL );

      __int64 getRecordCount();

      // implementation
      bool open( SquirrelReturnData *errData = NULL );
      bool close();

      bool isOpen();

      bool isFirst();
      bool isLast();

      bool next();

      bool fetchFields( BFields *aFields );
      bool fetchRecord( BRecord *aRecord );
};

#endif // __MySQLSquirrel_H

