
#ifndef __GFBTable_H
#define __GFBTable_H

#include "../Atoms/Freeable.h"
#include "../Molecules/Property.h"

#include "BFields.h"
#include "BRecords.h"

namespace Groundfloor {
   /// represents a table with fields and records
   class BTable : public Freeable {
   public:
      /// Fields and records are initialized to NULL,
      ///  setting the fields and records must be done manually.
      /// Recommended: Try to fill the fields and records before setting properties.
      BTable();
      ~BTable();

      Property<BFields *> fields;
      Property<BRecords *> records;
   };
}

#endif // __GFBTable_H

