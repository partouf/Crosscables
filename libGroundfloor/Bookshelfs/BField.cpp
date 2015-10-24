
#include "BField.h"


Groundfloor::BField::BField() : Groundfloor::Freeable() {
   name.setValue("", 0);
   datatype.set(-1);
   maxlength.set(-1);
   flags.set(0);
}

Groundfloor::BField::BField(char *n, int dt) : Groundfloor::Freeable() {
   name.setValue_ansi(n);
   datatype.set(dt);
   maxlength.set(-1);
   flags.set(0);
}

Groundfloor::BField::~BField() {
}

