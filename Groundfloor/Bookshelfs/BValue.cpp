
#include "BValue.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include "BField.h"


Groundfloor::BValue::BValue() : Groundfloor::Freeable() {
   sInternalValue.setValue( "", 0 );
   dInternalValue = 0.0;
   precision.set( 2 );
   lasttypeset.set( 0 );
}

Groundfloor::BValue::BValue( Groundfloor::BValue *anotherValue ) : Groundfloor::Freeable() {
   setValue( anotherValue );
   precision.set( 2 );

   lasttypeset.set( anotherValue->lasttypeset.get() );
}

Groundfloor::BValue::~BValue() {
}

void Groundfloor::BValue::setValue( const Groundfloor::BValue *anotherValue ) {
   sInternalValue.setValue( &anotherValue->sInternalValue );
   dInternalValue = anotherValue->dInternalValue;

   lasttypeset.set( anotherValue->lasttypeset.get() );
}

bool Groundfloor::BValue::equals( const Groundfloor::BValue *anotherValue ) const {
   return ( dInternalValue == anotherValue->dInternalValue &&
            sInternalValue.match( &anotherValue->sInternalValue ) );
}

Groundfloor::String *Groundfloor::BValue::asString() {
   return &sInternalValue;
}

long Groundfloor::BValue::asInteger() const {
   return static_cast<int>( dInternalValue );
}

double Groundfloor::BValue::asDouble() const {
   return dInternalValue;
}

bool Groundfloor::BValue::asBoolean() const {
   if ( dInternalValue == 0 ) {
      return false;
   } else {
      return true;
   }
}

unsigned int Groundfloor::BValue::asTimestamp() const {
   return static_cast<unsigned int>( dInternalValue );
}

void Groundfloor::BValue::identifyBooleanString() {
    if ( sInternalValue.getLength() <= 4 ) {
       Groundfloor::String tmp;
       tmp.setValue( &sInternalValue );
       tmp.lowercase_ansi();
       if ( tmp.match_ansi( "true" ) || tmp.match_ansi( "yes" ) || tmp.match_ansi( "y" ) ) {
           dInternalValue = 1;
       }
    }
}

void Groundfloor::BValue::setString( const char *sValue ) {
   dInternalValue = 0;
   sInternalValue.setValue_ansi( sValue );

   try {
      dInternalValue = atof( sInternalValue.getValue() );
   } catch ( ... ) {
   }

   if ( dInternalValue == 0 ) {
      identifyBooleanString();
   }

   lasttypeset.set( FLDTYPE_STRING );
}

void Groundfloor::BValue::setString( const char *sValue, unsigned int iLength ) {
   dInternalValue = 0;
   sInternalValue.setValue( sValue, iLength );

   try {
      dInternalValue = atof( sInternalValue.getValue() );
   } catch ( ... ) {
   }

   if ( dInternalValue == 0 ) {
      identifyBooleanString();
   }

   lasttypeset.set( FLDTYPE_STRING );
}

void Groundfloor::BValue::setString( const Groundfloor::String *sValue ) {
   dInternalValue = 0;
   sInternalValue.setValue( sValue );

   try {
      dInternalValue = atof( sInternalValue.getValue() );
   } catch ( ... ) {
   }

   if ( dInternalValue == 0 ) {
      identifyBooleanString();
   }

   lasttypeset.set( FLDTYPE_STRING );
}

void Groundfloor::BValue::setInteger( long iValue ) {
   dInternalValue = static_cast<double>( iValue );
   iInternalValue = iValue;

   sInternalValue.setSize( 50 );
   sprintf( sInternalValue.getPointer(0), "%ld\0", iValue );
   sInternalValue.setLength( strlen( sInternalValue.getValue() ) );

   lasttypeset.set( FLDTYPE_INT );
}

__int64 Groundfloor::BValue::asInt64() const {
   return iInternalValue;
}

void Groundfloor::BValue::setInt64( __int64 iValue ) {
   dInternalValue = static_cast<double>( iValue );
   iInternalValue = iValue;

   sInternalValue.setSize( 50 );
   #ifdef GF_OS_WIN32
   sprintf( sInternalValue.getPointer(0), "%I64u\0", iValue );
   #else
   sprintf( sInternalValue.getPointer(0), "%llu\0", iValue );
   #endif
   sInternalValue.setLength( strlen( sInternalValue.getValue() ) );

   lasttypeset.set( FLDTYPE_INT64 );
}

void Groundfloor::BValue::setDouble( double dValue ) {
   dInternalValue = dValue;
   iInternalValue = static_cast<__int64>( dValue );

   sInternalValue.setSize( 100 );
   sprintf( sInternalValue.getPointer(0), "%.*f\0", precision.get(), dValue );
   sInternalValue.setLength( strlen( sInternalValue.getValue() ) );
   sInternalValue.replace_ansi( ",", "." );

   lasttypeset.set( FLDTYPE_DOUBLE );
}

void Groundfloor::BValue::setBoolean( bool bValue ) {
   if ( bValue ) {
      dInternalValue = 1;
      sInternalValue.setValue_ansi( "true" );
   } else {
      dInternalValue = 0;
      sInternalValue.setValue_ansi( "false" );
   }

   lasttypeset.set( FLDTYPE_BOOL );
}

void Groundfloor::BValue::setTimestamp( __int64 iValue, bool bXml ) {
   dInternalValue = static_cast<double>( iValue );
   iInternalValue = iValue;

   time_t t = iValue;
   struct tm *i;

   i = localtime( &t );

   if ( i != NULL ) {
      i->tm_year  += 1900;
      i->tm_mon   += 1;

      sInternalValue.setSize( 20 );
      if ( bXml ) {
         sprintf( sInternalValue.getPointer(0), "%04d-%02d-%02dT%02d:%02d:%02d\0", i->tm_year, i->tm_mon, i->tm_mday, i->tm_hour, i->tm_min, i->tm_sec );
      } else {
         sprintf( sInternalValue.getPointer(0), "%04d-%02d-%02d %02d:%02d:%02d\0", i->tm_year, i->tm_mon, i->tm_mday, i->tm_hour, i->tm_min, i->tm_sec );
      }
      sInternalValue.setLength( 19 );

      // delete i;   ???
   } else {
      sInternalValue.setValue( "", 0 );
   }

   lasttypeset.set( FLDTYPE_DATE );
}
