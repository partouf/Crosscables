
#include "BRecord.h"

Groundfloor::BRecord::BRecord() : Groundfloor::Vector() {
   autoClear = true;
}

Groundfloor::BRecord::~BRecord() {
}

Groundfloor::BValue *Groundfloor::BRecord::newValue() {
   Groundfloor::BValue *val = new Groundfloor::BValue();

   addElement( val );

   return val;
}

Groundfloor::BValue *Groundfloor::BRecord::getValue( unsigned int iIndex ) {
   return static_cast<Groundfloor::BValue *>( elementAt( iIndex ) );
}

void Groundfloor::BRecord::setValueCount( unsigned int iCount ) {
   for ( unsigned int i = 0; i < iCount; i++ ) {
      if ( elementAt(i) == 0 ) {
         insertAt( i, new Groundfloor::BValue() );
      }
   }

   if ( autoClear ) {
      for ( unsigned int i = iCount; i < size(); i++ ) {
         if ( elementAt(i) == 0 ) {
            delete removeElement(i);
         }
      }
   }
}

bool Groundfloor::BRecord::matchInteger( unsigned int iColumn, int iNeedle ) {
   Groundfloor::BValue *val = static_cast<Groundfloor::BValue *>( elementAt( iColumn ) );

   return ( val->asInteger() == iNeedle);
}

