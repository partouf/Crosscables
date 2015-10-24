
#include "BRecords.h"
#include "../Atoms/Defines.h"

Groundfloor::BRecords::BRecords() : Groundfloor::Vector() {
   autoClear = true;
}

Groundfloor::BRecords::~BRecords() {
}

Groundfloor::BRecord *Groundfloor::BRecords::getRecord( unsigned int iIndex ) {
   return static_cast<Groundfloor::BRecord *>( elementAt( iIndex ) );
}

Groundfloor::BRecord *Groundfloor::BRecords::newRecord() {
   Groundfloor::BRecord *record = new Groundfloor::BRecord();
   addElement( record );

   return record;
}

Groundfloor::BRecord *Groundfloor::BRecords::findRecordByInteger( unsigned int iColumn, int iNeedle ) {
   Groundfloor::BRecord *record;

   unsigned int c = size();
   for ( unsigned int i = 0; i < c; i++ ) {
      record = static_cast<Groundfloor::BRecord *>( elementAt( i ) );

      if ( record->matchInteger( iColumn, iNeedle ) ) {
         return record;
      }
   }

   return NULL;
}

