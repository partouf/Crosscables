
#include "BFields.h"
#include "BField.h"

Groundfloor::BFields::BFields() : Groundfloor::Vector() {
   autoClear = true;
}

Groundfloor::BFields::~BFields() {
}

int Groundfloor::BFields::getFieldIndex( const char *sName, unsigned int len ) {
   unsigned int c = size();
   for ( unsigned int i = 0; i < c; i++ ) {
      Groundfloor::BField *obj = static_cast<Groundfloor::BField *>( elementAt( i ) );

      if ( obj != 0 ) {
         if ( obj->name.match( sName, len ) ) {
            return i;
         }
      }
   }

   return -1;
}

int Groundfloor::BFields::getFieldIndex_ansi( const char *sName ) {
    return getFieldIndex( sName, strlen(sName) );
}

int Groundfloor::BFields::getFieldIndex( const Groundfloor::String *sName ) {
   unsigned int c = size();
   for ( unsigned int i = 0; i < c; i++ ) {
      Groundfloor::BField *obj = static_cast<Groundfloor::BField *>( elementAt( i ) );

      if ( obj != 0 ) {
         if ( obj->name.match( sName ) ) {
            return i;
         }
      }
   }

   return -1;
}

Groundfloor::BField *Groundfloor::BFields::getField( unsigned int iIndex ) {
   return static_cast<Groundfloor::BField *>( elementAt(iIndex) );
}

Groundfloor::BField *Groundfloor::BFields::newField() {
   Groundfloor::BField *field = new Groundfloor::BField();
   addElement( field );
   return field;
}

Groundfloor::BField *Groundfloor::BFields::newField( char *n, int dt ) {
   Groundfloor::BField *field = new Groundfloor::BField( n, dt );
   addElement( field );
   return field;
}
