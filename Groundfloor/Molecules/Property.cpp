

#include "Property.h"
#include "../Molecules/Callbacks.h"
#include "../Materials/Functions.h"

Groundfloor::BasicStringProperty::BasicStringProperty() : Groundfloor::Freeable() {
}

Groundfloor::BasicStringProperty::~BasicStringProperty() {
}

Groundfloor::String *Groundfloor::BasicStringProperty::link( bool bDoRetreive ) {
   return &anObject;
}

void Groundfloor::BasicStringProperty::set( const char *object ) {
   internalSet( object );
}

void Groundfloor::BasicStringProperty::set( const Groundfloor::String *object ) {
   internalSetCopy(object);
}

char *Groundfloor::BasicStringProperty::get() const {
   return internalGet();
}

void Groundfloor::BasicStringProperty::internalSet( const char *object ) {
   this->anObject.setValue_ansi( object );
}
void Groundfloor::BasicStringProperty::internalSetCopy( const Groundfloor::String *object ) {
   this->anObject.setValue( object );
}

char *Groundfloor::BasicStringProperty::internalGet() const {
   return this->anObject.getValue();
}

unsigned int Groundfloor::BasicStringProperty::getLength() {
   return this->anObject.getLength();
}
//----------------------------------------------------------------------------


Groundfloor::StringProperty::StringProperty() {
   this->aNotify = 0;
   this->aRetreive = 0;
}

Groundfloor::StringProperty::~StringProperty() {
   if ( ( this->aNotify != 0 ) && (this->bAutoFreeNotify) ) {
      delete this->aNotify;
   }

   if ( ( this->aRetreive != 0 ) && (this->bAutoFreeRetreive) ) {
      delete this->aRetreive;
   }
}

void Groundfloor::StringProperty::setNotify( Groundfloor::NotifyAbstract<const char *> *anEventHandler, bool bAutoFree ) {
   if ( ( this->aNotify != 0 ) && (this->bAutoFreeNotify) ) {
      delete this->aNotify;
   }

   this->aNotify = anEventHandler;
   this->bAutoFreeNotify = bAutoFree;
}

void Groundfloor::StringProperty::setRetreive( Groundfloor::RetreiveAbstract<char *> *anEventHandler, bool bAutoFree ) {
   if ( ( this->aRetreive != 0 ) && (this->bAutoFreeRetreive) ) {
      delete this->aRetreive;
   }

   this->aRetreive = anEventHandler;
   this->bAutoFreeRetreive = bAutoFree;
}

Groundfloor::String *Groundfloor::StringProperty::link( bool bDoRetreive ) {
   if ( bDoRetreive && (this->aRetreive != 0 )) {
      this->aRetreive->retreive();
   }

   return &anObject;
}

void Groundfloor::StringProperty::set( const char * object ) {
   if ( this->aNotify != 0 ) {
      this->aNotify->execute( object );
   } else {
      this->internalSet( object );
   }
}

void Groundfloor::StringProperty::set( const Groundfloor::String * object ) {
   if ( this->aNotify != 0 ) {
	   this->aNotify->execute( object->getValue() );
   } else {
	   this->internalSetCopy( object );
   }
}

char *Groundfloor::StringProperty::get() const {
   if ( this->aRetreive != 0 ) {
      return this->aRetreive->retreive();
   } else {
      return this->internalGet();
   }
}

//------------------------------------------------------------------------------


Groundfloor::BasicWideStringProperty::BasicWideStringProperty() : Groundfloor::Freeable() {
}

Groundfloor::BasicWideStringProperty::~BasicWideStringProperty() {
}

void Groundfloor::BasicWideStringProperty::set( const wchar_t *object ) {
   internalSet( object );
}

void Groundfloor::BasicWideStringProperty::set( const Groundfloor::String *object ) {
   internalSetCopy( object );
}

wchar_t *Groundfloor::BasicWideStringProperty::get() const {
   return internalGet();
}

Groundfloor::String *Groundfloor::BasicWideStringProperty::link() {
   return &anObject;
}

void Groundfloor::BasicWideStringProperty::internalSet( const wchar_t *object ) {
   this->anObject.setValue_wide( object );
}

void Groundfloor::BasicWideStringProperty::internalSetCopy( const Groundfloor::String *object ) {
   this->anObject.setValue(object);
}

wchar_t *Groundfloor::BasicWideStringProperty::internalGet() const {
   return reinterpret_cast<wchar_t *>( this->anObject.getValue() );
}

unsigned int Groundfloor::BasicWideStringProperty::getLength() {
   return this->anObject.getLength();
}
//----------------------------------------------------------------------------


Groundfloor::WideStringProperty::WideStringProperty() {
   this->aNotify = 0;
   this->aRetreive = 0;
}

Groundfloor::WideStringProperty::~WideStringProperty() {
   if ( ( this->aNotify != 0 ) && (this->bAutoFreeNotify) ) {
      delete this->aNotify;
   }

   if ( ( this->aRetreive != 0 ) && (this->bAutoFreeRetreive) ) {
      delete this->aRetreive;
   }
}

void Groundfloor::WideStringProperty::setNotify( Groundfloor::NotifyAbstract<const wchar_t *> *anEventHandler, bool bAutoFree ) {
   if ( ( this->aNotify != 0 ) && (this->bAutoFreeNotify) ) {
      delete this->aNotify;
   }

   this->aNotify = anEventHandler;
   this->bAutoFreeNotify = bAutoFree;
}

void Groundfloor::WideStringProperty::setRetreive( Groundfloor::RetreiveAbstract<wchar_t *> *anEventHandler, bool bAutoFree ) {
   if ( ( this->aRetreive != 0 ) && (this->bAutoFreeRetreive) ) {
      delete this->aRetreive;
   }

   this->aRetreive = anEventHandler;
   this->bAutoFreeRetreive = bAutoFree;
}

void Groundfloor::WideStringProperty::set( const wchar_t * object ) {
   if ( this->aNotify != 0 ) {
      this->aNotify->execute( object );
   } else {
      this->internalSet( object );
   }
}

void Groundfloor::WideStringProperty::set( const Groundfloor::String * object ) {
   if ( this->aNotify != 0 ) {
	   this->aNotify->execute( object->getValueAsWide() );
   } else {
      this->internalSetCopy( object );
   }
}

wchar_t *Groundfloor::WideStringProperty::get() const {
   if ( this->aRetreive != 0 ) {
      return this->aRetreive->retreive();
   } else {
      return this->internalGet();
   }
}
