
#ifndef __GFBRecords_H
#define __GFBRecords_H

#include "../Molecules/Vector.h"
#include "BRecord.h"

namespace Groundfloor {
   class BRecords : public Vector {
   public:
      // initializes empty vector of records
      BRecords();
      ~BRecords();

      /// returns record at given index
      BRecord *getRecord(unsigned int iIndex);
      /// appends a new empty record, usage not recommended
      BRecord *newRecord();

      /// Looks up first record where given columnindex equals the given integer value iNeedle.
      ///  This is primarily added to simplify ID-field lookups.
      BRecord *findRecordByInteger(unsigned int iColumn, int iNeedle);
   };
}

#endif // __GFBRecords_H

