
#include "BAttribute.h"

Groundfloor::BAttribute::BAttribute() : Groundfloor::Freeable() {
   hideFromXML = false;
}

Groundfloor::BAttribute::BAttribute( const Groundfloor::BAttribute *attrib ) : Groundfloor::Freeable() {
   hideFromXML = attrib->hideFromXML;
   key.setValue( &attrib->key );
   value.setValue( &attrib->value );
}

Groundfloor::BAttribute::BAttribute( const char *sKey ) : Groundfloor::Freeable() {
   hideFromXML = false;
   key.setValue_ansi( sKey );
}

Groundfloor::BAttribute::BAttribute( const char *sKey, const char *sValue ) : Groundfloor::Freeable() {
   hideFromXML = false;
   key.setValue_ansi( sKey );
   value.setString( sValue );
}

Groundfloor::BAttribute::BAttribute( const char *sKey, const Groundfloor::String *sValue ) : Groundfloor::Freeable() {
   hideFromXML = false;
   key.setValue_ansi( sKey );
   value.setString( sValue );
}

Groundfloor::BAttribute::BAttribute( const char *sKey, bool bValue ) : Groundfloor::Freeable() {
   hideFromXML = false;
   key.setValue_ansi( sKey );
   value.setBoolean( bValue );
}

Groundfloor::BAttribute::BAttribute( const char *sKey, long iValue ) : Groundfloor::Freeable() {
   hideFromXML = false;
   key.setValue_ansi( sKey );
   value.setInteger( iValue );
}

Groundfloor::BAttribute::BAttribute( const char *sKey, double iValue, short iDecimal ) : Groundfloor::Freeable() {
   hideFromXML = false;
   key.setValue_ansi( sKey );
   value.precision.set( iDecimal );
   value.setDouble( iValue );
}


Groundfloor::BAttribute::~BAttribute() {
}
