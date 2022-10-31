
#ifndef __GFBField_H
#define __GFBField_H

namespace Groundfloor {
   class BField;
}

#include "../Atoms/Freeable.h"
#include "../Molecules/String.h"
#include "../Molecules/Property.h"

#define FLDTYPE_STRING 0
#define FLDTYPE_INT 1
#define FLDTYPE_DOUBLE 2
#define FLDTYPE_DATE 3
#define FLDTYPE_INT64 4
#define FLDTYPE_BOOL 5
#define FLDTYPE_UNKNOWN 255

#define FLDFLAG_NORMAL 0
#define FLDFLAG_PRIMARY 1


namespace Groundfloor {
   class BField : public Freeable {
   public:
      /// intializes field to unnamed, datatype -1, no maximum length, flagged normal
      BField();
      BField(char *n, int dt);
      ~BField();

      /// name of field
      String name;
      /// datatype of data
      Property<int> datatype;
      /// maximum length of data, set to -1 when no maximum length needed
      Property<int> maxlength;
      /// flags belonging to this field
      Property<int> flags;
   };
}

#endif // __GFBField_H

