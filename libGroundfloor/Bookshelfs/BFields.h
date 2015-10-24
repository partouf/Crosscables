
#ifndef __GFBFields_H
#define __GFBFields_H

#include "../Molecules/Vector.h"
#include "BField.h"

namespace Groundfloor {
   class BFields : public Vector {
   public:
      BFields();
      ~BFields();

      /// returns field index with given name, -1 if not found
      int getFieldIndex(const char *sName, unsigned int len);
      /// returns field index with given name, -1 if not found
      int getFieldIndex_ansi(const char *sName);
      /// returns field index with given name, -1 if not found
      int getFieldIndex(const String *sName);

      /// returns field at given index
      BField *getField(unsigned int iIndex);

      /// appends a newly created field, usage not recommended
      BField *newField();
      BField *newField(char *n, int dt);
   };
}

#endif // __GFBFields_H

