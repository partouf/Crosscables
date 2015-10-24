
#ifndef __GFBRecord_H
#define __GFBRecord_H

#include "../Molecules/Vector.h"
#include "BValue.h"

namespace Groundfloor {
   class BRecord : public Vector {
   public:
      BRecord();
      ~BRecord();

      /// appends a new empty value to record, usage not recommended
      BValue *newValue();
      /// returns value by given index
      BValue *getValue(unsigned int iIndex);

      /// creates or deletes values to match the given number of values
      void setValueCount(unsigned int iCount);

      /// returns true if the integer representation of the value at iColumn equals iNeedle
      bool matchInteger(unsigned int iColumn, int iNeedle);
   };
}

#endif // __GFBRecord_H

