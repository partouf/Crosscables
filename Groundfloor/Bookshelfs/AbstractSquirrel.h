
#ifndef __AbstractSquirrel_H
#define __AbstractSquirrel_H

#include "../Atoms/Freeable.h"

#include "BRecord.h"
#include "BFields.h"


#include "../Molecules/BaseCommunicator.h"

namespace Groundfloor {

   /// commreturndata extension for errorstring
   class SquirrelReturnData : public CommReturnData {
   public:
      String errorstring;
   };

   class AbstractSquirrelConnection : public Freeable {
   protected:
      bool bConnected;
   public:
      AbstractSquirrelConnection() : Freeable() {
         bConnected = false;
      };
      ~AbstractSquirrelConnection() {
      };

      /// connects
      virtual bool connect() = 0;
      /// disconnects
      virtual bool disconnect() = 0;

      virtual bool isConnected() {
         return bConnected;
      }
   };


   /// base class for database connections
   /** The generic way of using this class to traverse through records is:
     *
     * mySquirrel->open();
     * mySquirrel->fetchFields( myFields );
     * while ( mySquirrel->next() ) {
     *    mySquirrel->fetchRecord( myRecord );
     * }
     * mySquirrel->close();
     */
   class AbstractSquirrel : public Freeable {
   protected:
      AbstractSquirrelConnection *connection;
   public:
      AbstractSquirrel(AbstractSquirrelConnection *pConnection) : Freeable() {
         this->connection = pConnection;
      };
      ~AbstractSquirrel() {
      };

      /// opens 'resource' (a table, a query, depending on implementation)
      virtual bool open(SquirrelReturnData *errData = NULL) = 0;
      /// closes 'resource'
      virtual bool close() = 0;

      /// returns the number of individual rows or records (after open())
      virtual __int64 getRecordCount() = 0;

      /// returns true if a 'resource' has been opened succesfully
      virtual bool isOpen() = 0;

      /// returns true if the current row/record is the First row/record
      virtual bool isFirst() = 0;
      /// returns true if the current row/record is the Last row/record
      virtual bool isLast() = 0;

      /// sets current row/record pointer to the first or next row/record.
      virtual bool next() = 0;

      /// returns the fields selected into the resource, should only be called once
      virtual bool fetchFields(BFields *aFields) = 0;
      /// returns the current row/record
      virtual bool fetchRecord(BRecord *aRecord) = 0;
   };

}

#endif // __AbstractSquirrel_H

